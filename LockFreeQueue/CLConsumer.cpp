#include <iostream>

#include "CLConsumer.h"

CLMutex* CLConsumer::m_pMutex = new CLMutex;

void CLConsumer::doAction()
{
    int* result = queuePop(myqueue);
    CLCriticalSection(m_pMutex);
    std::cout << *result << std::endl;
}