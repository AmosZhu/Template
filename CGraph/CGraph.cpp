#include "CGraph.hpp"
#include "AmosType.hpp"

CGraph::CGraph(void)
{
    m_node=NULL;
    m_vexno=0;
}

//Not implement yet
#if 0
CGraph::CGraph(const CGraph& object)
{}
#endif /* Modify by Amos.zhu */

CGraph::~CGraph(void)
{
    //Destroy();
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

    for(; index<m_vexno; index++)
    {
        fscanf(fp,"%s",m_node[index].name);
        while(fscanf(fp,"%d",&input)&&input!=-1)
        {
            m_node[index].adjacent_node.Insert(input);
        }
    }

    return RETURN_SUCCESS;
}

BOOL CGraph::IsEmpty(void)
{
    if(!check_resource())
        return FALSE;
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
        std::cout<<"Node "<<index<<": ";
        std::cout<<this->m_node[index].adjacent_node;
        std::cout<<"In Degree: ";
        std::cout<<"Out Degree: ";
        std::cout<<std::endl;
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

