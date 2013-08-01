#include "CGraph.hpp"
#include "CGraph_Test.hpp"

#define FILEPATH "data.txt"


void CGraph_CreateTest(void)
{
    CGraph graph1;
    if(graph1.Create((char*)FILEPATH)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
}
