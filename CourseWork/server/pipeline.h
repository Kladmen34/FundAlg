#pragma once

const char * searchable_fields[] = {
    "buildId",
    "buildVer",
    "commitHash",
    "devLogin",
    "devEmail",
    "buildPath",
    "buildName",
    "errorsStaticAnalyze",
    "errorsAutoTests",
    "pathArtifact",
};

enum PLR_Fields {
    fBuildId,
    fBuildVer,
    fCommitHash,
    fDevLogin,
    fDevEmail,
    fBuildPath,
    fBuildName,
    fErrorsStaticAnalyze,
    fErrorsAutoTests,
    fPathArtifact,
    MIN_FIELDID = fBuildId,
    MAX_FIELDID = fPathArtifact+1,
};

#define PLR_MAX_BUILDID        6
#define PLR_MAX_BUILDVER      12
#define PLR_MAX_COMMITHASH    42
#define PLR_MAX_LOGIN         12
#define PLR_MAX_EMAIL         20
#define PLR_MAX_BUILDPATH    256
#define PLR_MAX_BUILDNAME     10
#define PLR_MAX_ERRSTATIC    256
#define PLR_MAX_ERRAUTOTEST  256
#define PLR_MAX_ARTIFACTPATH 256
#define PLR_MAX_TOKEN         65

size_t filelds_max_size[] = {
    PLR_MAX_BUILDID,
    PLR_MAX_BUILDVER,
    PLR_MAX_COMMITHASH,
    PLR_MAX_LOGIN,
    PLR_MAX_EMAIL,
    PLR_MAX_BUILDPATH,
    PLR_MAX_BUILDNAME,
    PLR_MAX_ERRSTATIC,
    PLR_MAX_ERRAUTOTEST,
    PLR_MAX_ARTIFACTPATH,
};

struct PLR_CLI {
    char buildId[PLR_MAX_BUILDID];               
    char buildVer[PLR_MAX_BUILDVER];            
    char commitHash[PLR_MAX_COMMITHASH];         
    char devLogin[PLR_MAX_LOGIN];                
    char devEmail[PLR_MAX_EMAIL];                
    char buildPath[PLR_MAX_BUILDPATH];           
    char buildName[PLR_MAX_BUILDNAME];           
    char errorsStaticAnalyze[PLR_MAX_ERRSTATIC]; 
    char errorsAutoTests[PLR_MAX_ERRAUTOTEST];   
    char pathArtifact[PLR_MAX_ARTIFACTPATH];     
};

#define PLR_STRING_MESSAGE_MAX 256

struct PLR_STRING_MESSAGE {
    char data[PLR_STRING_MESSAGE_MAX];
};

struct PLR_MESSAGE {
    long mtype;
    unsigned char code;
    char tok[PLR_MAX_TOKEN];
    union {
        PLR_CLI plr;
        PLR_STRING_MESSAGE sm;
    };
};

#define PLR_MESSAGE_SIZE (sizeof(PLR_MESSAGE) - sizeof(long))

enum PLR_Request {
    reqAdd,
    reqFind,
    reqDel,
    reqRegister,
    reqLogin,
};
