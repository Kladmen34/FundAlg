#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <vector>
#include <errno.h>
#include <signal.h>

#include "rbt.h"
#include "stringpool.h"
#include "pipeline.h"
#include "sha.hpp"
#include "mq.h"

typedef std::vector<PSTR> PLR;
typedef PLR* PPLR;

volatile sig_atomic_t gWorking = 1;
void ctrlc_signal_handler(int){
    gWorking = 0;
}

const char * store_filename = "base.dat";
const char * users_filename = "users.dat";
const char * admin_login    = "admin";
const char * admin_pass     = "a03ab19b866fc585b5cb1812a2f63ca861e7e7643ee5d43fd7106b623725fd67"; // 123

enum UserGroup {
    ugAdmin,
    ugEditor,
    ugUser,
};

struct UserInfo {
    std::string login;
    std::string password_hash;
    std::string token;
    UserGroup groupId;
};

RBTree<std::string, UserInfo*> users_db;       // login -> password hash
RBTree<std::string, UserInfo*> tok_2_user;     // token -> user

RBTree<PSTR, PPLR> byBuildId;
RBTree<PSTR, std::unordered_set<PPLR>> byBuildVer;
RBTree<PSTR, std::unordered_set<PPLR>> byCommitHash;
RBTree<PSTR, std::unordered_set<PPLR>> byDevLogin;
RBTree<PSTR, std::unordered_set<PPLR>> byDevEmail;
RBTree<PSTR, std::unordered_set<PPLR>> byBuildPath;
RBTree<PSTR, std::unordered_set<PPLR>> byBuildName;
RBTree<PSTR, std::unordered_set<PPLR>> byErrorsStaticAnalyze;
RBTree<PSTR, std::unordered_set<PPLR>> byErrorsAutoTests;
RBTree<PSTR, std::unordered_set<PPLR>> byPathArtifact;

RBTree<PSTR, std::unordered_set<PPLR>>* byFinder[] = {
    nullptr,
    &byBuildVer,
    &byCommitHash,
    &byDevLogin,
    &byDevEmail,
    &byBuildPath,
    &byBuildName,
    &byErrorsStaticAnalyze,
    &byErrorsAutoTests,
    &byPathArtifact,
};

std::string hashString(const std::string & str) {
    SHA3 sha3;
    sha3.add(str.c_str(),str.size());
    return sha3.getHash();
}


bool processRegister(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    assert(request.code == reqRegister);
    const int cmd_offset = 0;
    std::string line(request.sm.data);
    size_t idx = line.find(" ", cmd_offset);
    if (idx == std::string::npos) {
        return false;
    }
    size_t idx2 = line.find(" ", idx+1);
    if (idx2 == std::string::npos) {
        return false;
    }
    std::string login = line.substr(cmd_offset, idx-cmd_offset);
    std::string password = line.substr(idx+1, idx2-idx-1);
    std::string userGroup = line.substr(idx2+1);
    UserGroup ugroup;
    bool ok = (login.size() >= 5 && login.size() <= 15)
            && !password.empty()
            && !userGroup.empty();
    if (ok) {
        for (char ch : login) {
            if (!std::isalnum(ch)) {
                ok = false;
                break;
            }
        }
    }
    if (ok) {
        if (userGroup == "admin") {
            ugroup = ugAdmin;
        } else if (userGroup == "editor") {
            ugroup = ugEditor;
        } else if (userGroup == "user") {
            ugroup = ugUser;
        } else {
            ok = false;
        }
    }
    PLR_MESSAGE msg;
    msg.code = 0;
    if (!ok) {
        strcpy(msg.sm.data, "ERR: invalid request.");
    } else {
        if (users_db.Contains(login)) {
            strcpy(msg.sm.data, "ERR: login is taken.");
        } else {
            UserInfo * uinfo = new UserInfo;
            uinfo->login         = login;
            uinfo->password_hash = password;
            uinfo->groupId       = ugroup;

            users_db.Insert(login, uinfo);
            strcpy(msg.sm.data, "OK: register successful.");
        }
    }
    resp_messages.emplace_back( std::move(msg) );
    return true;
}


