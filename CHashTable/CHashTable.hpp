#ifndef _CHASHTABLE_HPP_
#define _CHASHTABLE_HPP_


/**************************************************************
*
*   CHashTable seperate chainning
*   @Author: Amos.Zhu
*   @2014-4-17
*
**************************************************************/


#include "AmosType.hpp"
#include "CList.hpp"
#include <iostream>

template<class type>
class CHashTable
{
public:
    CHashTable();
    CHashTable(AM_U32 size);
    CHashTable(const CHashTable<type>& object);

    ~CHashTable();

    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    void SetCopyFunc(void (*func)(type* dst,type* src));
    void SetPrintFunc(void (*func)(type* src));
    AM_U32 SetHashFunc(AM_U32 (*func)(type* elem));

    BOOL IsEmpty(void) const;
    Err_t Insert(type* elem);
    Err_t Delete(type* elem);
    /* Return the address of the element */
    type* Search(type* elem);
    void PrintOut(void);
    void Destroy(void);

public:
    CHashTable<type>& operator=(const CHashTable<type>& object);

public:
    template<typename T> friend std::ostream& operator<<(std::ostream& output,const CHashTable<T>& object);


private:
    AM_U32 (*m_hashRoute)(type* elem);
    CList<type>* m_list;
    AM_U32 m_size;


};

template<class type>
CHashTable<type>::CHashTable()
{
    m_size=0;
    m_list=NULL;
    m_hashRoute=NULL;
}

template<class type>
CHashTable<type>::CHashTable(AM_U32 size)
{
    m_size=size;
    m_list=new CList<type>[size];
    m_hashRoute=NULL;
}

template<class type>
CHashTable<type>::CHashTable(const CHashTable<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return;

    m_size=0;
    m_list=NULL;
    m_hashRoute=NULL;

    if(!object.IsEmpty())
    {
        m_size=object.m_size;
        m_list=new CList<type>[m_size];
        for(idx=0; idx<object.m_size; idx++)
        {
            m_list[idx]=object.m_list[idx];
        }
    }

    return;
}

template<class type>
CHashTable<type>::~CHashTable()
{
    Destroy();
}

template<class type>
void CHashTable<type>::Destroy(void)
{
    AM_U32 idx;
    for(idx=0; idx<m_size; idx++)
    {
        m_list[idx].Destroy();
    }

    delete[] m_list;

    m_size=0;
    m_list=NULL;
}

template<class type>
BOOL CHashTable<type>::IsEmpty(void) const
{
    if(m_size==0||m_list==NULL)
        return TRUE;

    return FALSE;
}

template<class type>
void CHashTable<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    AM_U32 idx;
    for(idx=0; idx<m_size; idx++)
    {
        m_list[idx].SetCompareFunc(func);
    }
}

template<class type>
void CHashTable<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    AM_U32 idx;
    for(idx=0; idx<m_size; idx++)
    {
        m_list[idx].SetCopyFunc(func);
    }
}

template<class type>
void CHashTable<type>::SetPrintFunc(void (*func)(type* src))
{
    AM_U32 idx;
    for(idx=0; idx<m_size; idx++)
    {
        m_list[idx].SetPrintFunc(func);
    }

}

template<class type>
AM_U32 CHashTable<type>::SetHashFunc(AM_U32 (*func)(type* elem))
{
    m_hashRoute=func;
}

template<class type>
Err_t CHashTable<type>::Insert(type * elem)
{
    AM_U32 pos;
    if(elem==NULL)
        return INVALIDE_PARAMET;

    pos=m_hashRoute(elem);

    if(m_list[pos].Search(elem)!=NULL)
        return RETURN_SUCCESS;

    m_list[pos].Insert(elem);

    return RETURN_SUCCESS;
}

template<class type>
Err_t CHashTable<type>::Delete(type * elem)
{
    AM_U32 pos;
    if(elem==NULL)
        return INVALIDE_PARAMET;

    pos=m_hashRoute(elem);

    if(m_list[pos].Delete(elem)==OPERATOR_FAILED)
        return OPERATOR_FAILED;

    return RETURN_SUCCESS;
}

template<class type>
type* CHashTable<type>::Search(type * elem)
{
    AM_U32 pos;
    if(elem==NULL)
        return NULL;

    pos=m_hashRoute(elem);

    return m_list[pos].Search(elem);
}

template<class type>
void CHashTable<type>::PrintOut(void)
{
    AM_U32 idx;
    for(idx=0; idx<m_size; idx++)
    {
        printf("[%d]:",idx);
        m_list[idx].PrintOut();
    }
}

template<class type>
CHashTable<type>& CHashTable<type>::operator=(const CHashTable<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return *this;

    m_size=0;
    m_list=NULL;

    if(!object.IsEmpty())
    {
        m_size=object.m_size;
        m_list=new CList<type>[m_size];
        for(idx=0; idx<object.m_size; idx++)
        {
            m_list[idx]=object.m_list[idx];
        }
    }
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& output,const CHashTable<T>& object)
{
    AM_U32 idx;
    for(idx=0; idx<object.m_size; idx++)
    {
        output<<"["<<idx<<"]";
        object.m_list[idx].PrintOut();
    }
    return output;
}

#endif
