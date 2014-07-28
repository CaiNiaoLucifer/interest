#include <fstream>
#include <vector>
#include <string>

#include <boost/unordered_map.hpp>
#include <boost/functional.hpp>
#include <boost/lambda/lambda.hpp>

#include "CLSliceMapper.h"
#include "SLConfigParser.h"
#include "PrintError.h"

#define SLICE_STANDARD_SIZE 0x1000
#define LONGESTLINELENGTH 1000
#define START 0

extern std::vector<std::string> g_Substitute;
extern std::string g_Changeinto;
extern std::string g_TextType;

boost::unordered_map<std::string , std::string> g_NameToPuc;

CLSliceMapper::CLSliceMapper(const std::string cfgfilepath
                            , const std::string slicepath)
:m_MyParser(SLConfigParser(cfgfilepath)) , m_SlicePath(slicepath)
{
    init();
}

int CLSliceMapper::makePairs()
{
    if(readConfig() == 0)
        if(dealWithText() == 0)
            if(splitIntoPairs() == 0)
                if(reduce() == 0)
                    return 0;
    return -1;

}

int CLSliceMapper::readConfig()
{
    if(m_MyParser() != 0)
    {
        PrintError(AT , "readConfig");
        return -1;
    }
    return 0;
}

int CLSliceMapper::dealWithText()
{
    std::string buf;
    std::fstream filein(m_SlicePath);
    std::fstream fileout;
    fileout.open(m_SlicePath + "_noblank" , std::fstream::out|std::fstream::app);
    m_SlicePath += "_noblank";
    if(filein)
    {
        while(getline(filein , buf))
        {
            for(auto ptr = g_Substitute.begin()
                ; ptr != g_Substitute.end() ; ++ptr)
            {
                int pos = 0;
                while(pos != static_cast<int>(std::string::npos))
                {
                    pos = static_cast<int>(buf.find_first_of(g_NameToPuc[*ptr]));
                    if(pos != -1)
                        buf.replace(pos , 1 , " ");
                }
            }
            if(fileout)
            {
                fileout << buf;
                buf.clear();
            }
            else
            {
                PrintError(AT , "out open wrong");
                return -1;
            }
        }
    }
    else
    {
        PrintError(AT , "fstream in is invalid");
        return -1;
    }
    filein.close();
    fileout.close();
    return 0;
}

int CLSliceMapper::splitIntoPairs(void)
{
    std::fstream in(m_SlicePath.c_str());
    std::string buf;
    if(!in)
    {
        PrintError(AT , "open file with no blank");
        return -1;
    }
    while(std::getline(in , buf))
    {
        doSplitIntoPairs(buf);
        buf.clear();
    }
    return 0;
}

int CLSliceMapper::doSplitIntoPairs(std::string& line) //[  )
{
    int cal = 0;
    int start = 0;
    int pos = 0;
    std::string words;
    int length = line.length();
    while(cal != length - 1)
    {
        start = eliminateTillWords(line);
        try
        {
            line = line.substr(start , std::string::npos);
        }
        catch(std::out_of_range)
        {
            PrintError(AT , "substr out_of_range");
            return -1;
        }
        pos = line.find_first_of(" ");

        if(pos == static_cast<int>(std::string::npos))
        {
            words = line.substr(START , std::string::npos);
            m_TmpResult.insert(words);
            m_Result.insert(words);
            break;
        }
        else
            words = line.substr(START , pos);

        m_TmpResult.insert(words);
        m_Result.insert(words);
        line = line.substr(pos , std::string::npos);
        cal += (pos + start);
    }
    return 0;
}

int CLSliceMapper::eliminateTillWords(std::string& line)
{
    int start = 0;
    for(unsigned int i =0 ; i != line.length() ; ++i)
    {
        if( ' ' != line[i] )
            break;
        else
            ++start;
    }
    return start;
}

void CLSliceMapper::init(void)
{
    g_NameToPuc["blank"] = " ";
    g_NameToPuc["comma"] = ",";
    g_NameToPuc["dot"]   = ".";
    g_NameToPuc["question"] = "?";
    g_NameToPuc["connector"] = "-";
}

void CLSliceMapper::printResult(const std::string& tmp)
{
    std::cout << "<" << tmp << "," << m_Result.count(tmp) << ">" << std::endl;
}

int CLSliceMapper::reduce(void)
{
    for(auto ptr = m_TmpResult.begin() ; ptr != m_TmpResult.end() ; ++ptr)
        printResult(*ptr);
    return 0;
}

