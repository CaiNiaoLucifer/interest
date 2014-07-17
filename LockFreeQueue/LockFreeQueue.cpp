#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#include "PrintError.h"

class CLMutex;

CLMutex* g_pMutex;

#define CAS(ptr , oldvalue , newvalue) __sync_bool_compare_and_swap(ptr , oldvalue , newvalue)

template<typename T>
struct Node
{
    T* data;
    Node<T>* next;
};

template<typename T>
struct Queue
{
    Node<T>* head;
    Node<T>* tail;
};

template<typename T>
Queue<T>*
queueNew(void)
{
    Node<T>* tmp = new Node<T>;
    Queue<T>* queue = new Queue<T>;
    queue->head = queue->tail = tmp;
    return queue;
}

Queue<int>* myqueue;

template<typename T>
void
queuePush(Queue<T>* queue , T* data)
{
    Node<T>* onenode= new Node<T>;
    onenode->data = data;
    onenode->next = NULL;
    Node<T>* tail;
    Node<T>* next;

    while(true)
    {
        tail = queue->tail;
        next = tail->next;

        if(tail != queue->tail)
            continue;
        if(next != NULL)
        {
            CAS(&queue->tail , tail , next);
            continue;
        }
        if(CAS(&tail->next , next , onenode))
            break;
    }
    CAS(&queue->tail , tail , onenode);
}

template<typename T>
T*
queuePop(Queue<T>* queue)
{
    Node<T>* head;
    Node<T>* tail;
    Node<T>* next;
    T* data = NULL;
    while(true)
    {
        head = queue->head;
        tail = queue->tail;
        next = head->next;

        if(head != queue->head)
            continue;
        if(next == NULL)
            return NULL;
        if(head == tail)//prevent head exceed tail
        {
            CAS(&queue->tail , tail , next);
            continue;
        }
        data = next->data;
        if(CAS(&queue->head , head , next))
            break;
    }
    delete head;
    return data;
}

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

    //CLCriticalSection(){}
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
    queuePush(myqueue , &this->m_data);
}


class CLConsumer:public CLEventHandler
{
public:

    void doAction();
};


void CLConsumer::doAction()
{
    int* result = queuePop(myqueue);

    CLCriticalSection xx(g_pMutex);
    std::cout << *result << std::endl;
}

int main(int argc , char** argv)
{
    if(argc != 3)
        PrintError(AT , "the number of argument is not valid");

    myqueue = queueNew<int>();
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
    delete myqueue;

    return 0;
}
