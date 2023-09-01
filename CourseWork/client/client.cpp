#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <errno.h>
#include <cstring>
#include "pipeline.h"
#include "sha.hpp"
#include "mq.h"

std::unordered_set<size_t> optional_fields = { 7, 8, 9 }; 
std::string token;

bool paramInSearchable(const std::string & param) {
    for (const char * par : searchable_fields) {
        if (param.compare(par) == 0) {
            return true;
        }
    }
    return false;
}

std::string hashString(const std::string & str) {
    SHA3 sha3;
    sha3.add(str.c_str(),str.size());
    return sha3.getHash();
}

bool processRegister(std::string& line, PLR_MESSAGE & msg) {
    const int cmd_offset = 9;
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
    bool ok = !login.empty()
            && (password.size() >= 8)
            && !userGroup.empty();
    if (ok) {
        ok =  (userGroup == "admin")
           || (userGroup == "editor")
           || (userGroup == "user");
    }
    if (ok) {
        std::stringstream ss;
        ss << login << " "
           << hashString(password) << " "
           << userGroup;
        msg.code = reqRegister;
        strcpy(msg.sm.data, ss.str().c_str());
    }
    return ok;
}

bool processLogin(std::string& line, PLR_MESSAGE & msg) {
    const int cmd_offset = 6;
    size_t idx = line.find(" ", cmd_offset);
    if (idx == std::string::npos) {
        return false;
    }
    std::string login = line.substr(cmd_offset, idx-cmd_offset);
    std::string password = line.substr(idx+1);
    bool ok = !login.empty() && !password.empty();
    if (ok) {
        std::stringstream ss;
        ss << login << " " << hashString(password);
        msg.code = reqLogin;
        strcpy(msg.sm.data, ss.str().c_str());
    }
    return ok;
}

bool processAdd(std::string& line, PLR_MESSAGE & msg) {
    std::string buildId = line.substr(4);
    if (buildId.empty()) {
        return false;
    }
    line += '\n';
    std::string buff;

    char* pfields[] = {
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

    if (buildId.size() >= filelds_max_size[0]) {
        return false;
    }
    strncpy(pfields[0], buildId.c_str(), filelds_max_size[0]);

    for (size_t field_idx = 1; field_idx < 10; ++field_idx) {
        std::getline(std::cin, buff);
        if (buff.empty() && !optional_fields.count(field_idx)) {
            return false;
        }
        if (buff.size() >= filelds_max_size[field_idx]) {
            return false;
        }
        strncpy(pfields[field_idx], buff.c_str(), filelds_max_size[field_idx]);
    }
    msg.code = reqAdd;
    return true;
}

bool processDel(std::string& line, PLR_MESSAGE & msg) {
    size_t idx = 4;
    idx = line.find("==", idx);
    if (idx == std::string::npos) {
        idx = line.find("!=", 0);
    }
    if (idx == std::string::npos) {
        return false;
    }
    std::string param = line.substr(4, idx-4);
    std::string value = line.substr(idx + 2);
    bool ok = !param.empty()
            && !value.empty()
            && paramInSearchable(param)
            && (line.size() < PLR_STRING_MESSAGE_MAX);
    if (ok) {
        msg.code = reqDel;
        strcpy(msg.sm.data, line.c_str()+4);
    }
    return ok;
}

bool processFind(std::string& line, PLR_MESSAGE & msg) {
    size_t idx = 5;
    idx = line.find("==", idx);
    if (idx == std::string::npos) {
        idx = line.find("!=", 0);
    }
    if (idx == std::string::npos) {
        return false;
    }
    std::string param = line.substr(5, idx-5);
    std::string value = line.substr(idx + 2);
    bool ok = !param.empty()
            && !value.empty()
            && paramInSearchable(param)
            && (line.size() < PLR_STRING_MESSAGE_MAX);
    if (ok) {
        msg.code = reqFind;
        strcpy(msg.sm.data, line.c_str()+5);
    }
    return ok;
}

int main() {
    MQ mq;
    if (!mq.Create(QUEUE_UPNAME)) {
        fprintf(stderr, "[ERR] cannot open UP queue: '%s'.\n", strerror(errno));
        return EXIT_FAILURE;
    }
    std::string line;
    PLR_MESSAGE msg;
    char* pfields[] = {
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
    bool waitingToken = false;
    for (;;) {
        memset(&msg, 0, PLR_MESSAGE_SIZE);
        strcpy(msg.tok, token.c_str());
        std::cout << " > ";
        std::getline(std::cin, line);
        bool ok = false;
        if (line == "quit") {
            break;
        } else if (line.rfind("add ", 0) == 0) {
            ok = processAdd(line, msg);
        } else if (line.rfind("del ", 0) == 0) {
            ok = processDel(line, msg);
        } else if (line.rfind("find ", 0) == 0) {
            ok = processFind(line, msg);
        } else if (line.rfind("register ", 0) == 0) {
            ok = processRegister(line, msg);
        } else if (line.rfind("login ", 0) == 0) {
            waitingToken = true;
            ok = processLogin(line, msg);
        } else {
            std::cout << "ERR: unknown command '" << line << "'." << std::endl;
            continue;
        }
        if (!ok) {
            waitingToken = false;
            std::cout << "ERR: invalid command." << std::endl;
            continue;
        }

        msg.mtype=1;
        mq.Write((const char*)&msg, PLR_MESSAGE_SIZE);
        memset(&msg, 0, PLR_MESSAGE_SIZE);
        ssize_t len = QUEUE_MSGSIZE;
        mq.Read((char*)&msg, len);
        if (len > 0) {
            if (msg.code == 0 && msg.sm.data[0]) {
                if (!waitingToken) {
                    std::cout << msg.sm.data << std::endl;
                } else {
                    token = std::string(msg.sm.data);
                    if (token.rfind("ERR: ", 0) == 0) {
                        std::cout << msg.sm.data << std::endl;
                        token.clear();
                    } else {
                        std::cout << "OK: login successful." << std::endl;
                    }
                    waitingToken = false;
                }
            } else if (msg.code >= 1) {
                int total = msg.code;
                for (;;) {
                    for (int i = 0; i < 10; ++i) {
                        std::cout << searchable_fields[i]
                                  << ": \"" << pfields[i]
                                  << "\"" << std::endl;
                    }
                    std::cout << std::endl;

                    total -= 1;
                    if (total == 0) break;

                    msg.code = 255;
                    mq.Read((char*)&msg, len);
                    if (msg.code == 255) break;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
