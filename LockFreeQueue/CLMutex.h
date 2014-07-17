#ifndef CLMUTEX_H
#define CLMUTEX_H

#include <pthread.h>

class CLMutex
{
public:

    CLMutex();
    ~CLMutex();
    void Lock();
    void UnLock();
    pthread_mutex_t* getMutexInterface();

private:

    pthread_mutex_t* m_pMutex;
};

#endif