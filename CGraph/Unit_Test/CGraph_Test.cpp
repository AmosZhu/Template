#include "CGraph.hpp"
#include "CGraph_Test.hpp"

#define FILEPATH "data.txt"
#define UNWEIGHED_PATH "unweight.txt"
#define WEIGHTED_PATH "weight.txt"

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

void CGraph_UnweightedTest(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
    number=graph1.CountNo();
    gtable_t table[number];
    graph1.UnWeightShortestPath(2,table);
    graph1.PrintShortestPath(table,number);
}

void CGraph_WeightedTest(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
    number=graph1.CountNo();
    gtable_t table[number];
    graph1.WeightShortestPath(0,table);
    graph1.PrintShortestPath(table,number);

}

void CGraph_WeightedTestDP(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    //graph1.PrintOut();
    //number=graph1.CountNo();
    graph1.WeightShortestPathDP(0);
}

void CGraph_WeightedTestBF(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    //graph1.PrintOut();
    //number=graph1.CountNo();
    graph1.WeightShortestPathBellmanFord(1);
}

void CGraph_WeightedTestFW(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    //graph1.PrintOut();
    //number=graph1.CountNo();
    graph1.WeightShortestPathFloydWarshall();
}


void CGraph_NeWeightedTest(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
    number=graph1.CountNo();
    gtable_t table[number];
    graph1.NegativeWeightShortestPath(0,table);
    graph1.PrintShortestPath(table,number);

}

void CGraph_MinSpanTree(char* path)
{
    CGraph graph1;
    int number;

    if(graph1.Create(path)!=RETURN_SUCCESS)
        printf("Test Failed!\n");

    graph1.PrintOut();
    number=graph1.CountNo();
    gtable_t table[number];
    graph1.MinSpanTree(table);
    graph1.PrintShortestPath(table,number);
}
