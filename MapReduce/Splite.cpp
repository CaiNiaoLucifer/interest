#include <fstream>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>

#include <boost/shared_array.hpp>

#include "PrintError.h"

#define SLICEMAX (0x01000000)
#define PRESIZE 20

int GetOffset(const int& fd , const unsigned int& average , unsigned int& start)
{
    if(lseek(fd,average-1,SEEK_SET) != -1)
    {
        char temp[20];
        if(-1 != read(fd,&temp,PRESIZE))
        {
            int cal = 0;
            for(char i:temp)
            {
                if(!((i >= 0x41 && i <= 0x5A)||(i >= 0x61 && i<= 0x7A)))
                    break;
                else
                    ++cal;
            }
            start += (cal + average);
        }
        else
        {
            PrintError(AT,"read for temp");
            throw std::string("read for temp");
        }
    }
    else
    {
        PrintError(AT,"lseek in GetOffset");
        throw std::string("lseek in GetOffset");
    }
    return 0;
}

int WriteFile(int source_fd , const unsigned int& start , int num , char* outfile_name)
{
    boost::shared_array<char> buf;
    try
    {
        boost::shared_array<char> buf1(new char[num+1]);
        buf = buf1;
    }
    catch(std::bad_alloc& s)
    {
        PrintError(AT , "bad allocate");
        std::abort();
    }
    buf[num] = '\0';
    if(-1 != lseek(source_fd , start , SEEK_SET))
    {
        if(-1 != read(source_fd , buf.get() , num))
        {
            int des_fd = open(outfile_name , O_RDWR|O_CREAT|O_EXCL , S_IRUSR|S_IWUSR);
            if(des_fd != -1)
            {
                if(-1 != write(des_fd,buf.get(),strlen(buf.get())))
                {
                    if(-1 == close(des_fd))
                    {
                        PrintError(AT , "close in WriteFile");
                        throw std::string("close in WriteFile");
                    }
                }
                else
                {
                    PrintError(AT , "write in WriteFile");
                    throw std::string("write in WriteFile");
                }
            }
            else
            {
                PrintError(AT , "open in WriteFile");
                throw std::string("open in WriteFile");
            }
        }
        else
        {
            PrintError(AT , "read in WriteFile");
            throw std::string("read in WriteFile");
        }
    }
    else
    {
        PrintError(AT , "lseek in WriteFile");
        throw std::string("lseek in WriteFile");
    }
    return 0;
}

int main(int argc , char** argv)
{
    if(2 != argc)
    {
        PrintError(AT , "number of argument is wrong");
        exit(-1);
    }
    int filefd = open(argv[1],O_RDWR);
    if(-1 == filefd)
    {
        PrintError(AT,"open");
        exit(-1);
    }

    struct stat file_state;
    fstat(filefd,&file_state);

    unsigned int  size = file_state.st_size;

    unsigned int size0_start = 0;
    unsigned int size1_start = 0;
    unsigned int size2_start = 0;
    unsigned int average_size = size/3+1;

    if(((average_size ^ SLICEMAX) >> (6*4)) == 0x01)
    {
        try
        {
            GetOffset(filefd , average_size , size1_start);
            GetOffset(filefd , average_size*2 , size2_start);
        }
        catch(std::string& s)
        {
            throw;
        }
    }
    else
    {
        PrintError(AT,"slice is too big");
        exit(-1);
    }
    try
    {
        WriteFile(filefd , size0_start , size1_start-size0_start , "Slice0");
        WriteFile(filefd , size1_start , size2_start-size1_start , "Slice1");
        WriteFile(filefd , size2_start , size-size2_start , "Slice2");
    }
    catch(std::string& s)
    {
        close(filefd);
        std::abort();
    }

    close(filefd);

    return 0;
}