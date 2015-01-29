#ifndef _CGRAPH_HPP
#define _CGRAPH_HPP

/***********************************************************
*
*   Author:Amos.zhu
*   @This template used for graphics
*   @Amos used adjacency list to store the graphics data
*
************************************************************/

#include "AmosType.hpp"
#include "CList.hpp"
#include "CQueue.hpp"
#include "CBinaryHeap.hpp"

#define NAMESIZE 100
#define DISTANCESELF -1000

/************************
*   Graph type
************************/
#define UNWEIGHTED 1
#define WEIGHTED 2

typedef struct GRAPH_VERTEX_TYPE
{
    AM_U32 index;
    AM_S32 weight;
} gvertex_t;

typedef struct GRAPH_NODE_TYPE
{
    AM_U8 name[NAMESIZE];
    AM_U32 index;
    AM_U32 indegree;
    CList<gvertex_t> adjacent_node;
} gnode_t;

typedef struct GRAPH_TABLE_TYPE
{
    BOOL know;
    AM_S32 distance;
    AM_S32 preVertex;   //Use to trace the route;
} gtable_t;

typedef struct HEAP_NODE_TYPE
{
    AM_U32 u;
    AM_U32 v;
    AM_S32 weight;
} hNode_t;

class CGraph
{
public:
    CGraph(void);
    CGraph(const CGraph& object);
    ~CGraph(void);
    void Destroy(void);

    /*******************************************
    *
    *   Get data from file
    *   @Input: path name
    *
    *******************************************/
    BOOL IsEmpty(void) const;
    Err_t Create(char* path);
    /********************************************
    *   A pair of function for topological sort,
    *   And the result can be print out by the
    *   print function.
    ******************************************/
    Err_t TopologicalSort(gnode_t* result);
    void PrintTopoSortResult(gnode_t* result,AM_U32 size);
    /*******************************************
    *   Pair function
    *******************************************/
    void PrintOut(void);
    /*********************************************
    *   Return the number of the vertex
    *********************************************/
    AM_U32 CountNo(void);
    /*****************************************************
    * This pair of function get unweighted & weighted
    * shortest path,And the result can be print out by the
    * print function.
    ******************************************************/
    Err_t UnWeightShortestPath(AM_U32 start,gtable_t* table);
    Err_t WeightShortestPath(AM_U32 start,gtable_t* table);
    Err_t NegativeWeightShortestPath(AM_U32 start,gtable_t* table);
    Err_t MinSpanTree(gtable_t* table);
    void PrintShortestPath(gtable_t* table,AM_U32 size);
public:
    CGraph& operator=(const CGraph& object);


private:
    gnode_t* m_node=nullptr;
    AM_U32 m_vexno;
    CBinaryHeap<hNode_t> m_binHeap;

private:
    void table_init(gtable_t* table);
    BOOL check_resource(void);
};

#endif
