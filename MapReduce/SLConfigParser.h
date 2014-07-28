#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <vector>
#include <string>

class CLSliceMapper;

struct SLConfigParser
{
    SLConfigParser()
    {}

    SLConfigParser(const std::string filepath):config_path(filepath)
    {}

    int operator()(void);

private:
    const std::string config_path;
};

#endif