bool processLogin(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    assert(request.code == reqLogin);
    const int cmd_offset = 0;
    std::string line(request.sm.data);
    size_t idx = line.find(" ", cmd_offset);
    if (idx == std::string::npos) {
        return false;
    }
    std::string login = line.substr(cmd_offset, idx-cmd_offset);
    std::string password = line.substr(idx+1);
    bool ok = !login.empty() && !password.empty();

    PLR_MESSAGE msg;
    msg.code = 0;
    if (!ok) {
        strcpy(msg.sm.data, "ERR: invalid request.");
    } else {
        if (!users_db.Contains(login)) {
            strcpy(msg.sm.data, "ERR: bad user or password.");
        } else {
            UserInfo * cur_user = users_db.Get(login);
            if (!cur_user || cur_user->password_hash != password) {
                strcpy(msg.sm.data, "ERR: bad user or password.");
            } else {
                std::stringstream ss;
                ss << "1:" << cur_user->password_hash;
                std::string token = hashString(ss.str());
                strcpy(msg.sm.data, token.c_str());
                if (cur_user->token.empty()) {
                    tok_2_user.Remove(cur_user->token);
                }
                cur_user->token = token;
                if (tok_2_user.Contains(token)) {
                    tok_2_user.Update(token, cur_user);
                } else {
                    tok_2_user.Insert(token, cur_user);
                }
            }
        }
    }
    resp_messages.emplace_back( std::move(msg) );
    return true;
}


bool processAdd(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    assert(request.code == reqAdd);

    const char* fields[] = {
        request.plr.buildId,
        request.plr.buildVer,
        request.plr.commitHash,
        request.plr.devLogin,
        request.plr.devEmail,
        request.plr.buildPath,
        request.plr.buildName,
        request.plr.errorsStaticAnalyze,
        request.plr.errorsAutoTests,
        request.plr.pathArtifact,
    };

    StringPool & sp  = StringPool::instance();
    PSTR pstrBuildId = sp.find(fields[0]);

    if (byBuildId.Contains(pstrBuildId)) {
        PLR_MESSAGE msg;
        msg.code = 0;
        strcpy(msg.sm.data, "ERR: already exists");
        resp_messages.push_back(msg);
        return true;
    }

    PPLR pRecord = new PLR;
    pRecord->push_back(pstrBuildId);

    for (int index = 1; index < 10; ++index) {
        PSTR pstrField = sp.find(fields[index]);
        pRecord->push_back(pstrField);
        (*byFinder[index]).Get(pstrField).insert(pRecord);
    }
    byBuildId.Insert(pstrBuildId, pRecord);

    PLR_MESSAGE msg;
    msg.code = 0;
    msg.sm.data[0] = '\0';
    resp_messages.emplace_back(std::move(msg));
    return true;
}


int paramInSearchable(const std::string & param) {
    for (int index = 0; index < 10; ++index) {
        if (param.compare(searchable_fields[index]) == 0) {
            return index;
        }
    }
    return -1;
}


bool parseParamString(std::string line,
                      int & param_index,
                      std::string & value,
                      int & operation) {
    size_t idx = 0;
    operation = -1;
    idx = line.find("==", 0);
    if (idx != std::string::npos) {
        operation = 0;
    } else {
        idx = line.find("!=", 0);
        if (idx != std::string::npos) {
            operation = 1;
        }
    }
    if (operation == -1) {
        return false;
    }
    std::string param = line.substr(0, idx);
    value = line.substr(idx + 2);
    bool ok = !param.empty() && !value.empty();
    if (ok) {
        param_index = paramInSearchable(param);
        ok = param_index != -1;
    }
    return ok;
}


