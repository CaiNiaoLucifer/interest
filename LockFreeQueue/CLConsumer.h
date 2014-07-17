#ifndef CLCONSUMER_H
#define CLCONSUMER_H

class CLConsumer:public CLEventHandler
{
public:

    CLConsumer();
    void doAction();

private:

    static CLMutex* m_pMutex;
};

#endif