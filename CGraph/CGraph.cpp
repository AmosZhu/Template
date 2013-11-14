#include "CGraph.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


CGraph::CGraph(void)
{
    m_node=NULL;
    m_vexno=0;
}

CGraph::CGraph(const CGraph& object)
{
    int i=0;
    if(this == &object)
        return;

    if(!object.IsEmpty())
    {
        m_vexno=object.m_vexno;
        m_node=new gnode_t[m_vexno];
        for(i=0; i<m_vexno; i++)
        {
            memcpy(m_node[i].name,object.m_node[i].name,NAMESIZE);
            m_node[i].indegree=object.m_node[i].indegree;
            m_node[i].index=object.m_node[i].index;
            m_node[i].weight=object.m_node[i].weight;
            m_node[i].adjacent_node=object.m_node[i].adjacent_node;
        }
    }
    else
    {
        m_node=NULL;
        m_vexno=0;
    }
    return;
}

CGraph::~CGraph(void)
{
    Destroy();
}

void CGraph::Destroy(void)
{
    if(this->IsEmpty())
        return;

    AM_U32 index;
    for(index=0; index<this->m_vexno; index++)
    {
        this->m_node[index].adjacent_node.Destroy();
    }

    delete[] m_node;
    m_vexno=0;
}

Err_t CGraph::Create(char* path)
{
    if(path==NULL)
        return INVALIDE_PARAMET;
    FILE* fp;
    AM_U32 index=0;
    AM_U32 input=0;

    if((fp=fopen(path,"rb"))==NULL)
        return RESOURCE_UNAVAILABLE;

    /*******************************
    *
    *   Read number of vex first
    *
    *******************************/
    if(fscanf(fp,"%d",&m_vexno)<0)
        return FILEDATA_CORRUPT;

    if(m_vexno!=0)
    {
        m_node=new gnode_t[m_vexno];
    }

    memset(m_node,0,m_vexno*sizeof(gnode_t));

    for(; index<m_vexno; index++)
    {
        //fscanf(fp,"%s",m_node[index].name);
        fscanf(fp,"%d",&m_node[index].index);
        while(fscanf(fp,"%d",&input)&&input!=-1)
        {
            m_node[index].adjacent_node.Insert(input);
            m_node[input].indegree++;                   //caculate indegree
        }
    }

    fclose(fp);
    return RETURN_SUCCESS;
}

/**************************************************
*
*   Topological Sorting API
*
*   @result use to store the sort result
*
**************************************************/
Err_t CGraph::TopologicalSort(gnode_t * result)
{
    if(result==NULL)
        return INVALIDE_PARAMET;

    CGraph dup1=*this;  //Create a duplication, due to indegree will decrease, Don not change the original data

    CQueue<AM_U32> q1;
    int i;
    AM_U32 idx=0; //index for dup1;
    /************************************************
    * Search the node whose indegree was zero
    ************************************************/
    for(i=0; i<dup1.m_vexno; i++)
    {
        if(dup1.m_node[i].indegree==0)
            q1.Enqueue(dup1.m_node[i].index);
    }
    /***************************************************
    * Get the element from the queue,indegree decrease
    ***************************************************/
    i=0;
    while(!q1.IsEmpty())
    {
        q1.Dequeue(&result[i].index); //It will cause double free,because CList in the struct
        memcpy(&result[i],&this->m_node[result[i].index],sizeof(gnode_t));
        dup1.m_node[result[i].index].adjacent_node.ResetElemNext(); //Reset pointer before get next element
        while(dup1.m_node[result[i].index].adjacent_node.GetElemNext(&idx)==RETURN_SUCCESS)
        {
            dup1.m_node[idx].indegree--;
            if(dup1.m_node[idx].indegree==0)
                q1.Enqueue(dup1.m_node[idx].index);
        }
        i++;
    }
    if(i!=dup1.m_vexno)
    {
        std::cout<<"Graph has a cycle!"<<std::endl;
        return OPERATOR_FAILED;
    }

    return RETURN_SUCCESS;
}

void CGraph::PrintTopoSortResult(gnode_t* result,AM_U32 size)
{
    int i;
    if((result==NULL)||(size!=m_vexno))
    {
        std::cout<<"The node is invalid,please check!"<<std::endl;
        return;
    }
    for(i=0; i<m_vexno-1; i++)
    {
        printf("%d-->",result[i].index);
    }
    printf("%d\n",result[i].index);
    return;
}



/***********************************************************************
*
*       This is the table use to store the data of outweighted
*       shortest path.
*
*                   infinity=-1
*   --------------------------------------
*   |  v  | known | distance | preVertex |
*   --------------------------------------
*   |  v1 | false | infinity |  unkown   |
*   |  v2 | false |    1     |    v7     |
*   |  v3 | false | infinity |  unkown   |
*   |  v4 | false | infinity |  unkown   |
*   |  v5 | false | infinity |  unkown   |
*   |  .  |  ...  |  ...     |    ...    |
*   |  v7 |  true |    0     |     0     |  <---The start vertex
*   |  .  |  ...  |  ...     |    ...    |
*   |  .  |  ...  |  ...     |    ...    |
*   |  .  |  ...  |  ...     |    ...    |
*   |  .  |  ...  |  ...     |    ...    |
*   |  .  |  ...  |  ...     |    ...    |
*   --------------------------------------
*
*************************************************************************/