int findByParam(std::string line, std::vector< PPLR > & records) {
    int param_index = -1;
    std::string value;
    int operation = -1;
    bool ok = parseParamString(line, param_index, value, operation);
    if (!ok) {
        return -1;
    }
    StringPool & sp  = StringPool::instance();
    PSTR pstr = sp.find(value);
    records.clear();

    if (param_index == 0) {
        if (operation == 0) {
            if (!byBuildId.Contains(pstr)) {
                return -2;
            }
            PPLR pRecord = byBuildId.Get(pstr);
            if (pRecord) {
                records.push_back(pRecord);
            }
        } else if (operation == 1) {
            auto comparer = [pstr](PSTR cur_str) {
                return pstr != cur_str;
            };
            auto functor = [&records](PPLR pplr) {
                records.push_back(pplr);
            };
            byBuildId.InorderWalk(comparer, functor);
        }
    } else {
        auto pFinder = byFinder[param_index];
        if (operation == 0) {
            for (PPLR pplr : (*pFinder).Get(pstr)) {
                records.push_back(pplr);
            }
        } else if (operation == 1) {
            auto comparer = [pstr](PSTR cur_str) {
                return pstr != cur_str;
            };
            auto functor = [&records](const std::unordered_set<PPLR> & cur_set) {
                for (PPLR pplr : cur_set) {
                    records.push_back(pplr);
                }
            };
            (*pFinder).InorderWalk(comparer, functor);
        }
    }
    return 0;
}


bool processFind(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    assert(request.code == reqFind);

    std::vector<PPLR> records;
    int rc = findByParam(request.sm.data, records);

    if (rc < 0) {
        PLR_MESSAGE msg;
        switch (rc) {
        case -1:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: invalid request.");
            break;

        case -2:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: not found.");
            break;

        case -3:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: not found.");
            break;

        default:
            assert(!"not implimented");
            break;
        }
        resp_messages.push_back(msg);
        return true;
    }

    assert (records.size() != 0);

    PLR_MESSAGE msg;
    char* fields[] = {
        msg.plr.buildId,
        msg.plr.buildVer,
        msg.plr.commitHash,
        msg.plr.devLogin,
        msg.plr.devEmail,
        msg.plr.buildPath,
        msg.plr.buildName,
        msg.plr.errorsStaticAnalyze,
        msg.plr.errorsAutoTests,
        msg.plr.pathArtifact,
    };
    int total = records.size();
    for (PPLR plr : records) {
        for (int i = 0; i < 10; ++i) {
            strcpy(fields[i], (*plr)[i]);
        }
        msg.code = total--;
        resp_messages.emplace_back(std::move(msg));
        if (total == 0) break;
    }
    return true;
}


bool processDel(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    assert(request.code == reqDel);
    std::vector<PPLR> records;
    int rc = findByParam(request.sm.data, records);

    if (rc < 0) {
        PLR_MESSAGE msg;
        switch (rc) {
        case -1:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: invalid request.");
            break;

        case -2:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: not found.");
            break;

        case -3:
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: not found.");
            break;

        default:
            assert(!"not implimented");
            break;
        }
        resp_messages.push_back(msg);
        return true;
    }

    assert (records.size() != 0);
    for (PPLR pplr : records) {
        auto ref_vec = *pplr;
        byBuildId.Remove(ref_vec[0]);
        for (int i = 1; i < 10; ++i) {
            (*byFinder[i]).Get(ref_vec[i]).erase( pplr );
        }
    }

    PLR_MESSAGE msg;
    msg.code = 0;
    sprintf(msg.sm.data, "OK: %d rows affected.", (int) records.size());
    resp_messages.push_back(msg);
    return true;
}

UserInfo * getUserByToken(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    std::string tok(request.tok);
    if (tok.empty() || !tok_2_user.Contains(tok)) {
        PLR_MESSAGE msg;
        msg.code = 0;
        strcpy(msg.sm.data, "ERR: login first!");
        resp_messages.emplace_back( std::move(msg) );
        return nullptr;
    }
    return tok_2_user.Get(tok);
}


