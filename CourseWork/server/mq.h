#pragma once

#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <cstring>
#include "pipeline.h"

#define QUEUE_UPNAME  "mq.h"      
#define QUEUE_MSGSIZE (PLR_MESSAGE_SIZE+1)  
#define QUEUE_POLL_CONSUMER ((struct timespec){0, 5000000})
#define QUEUE_POLL_PUBLISHER ((struct timespec){5, 0})

class MQ {
    bool m_isOK = false;
    key_t key;
    int msgid;

public:

    MQ() {}

    ~MQ() {}

    bool isOk() const {
        return m_isOK;
    }

    bool Create(const char * name) {
        key = ftok(name, 65);
        msgid = msgget(key, 0666 | IPC_CREAT);
        if (msgid == -1) {
            return false;
        }
        m_isOK = true;
        return true;
    }

    void Unlink() {
        if (msgid != -1) {
            msgctl(msgid, IPC_RMID, NULL);
            msgid = -1;
        }
    }

    void Read(char * buffer, ssize_t & count) {
        count = msgrcv(msgid, buffer, count, 1, 0);
        if (count == -1) {
#if 1
            fprintf(stderr, "Error: %s.\n", strerror(errno));
#endif
        }
    }

    void Write(const char * buffer, size_t count) {
        int rc = msgsnd(msgid, buffer, count, 0);
        if (rc == -1) {
#if 1
            fprintf(stderr, "Error: %s.\n", strerror(errno));
#endif
        }
    }

    static void Sleep() {
        struct timespec poll_sleep;
        poll_sleep = QUEUE_POLL_CONSUMER;
        nanosleep(&poll_sleep, NULL);
    }
};
