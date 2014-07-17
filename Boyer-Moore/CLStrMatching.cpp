#include <fstream>
#include <iostream>

#include "CLStrMatching.h"
#include "PrintError.h"

CLStrMatching::CLStrMatching(std::string search , std::string filepath):m_SearchStr(search)
, m_FilePath(filepath) , m_SeaStrTailOffset(search.length() - 1) ,
m_StrCurOffset(m_SeaStrTailOffset) ,m_ContextCurOffset(m_StrCurOffset)
, m_SeaStrHeadOffset(0) , m_pWRuleProducer(new CLWRuleProducer(m_SearchStr))
{

}

CLStrMatching::~CLStrMatching()
{
    if(m_pWRuleProducer != 0)
        delete m_pWRuleProducer;
}


int CLStrMatching::readFile()
{
    std::fstream filein;
    filein.open(m_FilePath.c_str());
    if(!filein.is_open())
    {
        PrintError(AT , "open file failed");
        filein.clear();
        exit(0);
    }
    std::getline(filein , m_Context);
    filein.close();
    return 0;
}

int CLStrMatching::startMatch()
{
    readFile();
    m_pWRuleProducer->init();

    int initial = -1;//just initialize initial var with an impossible value

    bool flag = true;//flag stands for if we have find an offset in context for
    //a given string or not . True:have found one , False:not have found

    while(m_ContextCurOffset <= m_Context.length() - 1)
    {
        initial = m_ContextCurOffset;
        while(m_StrCurOffset >= m_SeaStrHeadOffset)
        {
            if(m_SearchStr[m_StrCurOffset] == m_Context[m_ContextCurOffset])
            {
                if(m_StrCurOffset == 0)
                    break;
                --m_StrCurOffset;
                --m_ContextCurOffset;
            }
            else
            {
                int tmp = m_pWRuleProducer->pBadCharRule(m_Context[m_ContextCurOffset]
                                            , m_StrCurOffset);
                int tmp1 = -1;

                if(m_StrCurOffset < m_SeaStrTailOffset)//m_StrCurOffset + 1 is
                    //start of Good Suffix , so if m_StrCurOffset is equal to
                    //m_SeaStrTailOffset, it means we can only use bad char rule

                    tmp1 = (m_pWRuleProducer->m_GoodSuffixRule)[m_StrCurOffset + 1];
                int step = MAX(tmp , tmp1);
                m_ContextCurOffset = initial + step;
                m_StrCurOffset = m_SeaStrTailOffset;
                flag = false;
                break;
            }
        }
        if(flag)
        {
            std::cout << (m_ContextCurOffset + 1) << std::endl; //offset begins
            //with zero , bur offset in result needs to begin with 1 , so every
            //offset of result add by 1

            m_ContextCurOffset = (initial +
                        (m_pWRuleProducer->m_GoodSuffixRule)[m_StrCurOffset]);
            m_StrCurOffset = m_SeaStrTailOffset;
        }

        flag = true;
    }
    return 0;
}