bool processMessage(const PLR_MESSAGE & request, std::vector<PLR_MESSAGE> & resp_messages) {
    resp_messages.clear();

    {
        UserGroup ugroup;
        UserInfo * uinfo = nullptr;
        bool groupError = false;
        if (request.code == reqLogin) {
            ugroup = ugAdmin;
        } else {
            uinfo = getUserByToken(request, resp_messages);
            if (!uinfo) {
                return true;
            }
            ugroup = uinfo->groupId;
        }
        switch (request.code) {
        case reqAdd:      groupError = ugroup > ugEditor; break;
        case reqFind:     groupError = ugroup > ugUser; break;
        case reqDel:      groupError = ugroup > ugEditor; break;
        case reqRegister: groupError = ugroup > ugAdmin; break;
        case reqLogin:    groupError = false; break;
        default:          groupError = false; break;
        }
        if (groupError) {
            PLR_MESSAGE msg;
            msg.code = 0;
            strcpy(msg.sm.data, "ERR: acess denied.");
            resp_messages.emplace_back( std::move(msg) );
            return true;
        }
    }

    switch (request.code) {
    case reqAdd:
        return processAdd(request, resp_messages);

    case reqFind:
        return processFind(request, resp_messages);

    case reqDel:
        return processDel(request, resp_messages);

    case reqRegister:
        return processRegister(request, resp_messages);

    case reqLogin:
        return processLogin(request, resp_messages);

    default:
        std::cerr << "ERR: unknown message" << std::endl;
        break;
    }
    return false;
}

void initUsers() {
    if (users_db.IsEmpty()) {
        UserInfo * uinfo = new UserInfo;
        uinfo->login = admin_login;
        uinfo->password_hash = admin_pass;
        uinfo->groupId = ugAdmin;
        users_db.Insert( uinfo->login, uinfo );
    }
}


void stateLoad() {
    {
        std::ifstream input(store_filename, std::ios::in | std::ios::binary);
        if (!input) {
            return;
        }
        byBuildId.Clear();
        for (int i = 1; i < 10; ++i) {
            (*byFinder[i]).Clear();
        }

        PLR_CLI plr;
        const char* fields[] = {
            plr.buildId,
            plr.buildVer,
            plr.commitHash,
            plr.devLogin,
            plr.devEmail,
            plr.buildPath,
            plr.buildName,
            plr.errorsStaticAnalyze,
            plr.errorsAutoTests,
            plr.pathArtifact,
        };
        StringPool & sp = StringPool::instance();

        while (input) {
            input.read((char *)&plr, sizeof(PLR_CLI));
            PSTR pstrBuildId = sp.find(fields[0]);
            if (byBuildId.Contains(pstrBuildId)) {
                continue;
            }
            PPLR pRecord = new PLR;
            pRecord->push_back(pstrBuildId);

            for (int index = 1; index < 10; ++index) {
                PSTR pstrField = sp.find(fields[index]);
                pRecord->push_back(pstrField);
                (*byFinder[index]).Get(pstrField).insert(pRecord);
            }
            byBuildId.Insert(pstrBuildId, pRecord);
        }
        input.close();
    }
}


void stateStore() {
    std::ofstream wf(store_filename, std::ios::out | std::ios::binary);
    if (!wf) {
        std::cerr << "Cannot open store file!" << std::endl;
        return;
    }
    auto functor = [&wf](PPLR pplr) {
        PLR_CLI plr;
        char* fields[] = {
            plr.buildId,
            plr.buildVer,
            plr.commitHash,
            plr.devLogin,
            plr.devEmail,
            plr.buildPath,
            plr.buildName,
            plr.errorsStaticAnalyze,
            plr.errorsAutoTests,
            plr.pathArtifact,
        };
        auto & ref_vec = (*pplr);
        assert(ref_vec.size() == 10);
        for (int i = 0; i < 10; ++i) {
            strcpy(fields[i], ref_vec[i]);
        }
        wf.write((char *) &plr, sizeof(PLR_CLI));
    };
    byBuildId.InorderWalk(functor);
}

