#include <iostream>

#include "CLWRuleProducer.h"
#include "PrintError.h"

CLWRuleProducer::CLWRuleProducer(std::string rhs):m_InputWord(rhs) , m_DefalutMoveStep(-1)
 , m_WordLengthOffset(rhs.length() - 1) , m_GoodSuffixTail(m_WordLengthOffset)
{}

void CLWRuleProducer::init()
{
    pGoodSuffixRule();
}


/*
 * @Function Name : pBadCharRule
 *
 * @Description : This function use bad char and the current search-word offset
 *                to produce the step the search- word need to move forward and
 *                continue to match
 *
 * @para1 : the bad char in the context which don't match the search-word
 *
 * @para2 : the current offset of the search-word
 *
 * @return : the step which search-word need to move forward
 *
 */
int CLWRuleProducer::pBadCharRule(char const badchar , unsigned int currentoffset)
{
    for(int ptr = currentoffset - 1 ; ptr >=0 ; --ptr)
    {
        if(m_InputWord[ptr] == badchar)
            return (currentoffset - ptr);
    }

    return (currentoffset - m_DefalutMoveStep);
}
/*
 * @Function Name : pGoodSuffixRule
 *
 * @Description : To produce complete Good Suffix Rule table
 *
 * @para : none
 *
 * @return : just a flag which stands for success , no meaningful
 *
 */
int CLWRuleProducer::pGoodSuffixRule()
{
    int tmp = -1;
    int tmp1 = -1;
    for(unsigned int ptr =0 ; ptr != m_WordLengthOffset + 1; ++ptr)
    {
        tmp = isNotOnlySuffix(ptr);
        if(tmp == -1)
        {
            tmp1 = hasSubSuffixAhead(ptr);

            m_GoodSuffixRule[ptr] = m_GoodSuffixTail - tmp1;
        }
        else
            m_GoodSuffixRule[ptr] = m_GoodSuffixTail - tmp;
    }
    return 0;
}

/*
 * @Function Name : hasSubSuffixAhead
 *
 * @Description : look for if there is a sub-goodsuffix of good suffix in the
 *                beginning of the search-word
 *
 * @para1 : the beginning offset of good suffix
 *
 * @return : if there is , return the the sub-goodsuffix's last character's offset
 * in the search-word or return -1
 *
 */
int CLWRuleProducer::hasSubSuffixAhead(unsigned int suffixbegin)
{
    if(suffixbegin > m_WordLengthOffset)
    {
        PrintError(AT , "suffixbegin is too big in hasSubSuffixAhead function");
        exit(0);
    }

    unsigned int ptr = m_WordLengthOffset;
    int result = m_DefalutMoveStep;

    while(ptr > suffixbegin)
    {
        result = doHasSubSuffixAhead(ptr);
        if(result == -1)
            --ptr;
        else
            break;
    }
    return result;
}

int CLWRuleProducer::doHasSubSuffixAhead(unsigned int offset)//the excute function
    //for hasSubSuffixAhead function

{
    std::string search = m_InputWord.substr(offset , std::string::npos);
    int tmp = static_cast<int>(m_InputWord.find(search));
    if(tmp == 0)
        return (search.length() - 1);
    else
        return -1;
}

int CLWRuleProducer::isNotOnlySuffix(unsigned int suffixbegin)
{
    if(suffixbegin > m_WordLengthOffset)
    {
        PrintError(AT , "suffixbegin is invalid");
        exit(0);
    }
    std::string search = m_InputWord.substr(suffixbegin , std::string::npos);
    int start = -1;
    if(suffixbegin != 0)
        start = static_cast<int>(m_InputWord.rfind(search , m_GoodSuffixTail - search.length()));
    if(start == -1)
        return  -1;
    else
        return (start + search.length() - 1);
}
