#include "CGraph.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DB_PRINT(fmt,args...) printf("\033[33m[%s]:"fmt"\033[0m\n",__FUNCTION__,##args)


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

BOOL CGraph::IsEmpty(void) const
{
    if(this->m_vexno==0)
        return TRUE;
    return FALSE;
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

