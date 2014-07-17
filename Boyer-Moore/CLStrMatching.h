#ifndef STRMATCHING_H
#define STRMATCHING_H

#include <string>

#include "CLWRuleProducer.h"

#define MAX(x , y) ((x>y)?(x):(y))

class CLStrMatching
{

public:

    CLStrMatching(std::string , std::string);
    ~CLStrMatching();
    int readFile();
    int startMatch();

private:

    std::string m_Context;
    std::string m_SearchStr;
    const std::string m_FilePath;
    const unsigned int m_SeaStrTailOffset;
    unsigned int m_StrCurOffset;//for search word
    unsigned int m_ContextCurOffset;
    const unsigned int m_SeaStrHeadOffset;

    CLWRuleProducer* m_pWRuleProducer;
};

#endif