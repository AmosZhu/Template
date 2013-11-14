#include "CGraph.hpp"
#include "CGraph_Test.hpp"

#define FILEPATH "data.txt"
#define UNWEIGHED "unweight.txt"

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
    graph1.PrintTopoSortResult(q1,9);
}

void CGraph_UnweightedTest(void)
{
    CGraph graph1;
    int number;

    if(graph1.Create((char*)UNWEIGHED)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
    number=graph1.CountNo();
    gtable_t table[number];
    graph1.OutWeightShortestPath(5,table);
    graph1.PrintShortestPath(table,number);
}