void TestRBT() {
    RBTree<int, std::string> tree;
    tree.Insert(1, "one");
    tree.Insert(2, "two");
    tree.Insert(3, "three");
    tree.Insert(4, "four");

    assert(tree.Contains(1));
    assert(tree.Get(1) == "one");
    assert(tree.Contains(2));
    assert(tree.Get(2) == "two");
    assert(tree.Contains(3));
    assert(tree.Get(3) == "three");
    assert(tree.Contains(4));
    assert(tree.Get(4) == "four");
    assert(tree.Size() == 4);

    tree.Remove(2);

    assert(tree.Contains(1));
    assert(tree.Get(1) == "one");
    assert(!tree.Contains(2));
    assert(tree.Contains(3));
    assert(tree.Get(3) == "three");
    assert(tree.Contains(4));
    assert(tree.Get(4) == "four");
    assert(tree.Size() == 3);

    tree.Remove(3);

    assert(tree.Contains(1));
    assert(tree.Get(1) == "one");
    assert(!tree.Contains(2));
    assert(!tree.Contains(3));
    assert(tree.Contains(4));
    assert(tree.Get(4) == "four");
    assert(tree.Size() == 2);

    tree.Remove(1);

    assert(!tree.Contains(1));
    assert(!tree.Contains(2));
    assert(!tree.Contains(3));
    assert(tree.Contains(4));
    assert(tree.Get(4) == "four");
    assert(tree.Size() == 1);

    tree.Remove(4);

    assert(!tree.Contains(1));
    assert(!tree.Contains(2));
    assert(!tree.Contains(3));
    assert(!tree.Contains(4));
    assert(tree.Size() == 0);
}

void TestStringPool() {
    StringPool& sp = StringPool::instance();
    const char* string = sp.find("string");
    assert(string != nullptr);
    const char* another_string = sp.find("string");
    assert(string == another_string);
}

void TestRegister() {
    {
        users_db.Clear();
        tok_2_user.Clear();
        initUsers();

        PLR_MESSAGE msg;
        msg.code = reqRegister;
        const char * testLogin = "";
        const char * testPass  = "password";
        strcpy(msg.sm.data, testLogin);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, testPass);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, "user");

        std::vector<PLR_MESSAGE> resp_messages;
        processMessage(msg, resp_messages);

        assert(resp_messages.size() == 1);
        assert(!users_db.Contains(testLogin));
    }
    {
        users_db.Clear();
        tok_2_user.Clear();

        PLR_MESSAGE msg;
        msg.code = reqRegister;
        const char * testLogin = "login";
        const char * testPass  = "password";
        strcpy(msg.sm.data, testLogin);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, testPass);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, "user");

        std::vector<PLR_MESSAGE> resp_messages;
        processMessage(msg, resp_messages);

        assert(resp_messages.size() == 1);
        assert(!users_db.Contains(testLogin));
    }

    {
        users_db.Clear();
        tok_2_user.Clear();
        initUsers();

        PLR_MESSAGE msg;
        std::vector<PLR_MESSAGE> resp_messages;

        msg.code = reqLogin;
        strcpy(msg.sm.data, admin_login);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, admin_pass);

        processMessage(msg, resp_messages);

        assert(resp_messages.size() == 1);
        auto & resp = resp_messages.at(0);
        std::string adm_tok(resp.sm.data);

        msg.code = reqRegister;
        strcpy(msg.tok, adm_tok.c_str());
        const char * testLogin = "login";
        const char * testPass  = "password";
        strcpy(msg.sm.data, testLogin);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, testPass);
        strcat(msg.sm.data, " ");
        strcat(msg.sm.data, "user");

        processMessage(msg, resp_messages);

        assert(resp_messages.size() == 1);
        assert(users_db.Contains(testLogin));
        UserInfo * uinfo = users_db.Get(testLogin);
        assert(uinfo->groupId == ugUser);
    }

}

