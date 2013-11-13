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

void CGraph_TopoSortTest(void)
{
    CGraph graph1;
    int i;
    if(graph1.Create((char*)FILEPATH)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    gnode_t q1[9];
    graph1.TopologicalSort(q1);
    for(i=0; i<8; i++)
    {
        printf("%d-->",q1[i].index);
    }
    printf("%d\n",q1[i].index);
}
