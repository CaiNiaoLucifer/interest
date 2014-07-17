#ifndef WORDRYLE
#define WORDRYLE

#include <string>
#include <utility>

#include <boost/unordered_map.hpp>

class CLStrMatching;

using boost::unordered_map;

class CLWRuleProducer
{

public:

    CLWRuleProducer(std::string word);
    void init();
    int pBadCharRule(char , unsigned int);
    int pGoodSuffixRule();

    friend class CLStrMatching;

private:

    std::string m_InputWord;
    const int m_DefalutMoveStep;
    const unsigned int m_WordLengthOffset;//search-word's length -1
    const unsigned int m_GoodSuffixTail; //search-word's length -1
    unordered_map<unsigned int , int> m_GoodSuffixRule;

    int hasSubSuffixAhead(unsigned int);
    int doHasSubSuffixAhead(unsigned int);
    int isNotOnlySuffix(unsigned int);

};

#endif