#pragma once

#include <pthread.h>
#include <queue>
#include <iostream>
#include "Message.h"

using namespace std;

struct msgEvent
{
    unsigned int id_;
    Message* msg_;
};


class MsgQueue
{
public:
    MsgQueue(unsigned long maxSize);
    void send(unsigned long id, Message* msg = NULL);
    Message* receive(unsigned long& id);
    ~MsgQueue();
private:
    unsigned long maxSize_;
    queue<msgEvent> container;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t txCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t rxCond = PTHREAD_COND_INITIALIZER;
};