/*********************************************************************
*
*   w is the adjacent node of v;
*   @The table to store the result of the outweighted shortest path
*
**********************************************************************/
Err_t CGraph::OutWeightShortestPath(AM_U32 start,gtable_t* table)
{
    AM_U32 currDist=0;
    AM_U32 v;
    AM_U32 w;
    CQueue<AM_U32> q1;
    if((table==NULL)||(IsEmpty()))
        return OPERATOR_FAILED;

    table_init(table);

    table[start].distance=0;
    q1.Enqueue(start);
    while(!q1.IsEmpty())
    {
        q1.Dequeue(&v);
        if(table[v].distance!=currDist)
            currDist++;
        table[v].know=TRUE;
        m_node[v].adjacent_node.ResetElemNext();
        while(m_node[v].adjacent_node.GetElemNext(&w)==RETURN_SUCCESS)
        {
            if(table[w].distance==INFINITY)
            {
                table[w].distance=currDist+1;
                table[w].preVertex=v;
                q1.Enqueue(w);
            }
        }
    }
    return RETURN_SUCCESS;
}

/************************************************
*   This function used for unweighted path yet
************************************************/
void CGraph::PrintShortestPath(gtable_t* table,AM_U32 size)
{
    AM_U32 i=0;
    AM_U32 output[m_vexno];
    AM_U32 v;
    AM_S32 j;
    AM_U32 index;
    AM_U32 start;

    if((table==NULL)||(size!=m_vexno))
    {
        std::cout<<"Table is invalid,please check!"<<std::endl;
        return;
    }

    /******************************
    *   Search the start point
    ******************************/
    for(i=0; i<m_vexno; i++)
    {
        if(table[i].distance==0)
        {
            start=i;
            break;
        }
    }

    std::cout<<"+++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<std::right;
    std::cout<<"|"<<std::setw(7)<<"Vertex"<<std::setw(2)<<"|";
    std::cout<<std::setw(6)<<"know"<<std::setw(3)<<"|";
    std::cout<<std::setw(9)<<"Distance"<<std::setw(2)<<"|";
    std::cout<<std::setw(6)<<"Path"<<std::setw(3)<<"|"<<std::endl;
    std::cout<<"======================================="<<std::endl;
    for(i=0; i<m_vexno; i++)
    {
        std::cout<<std::right;
        std::cout<<"|"<<std::setw(4)<<"V"<<i<<std::setw(4)<<"|";
        std::cout<<std::setw(4)<<std::boolalpha<<table[i].know<<std::setw(5)<<"|";
        std::cout<<std::setw(6)<<table[i].distance<<std::setw(5)<<"|";
        if(table[i].preVertex==DISTANCESELF)
            std::cout<<std::setw(6)<<"Self"<<std::setw(3)<<"|"<<std::endl;
        else
            std::cout<<std::setw(4)<<"V"<<std::left<<std::setw(4)<<table[i].preVertex<<std::right<<std::setw(1)<<"|"<<std::endl;
    }
    std::cout<<"+++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"CAUTION: If the distance was -1, it present there no valid path from the start point to this point!"<<std::endl;
    std::cout<<std::endl;
    for(i=0; i<m_vexno; i++)
    {
        v=i;
        index=0;
        while((v=table[v].preVertex)!=DISTANCESELF)
        {
            output[index]=v;
            index++;
        }
        std::cout<<"The path from "<<start<<" to "<<i<<std::endl;
        for(j=index-1; j>=0; j--)
        {
            std::cout<<output[j]<<"-->";
        }
        std::cout<<i<<std::endl;
    }

}

BOOL CGraph::IsEmpty(void) const
{
    if(this->m_vexno==0)
        return TRUE;
    return FALSE;
}

AM_U32 CGraph::CountNo(void)
{
    return m_vexno;
}

void CGraph::PrintOut(void)
{
    if(this->IsEmpty())
    {
        std::cout<<"This is empty graphics!"<<std::endl;
        return;
    }

    AM_U32 index=0;
    for(; index<this->m_vexno; index++)
    {
        std::cout<<"Node "<<m_node[index].index<<": ";
        std::cout<<this->m_node[index].adjacent_node;
        std::cout<<"In Degree: "<<this->m_node[index].indegree<<std::endl;
        std::cout<<"Out Degree: ";
        std::cout<<std::endl<<std::endl;
    }

}

BOOL CGraph::check_resource(void)
{
    if(this->m_vexno==0&&this->m_node!=NULL)
    {
        std::cout<<"Resource ERROR! Caution, Stack Overflow!\n"<<std::endl;
        return FALSE;
    }

    return TRUE;
}

void CGraph::table_init(gtable_t* table)
{
    int i=0;
    for(i=0; i<m_vexno; i++)
    {
        table[i].know=FALSE;
        table[i].distance=INFINITY;
        table[i].preVertex=DISTANCESELF;
    }
}

CGraph& CGraph::operator=(const CGraph& object)
{
    int i=0;
    if(this == &object)
        return *this;

    if(!object.IsEmpty())
    {
        m_vexno=object.m_vexno;
        m_node=new gnode_t[m_vexno];
        for(i=0; i<m_vexno; i++)
        {
            memcpy(m_node[i].name,object.m_node[i].name,NAMESIZE);
            m_node[i].indegree=object.m_node[i].indegree;
            m_node[i].index=object.m_node[i].index;
            m_node[i].weight=object.m_node[i].weight;
            m_node[i].adjacent_node=object.m_node[i].adjacent_node;
        }
    }
    else
    {
        m_node=NULL;
        m_vexno=0;
    }
    return *this;
}

