#include "CGraph.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "CDisjointSet.hpp"


static void copyFunc(gvertex_t* dst,gvertex_t* src)
{
    memcpy(dst,src,sizeof(gvertex_t));
}


static void BinCopyFunc(hNode_t* dst,hNode_t* src)
{
    memcpy(dst,src,sizeof(hNode_t));
}

static cmp_t BinCmpFunc(hNode_t* arg1,hNode_t* arg2)
{
    if((arg1==NULL)||(arg2==NULL))
        return INVALID;

    if(arg1->weight==arg2->weight)
        return EQUAL;
    else if(arg1->weight>arg2->weight)
        return LARGER;
    else if(arg1->weight<arg2->weight)
        return SMALLER;
}

static BOOL BinIsDummyFunc(hNode_t *src)
{
    if(src->u==-1&&src->v==-1)
        return TRUE;

    return FALSE;
}

static void BinDummyFunc(hNode_t* src)
{
    src->u=-1;
    src->v=-1;
}

static cmp_t SCmpFunc(AM_U32* arg1,AM_U32* arg2)
{
    if((arg1==NULL)||(arg2==NULL))
        return INVALID;

    if(*arg1==-1||*arg2==-1)
        return INVALID;

    if(*arg1==*arg2)
        return EQUAL;
    else if(*arg1>*arg2)
        return LARGER;
    else if(*arg1<*arg2)
        return SMALLER;
}

static void SCopyFunc(AM_U32* dst,AM_U32* src)
{
    memcpy(dst,src,sizeof(AM_U32));
}

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
    m_node=nullptr;

#if 0
    if(m_binHeap!=nullptr)
    {
        delete m_binHeap;
        m_binHeap=nullptr;
    }
#endif /* Modify by Amos.zhu */

    m_vexno=0;
}

