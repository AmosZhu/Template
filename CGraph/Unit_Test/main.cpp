#include "CGraph_Test.hpp"
#include <stdio.h>

int main(int argc,char* argv[])
{

    if(argc!=2)
    {
        printf("Usage: <func><filePath>\n");
        return -1;
    }

    CGraph_WeightedTest(argv[1]);
    //CGraph_UnweightedTest(argv[1]);
    return 0;
}
