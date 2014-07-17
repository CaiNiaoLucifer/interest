#ifndef CLCRITICALSECTION_H
#define CLCRITICALSECTION_H

#include "CLMutex.h"

class CLCriticalSection
{
public:

    CLCriticalSection(CLMutex*);

    ~CLCriticalSection();

private:

    CLMutex* m_pMutex;

};

#endif