void TestLogin() {
    const char * testLogin = "login";
    const char * testPass  = "password";

    UserInfo *uinfo = new UserInfo;
    uinfo->login = testLogin;
    uinfo->password_hash = testPass;
    uinfo->groupId = ugUser;

    users_db.Clear();
    users_db.Insert(testLogin, uinfo);
    tok_2_user.Clear();

    PLR_MESSAGE msg;
    msg.code = reqLogin;
    strcpy(msg.sm.data, testLogin);
    strcat(msg.sm.data, " ");
    strcat(msg.sm.data, testPass);

    std::vector<PLR_MESSAGE> resp_messages;
    processMessage(msg, resp_messages);

    assert(resp_messages.size() == 1);
    std::string token( resp_messages[0].sm.data );
    assert(!token.empty());
    assert(tok_2_user.Contains(token));

    msg.code = reqAdd;
    msg.tok[0] = '\0';
    strcpy(msg.plr.buildId, "10");
    strcpy(msg.plr.buildVer, "1.0.0.0");
    strcpy(msg.plr.commitHash, "e1cbbce92f1d177019fb89b7ef589369f4026be5");
    strcpy(msg.plr.devLogin, "devlogin");
    strcpy(msg.plr.devEmail, "devlogin@mail.com");
    strcpy(msg.plr.buildPath, "/build/path");
    strcpy(msg.plr.buildName, "buildName");
    strcpy(msg.plr.errorsStaticAnalyze, "");
    strcpy(msg.plr.errorsAutoTests, "");
    strcpy(msg.plr.pathArtifact, "/artifact/path");

    processMessage(msg, resp_messages);
    {
        assert(resp_messages.size() == 1);
        const PLR_MESSAGE & resp = resp_messages.at(0);
        assert(resp.code == 0);
        assert(resp.sm.data[0] != '\0');
    }

    strcpy(msg.tok, token.c_str());
    processMessage(msg, resp_messages);
    {
        assert(resp_messages.size() == 1);
        const PLR_MESSAGE & resp = resp_messages.at(0);
        assert(resp.code == 0);
        assert(strcmp(resp.sm.data, "ERR: acess denied.")==0);
    }

    uinfo->groupId = ugEditor;
    processMessage(msg, resp_messages);
    {
        assert(resp_messages.size() == 1);
        const PLR_MESSAGE & resp = resp_messages.at(0);
        assert(resp.code == 0);
        assert(resp.sm.data[0]=='\0');
    }
}

void Tests() {
    TestRBT();
    TestStringPool();
    TestRegister();
    TestLogin();
}

int main() {

#if 0
    Tests();
    return EXIT_SUCCESS;
#endif

    MQ mq;
    if (!mq.Create(QUEUE_UPNAME)) {
        std::cerr << "[ERR] cannot create UP queue: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    PLR_MESSAGE message;
    ssize_t readed = 0;
    std::vector<PLR_MESSAGE> resp_messages;
    stateLoad();
    initUsers();

    signal(SIGINT, ctrlc_signal_handler);
    while (gWorking) {
        MQ::Sleep();
        readed = PLR_MESSAGE_SIZE;
        mq.Read((char*)&message, readed);
        if (readed <= 0) {
            continue;
        }
        std::cout << "readed: "<< readed << std::endl;
        if (!processMessage(message, resp_messages)) {
            continue;
        }
        for (PLR_MESSAGE & resp_msg : resp_messages) {
            resp_msg.mtype = 1;
            mq.Write((const char*)&resp_msg, PLR_MESSAGE_SIZE);
        }
    }
    stateStore();
    mq.Unlink();
    std::cout << "Bye" << std::endl;
}