Err_t CGraph::Create(char* path)
{
    if(path==NULL)
        return INVALIDE_PARAMET;
    FILE* fp;
    AM_U32 index=0;
    AM_U32 input=0;
    char buf[MAXLINE];
    char* ret;
    AM_U32 type;
    gvertex_t temp;
    hNode_t hNode;
    AM_U32 noOfeges=0;
    gvertex_t v;

    if((fp=fopen(path,"rb"))==NULL)
        return RESOURCE_UNAVAILABLE;

    /*******************************
    *   Read the vertex type
    *******************************/
    if(fscanf(fp,"%s",buf)<0)
        return FILEDATA_CORRUPT;
    ret=strchr(buf,'=');
    ret++;
    if(strcmp(ret,"UNWEIGHTED")==0)
        type=UNWEIGHTED;
    else if(strcmp(ret,"WEIGHTED")==0)
        type=WEIGHTED;

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

    //memset(m_node,0,m_vexno*sizeof(gnode_t));

    CBinaryHeap<hNode_t>::SetCompareFunc(BinCmpFunc);
    CBinaryHeap<hNode_t>::SetCopyFunc(BinCopyFunc);
    CBinaryHeap<hNode_t>::SetDummyFunc(BinDummyFunc,BinIsDummyFunc);
    CDisjointSet<AM_U32>::SetCompareFunc(SCmpFunc);
    CDisjointSet<AM_U32>::SetCopyFunc(SCopyFunc);
    CList<gvertex_t>::SetCopyFunc(copyFunc);

    for(; index<m_vexno; index++)
    {
        //fscanf(fp,"%s",m_node[index].name);
        fscanf(fp,"%d",&m_node[index].index);
        while(fscanf(fp,"%d",&input)&&input!=-1)
        {
            m_node[input].indegree++;                   //caculate indegree
            temp.index=input;
            switch(type)
            {
                case UNWEIGHTED:
                {
                    temp.weight=1;
                }
                break;
                case WEIGHTED:
                {
                    if(fscanf(fp,"%d",&input)&&input!=-1)
                    {
                        temp.weight=input;
                    }
                }
            }
            m_node[index].adjacent_node.Insert(&temp);
            noOfeges++;
        }
    }

    m_binHeap.Create(noOfeges);
    for(index=0; index<m_vexno; index++)
    {
        m_node[index].adjacent_node.ResetElemNext();
        while(m_node[index].adjacent_node.GetElemNext(&v)==RETURN_SUCCESS)
        {
            hNode.u=index;
            hNode.v=v.index;
            hNode.weight=v.weight;
            m_binHeap.Insert(&hNode);
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
    gvertex_t w; //index for dup1;
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
        while(dup1.m_node[result[i].index].adjacent_node.GetElemNext(&w)==RETURN_SUCCESS)
        {
            dup1.m_node[w.index].indegree--;
            if(dup1.m_node[w.index].indegree==0)
                q1.Enqueue(dup1.m_node[w.index].index);
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



/*************************************************************************
*
*       This is the table use to store the data of weighted & unweighted
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
*   @The table to store the result of the unweighted shortest path
*   @The unweighted path's value is one
**********************************************************************/
Err_t CGraph::UnWeightShortestPath(AM_U32 start,gtable_t* table)
{
    AM_U32 currDist=0;
    AM_U32 v;  //use index only
    gvertex_t w;
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
            if(table[w.index].distance==INFINITY)
            {
                table[w.index].distance=currDist+1;
                table[w.index].preVertex=v;
                q1.Enqueue(w.index);
            }
        }
    }
    return RETURN_SUCCESS;
}


/*********************************************************************
*
*   w is the adjacent node of v;
*   @The table to store the result of the weighted shortest path
*
**********************************************************************/
Err_t CGraph::WeightShortestPath(AM_U32 start,gtable_t* table)
{
    AM_S32 currDist=INFINITY;
    AM_U32 v;  //use index only
    gvertex_t w;
    AM_U32 i; //Use for loop
    AM_U32 loopNum=0;
    if((table==NULL)||(IsEmpty()))
        return OPERATOR_FAILED;

    table_init(table);

    table[start].distance=0;

    for(loopNum=0; loopNum<m_vexno; loopNum++)
    {
        currDist=INFINITY;
        for(i=0; i<m_vexno; i++)  //Search the smallest unknown distance
        {
            if((table[i].know!=TRUE)&&(table[i].distance!=INFINITY)&&(table[i].distance<currDist))
            {
                v=i;
                currDist=table[i].distance;
            }
        }

        table[v].know=TRUE;
        m_node[v].adjacent_node.ResetElemNext();
        while(m_node[v].adjacent_node.GetElemNext(&w)==RETURN_SUCCESS)
        {
            if(table[w.index].know!=TRUE)
            {
                if(table[w.index].distance>currDist+w.weight)
                {
                    table[w.index].distance=currDist+w.weight;
                    table[w.index].preVertex=v;
                }
            }
        }
    }
}


Err_t CGraph::NegativeWeightShortestPath(AM_U32 start,gtable_t* table)
{
    AM_S32 currDist=INFINITY;
    AM_U32 v;  //use index only
    gvertex_t w;
    CQueue<AM_U32> q1;
    if((table==NULL)||(IsEmpty()))
        return OPERATOR_FAILED;

    table_init(table);

    table[start].distance=0;

    q1.Enqueue(start);
    table[start].know=TRUE; //It means this node is in the queue;
    while(!q1.IsEmpty())
    {
        q1.Dequeue(&v);
        table[v].know=FALSE; //If out the queue, reset the know flag;
        currDist=table[v].distance;
        m_node[v].adjacent_node.ResetElemNext();

        while(m_node[v].adjacent_node.GetElemNext(&w)==RETURN_SUCCESS)
        {
            if(table[w.index].distance>currDist+w.weight)
            {
                table[w.index].distance=currDist+w.weight;
                table[w.index].preVertex=v;
                if(!table[w.index].know)
                {
                    q1.Enqueue(w.index);
                    table[w.index].know=TRUE;
                }
            }
        }

    }
    return RETURN_SUCCESS;

}

Err_t CGraph::MinSpanTree(gtable_t* table)
{
    AM_U32 s1,s2;
    AM_U32 idx;
    hNode_t hNode;
    CDisjointSet<AM_U32> nodeSet;
    CQueue<AM_U32> q1;
    if((table==NULL)||(IsEmpty()))
        return OPERATOR_FAILED;

    table_init(table);


    for(idx=0; idx<m_vexno; idx++)
    {
        nodeSet.Add(&(m_node[idx].index));
    }

    idx=0;

    while(idx<m_vexno-1)
    {
        m_binHeap.DeleteMin(&hNode);
        s1=nodeSet.Find(&hNode.u);
        s2=nodeSet.Find(&hNode.v);
        if(s1!=s2&&!table[hNode.u].know)
        {
            nodeSet.Union(&s1,&s2);
            table[hNode.u].distance=hNode.weight;
            table[hNode.u].preVertex=hNode.v;
            table[hNode.u].know=TRUE;
            idx++;

        }
    }

    return RETURN_SUCCESS;
}


/************************************************
*   This function used for shortest path yet
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

    std::cout<<"+++++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<std::right;
    std::cout<<"|"<<std::setw(7)<<"Vertex"<<std::setw(2)<<"|";
    std::cout<<std::setw(6)<<"know"<<std::setw(3)<<"|";
    std::cout<<std::setw(10)<<"Distance"<<std::setw(3)<<"|";
    std::cout<<std::setw(6)<<"Path"<<std::setw(3)<<"|"<<std::endl;
    std::cout<<"========================================="<<std::endl;
    for(i=0; i<m_vexno; i++)
    {
        std::cout<<std::right;
        std::cout<<"|"<<std::setw(4)<<"V"<<i<<std::setw(4)<<"|";
        std::cout<<std::setw(4)<<std::boolalpha<<table[i].know<<std::setw(5)<<"|";
        if(table[i].distance==INFINITY)
            std::cout<<std::setw(10)<<"INFINITY"<<std::setw(3)<<"|";
        else
            std::cout<<std::setw(7)<<table[i].distance<<std::setw(6)<<"|";
        if(table[i].preVertex==DISTANCESELF)
            std::cout<<std::setw(6)<<"Self"<<std::setw(3)<<"|"<<std::endl;
        else
            std::cout<<std::setw(4)<<"V"<<std::left<<std::setw(4)<<table[i].preVertex<<std::right<<std::setw(1)<<"|"<<std::endl;
    }
    std::cout<<"+++++++++++++++++++++++++++++++++++++++++"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"CAUTION: If the distance was INFINITY, it present there no valid path from the start point to this point!"<<std::endl;
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
    gvertex_t v;
    for(; index<this->m_vexno; index++)
    {
        m_node[index].adjacent_node.ResetElemNext();
        std::cout<<"Node "<<m_node[index].index<<": ";
        while(m_node[index].adjacent_node.GetElemNext(&v)==RETURN_SUCCESS)
        {
            std::cout<<v.index<<":"<<v.weight<<"  ";
        }
        std::cout<<std::endl;
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

