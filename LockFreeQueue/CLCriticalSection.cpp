#include "CLCriticalSection.h"
#include "PrintError.h"
#include "CLMutex.h"

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