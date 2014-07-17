#include "PrintError.h"

void PrintError(const char* location,const char* errormessage)
{
    fprintf(stdout,"Error at %s ; Error Discription:%s\n",location , errormessage);
}