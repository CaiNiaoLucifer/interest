#include "CLMutex.h"
#include "PrintError.h"

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