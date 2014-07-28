#include <fstream>
#include <iostream>//test
#include <vector>
#include <string>

#include "SLConfigParser.h"
#include "PrintError.h"

#define START 0


#define SUBSTITUTE "SUBSTITUTE"
#define CHANGEINTO "CHANGEINTO"
#define TEXTTYPE "TEXTTYPE"

std::vector<std::string> g_Substitute;
std::string g_ChangeInto;
std::string g_TextType;

int SLConfigParser::operator()(void)
{
    std::fstream in(config_path);
    std::string buf;
    if(!in)
    {
        PrintError(AT , "open configfile");
        return -1;
    }
    while(getline(in , buf))
    {
        int delim = buf.find_first_of(":");
#ifdef START
        std::string metadata = buf.substr(START , delim);
#endif
        std::string data = buf.substr(delim+1 , std::string::npos);

        if(metadata == SUBSTITUTE)
            g_Substitute.push_back(data);
        else if(metadata == CHANGEINTO)
            g_ChangeInto = data;
        else if(metadata == TEXTTYPE)
            g_TextType = data;
        else
        {
            PrintError(AT , "Configuration meta-data is invalid");
            return -1;
        }
        buf.clear();
    }
    in.close();
//test
    for(auto ptr = g_Substitute.begin() ; ptr != g_Substitute.end() ; ++ptr)
        std::cout << *ptr << std::endl;
    std::cout << g_ChangeInto << std::endl;
    return 0;
}