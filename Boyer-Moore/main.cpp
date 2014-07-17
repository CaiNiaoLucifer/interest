#include "CLStrMatching.h"
#include "PrintError.h"

int main(int argc , char**argv)
{
    if(argc != 3)
    {
        PrintError(AT , "argc is invalid");
        exit(0);
    }
    CLStrMatching* temp = new CLStrMatching(argv[1] , argv[2]);
    temp->startMatch();
    delete temp;
    return 0;
}