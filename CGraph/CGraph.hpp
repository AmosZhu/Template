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

#define NAMESIZE 100

typedef struct GRAPH_NODE_TYPE
{
    AM_U8 name[NAMESIZE];
    AM_U32 index;
    AM_U32 weight;
    CList<AM_U32> adjacent_node;
} gnode_t;

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
    BOOL IsEmpty(void);
    Err_t Create(char* path);
    void PrintOut(void);

private:
    gnode_t* m_node;
    AM_U32 m_vexno;


private:
    BOOL check_resource(void);
};

#endif
