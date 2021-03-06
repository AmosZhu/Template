#include "CGraph_Test.hpp"
#include <stdio.h>

int main(int argc,char* argv[])
{

    if(argc!=2)
    {
        printf("Usage: <func><filePath>\n");
        return -1;
    }

    //CGraph_WeightedTest(argv[1]);
    //CGraph_UnweightedTest(argv[1]);
    //CGraph_NeWeightedTest(argv[1]);
    //CGraph_MinSpanTree(argv[1]);
    CGraph_WeightedTestDP(argv[1]);
    CGraph_WeightedTestBF(argv[1]);
    CGraph_WeightedTestFW(argv[1]);
    return 0;
}
