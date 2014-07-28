#ifndef SLICEMAPPER_H
#define SLICEMAPPER_H

#include <map>
#include <string>
#include <set>

#include <boost/tr1/functional.hpp>

#include "SLConfigParser.h"

typedef boost::function<int (void)> ConfigParser;

class CLSliceMapper
{
public:

    CLSliceMapper(const std::string , const std::string);

public:
    int makePairs();

private:

    ConfigParser m_MyParser;
    std::string m_SlicePath;
    std::set<std::string> m_TmpResult;
    std::multiset<std::string> m_Result;

    int readConfig();
    int dealWithText();
    void init(void);
    int splitIntoPairs(void);
    int doSplitIntoPairs(std::string&);
    int eliminateTillWords(std::string&);
    void printResult(const std::string&);
    int reduce(void);
    CLSliceMapper(const CLSliceMapper&);
    const CLSliceMapper& operator=(const CLSliceMapper&);

};

#endif