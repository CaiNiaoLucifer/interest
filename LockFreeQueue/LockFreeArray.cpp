#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#include "PrintError.h"

#define CAS(ptr , oldvalue , newvalue) __sync_bool_compare_and_swap(ptr , oldvalue , newvalue)
#define FULL false
#define EMPTY false
#define DEFAULTSIZE 100

class CLMutex;
CLMutex* g_pMutex;

template<typename T , uint32_t arraysize = DEFAULTSIZE>
class CLArrayLockedFree
{
public:

    CLArrayLockedFree();
    bool push(T);
    T pop();

private:

    T m_Queue[arraysize];
    uint32_t m_CurrentWriteIndex;
    uint32_t m_CurrentReadIndex;
    uint32_t m_MaxReadIndex;

    inline uint32_t countToIndex(uint32_t);
};

template<typename T , uint32_t arraysize>
CLArrayLockedFree<T , arraysize>::CLArrayLockedFree()
{
    m_CurrentWriteIndex = 0;
    m_CurrentReadIndex = 0;
    m_MaxReadIndex = 0;
}


template<typename T , uint32_t arraysize>
inline uint32_t
CLArrayLockedFree<T , arraysize>::countToIndex(uint32_t count)
{
    return (count%arraysize);
}

template<typename T , uint32_t arraysize>
bool
CLArrayLockedFree<T , arraysize>::push(T element)
{
    uint32_t CurrentWriteIndex;
    uint32_t CurrentReadIndex;

    do
    {
        CurrentReadIndex = m_CurrentReadIndex;
        CurrentWriteIndex = m_CurrentWriteIndex;


        if(countToIndex(CurrentWriteIndex + 1) == countToIndex(CurrentReadIndex))
            return FULL;

        if(!CAS(&m_CurrentWriteIndex , CurrentWriteIndex , CurrentWriteIndex + 1))
            continue;

        m_Queue[countToIndex(CurrentWriteIndex)] = element;
        break;

    }while(1);

    while(!CAS(&m_MaxReadIndex , CurrentWriteIndex , CurrentWriteIndex + 1))
    {
        sched_yield();
    }

    return true;
}

template<typename T , uint32_t arraysize>
T
CLArrayLockedFree<T , arraysize>::pop()
{
    uint32_t CurrentReadIndex;
    uint32_t result;

    do
    {
        CurrentReadIndex = m_CurrentReadIndex;

        if(countToIndex(CurrentReadIndex) == countToIndex(m_MaxReadIndex))
            return EMPTY;

        result = m_Queue[countToIndex(CurrentReadIndex)];

        if(!CAS(&m_CurrentReadIndex , CurrentReadIndex , CurrentReadIndex + 1))
            continue;

        return result;

    }while(1);
}

CLArrayLockedFree<int , DEFAULTSIZE>* g_pQueue;


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

CLMutex::CLMutex()
{
    m_pMutex = new pthread_mutex_t;
    int rt = pthread_mutex_init(m_pMutex , 0);

    if(rt != 0)
    {
        delete m_pMutex;
        PrintError(AT , "pthread_mutex_init wrong");
    }
}

CLMutex::~CLMutex()
{
    int rt = pthread_mutex_destroy(m_pMutex);

    if(rt != 0)
    {
        delete m_pMutex;
        PrintError(AT , "pthread_mutex_destory wrong");
    }
}

void CLMutex::Lock()
{
    int rt = pthread_mutex_lock(m_pMutex);

    if(rt != 0)
    {
        PrintError(AT , "pthread_mutex_lock wrong");
    }
}

void CLMutex::UnLock()
{
    int rt = pthread_mutex_unlock(m_pMutex);

    if(rt != 0)
    {
        PrintError(AT , "pthread_mutex_unlock wrong");
    }

}

pthread_mutex_t* CLMutex::getMutexInterface()
{
    return m_pMutex;
}


class CLCriticalSection
{
public:

    CLCriticalSection(CLMutex*);

    ~CLCriticalSection();

private:

    CLMutex* m_pMutex;

};

CLCriticalSection::CLCriticalSection(CLMutex* rhs)
{
    if(rhs == 0)
        PrintError(AT , "CLCriticalSection wrong");

    m_pMutex = rhs;
    m_pMutex->Lock();
}

CLCriticalSection::~CLCriticalSection()
{
    m_pMutex->UnLock();
}

class CLEventHandler
{
public:

    virtual void doAction() = 0;

};

class CLThread
{
public:

    CLThread(CLEventHandler*);

    void run();

    pthread_t getThreadId();

private:

    CLEventHandler* m_peventhandler;

    pthread_t m_ThreadId;

private:

    static void* startRunFunction(void*);
};

CLThread::CLThread(CLEventHandler* rhs):m_peventhandler(rhs)
{
}

void CLThread::run()
{
    pthread_create(&m_ThreadId , NULL , &startRunFunction , this);
}

pthread_t CLThread::getThreadId()
{
    return m_ThreadId;
}

void* CLThread::startRunFunction(void* rhs)
{
    (static_cast<CLThread*>(rhs))->m_peventhandler->doAction();

    return NULL;
}

class CLProducer:public CLEventHandler
{
public:

    CLProducer(int);

    void doAction();

private:

    int m_data;

};

CLProducer::CLProducer(int data):m_data(data)
{
}


void CLProducer::doAction()
{
    g_pQueue->push(this->m_data);
}


class CLConsumer:public CLEventHandler
{
public:

    void doAction();
};


void CLConsumer::doAction()
{
    int result = g_pQueue->pop();

    CLCriticalSection xx(g_pMutex);
    std::cout << result << std::endl;
}


int main(int argc , char** argv)
{
    if(argc != 3)
        PrintError(AT , "the number of argument is not valid");

    g_pQueue = new CLArrayLockedFree<int>;
    g_pMutex = new CLMutex;

    CLThread* proarray[atoi(argv[1])];
    CLThread* conarray[atoi(argv[2])];

    for(int i = 0 ; i != atoi(argv[1]) ; ++i)
    {
        proarray[i] = new CLThread(new CLProducer(i));
        proarray[i]->run();
    }

    for(int i = 0 ; i != atoi(argv[1]) ; ++i)
    {
        pthread_join(proarray[i]->getThreadId() , NULL);
        delete proarray[i];
    }

    for(int i = 0 ; i != atoi(argv[2]) ; ++i)
    {
        conarray[i] = new CLThread(new CLConsumer);
        conarray[i]->run();
    }

    for(int i = 0 ; i != atoi(argv[2]) ; ++i)
    {
        pthread_join(conarray[i]->getThreadId() , NULL);
        delete conarray[i];
    }

    delete g_pMutex;
    delete g_pQueue;

    return 0;
}
