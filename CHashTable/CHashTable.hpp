#ifndef _CHASHTABLE_HPP_
#define _CHASHTABLE_HPP_

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
    CList<type>* list;
    AM_U32 tblsize;


};

template<class type>
CHashTable<type>::CHashTable()
{
	tblsize=0;
    list=NULL;
    m_hashRoute=NULL;
}

template<class type>
CHashTable<type>::CHashTable(AM_U32 size)
{
	tblsize=size;
    list=new CList<type>[size];
    m_hashRoute=NULL;
}

template<class type>
CHashTable<type>::CHashTable(const CHashTable<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return;

    tblsize=0;
    list=NULL;
    m_hashRoute=NULL;

    if(!object.IsEmpty())
    {
    	tblsize=object.tblsize;
    	list=new CList<type>[tblsize];
        for(idx=0; idx<object.tblsize; idx++)
        {
            list[idx]=object.list[idx];
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
    for(idx=0; idx<tblsize; idx++)
    {
        list[idx].Destroy();
    }

    delete[] list;

    tblsize=0;
    list=NULL;
}

template<class type>
BOOL CHashTable<type>::IsEmpty(void) const
{
	if(tblsize==0||list==NULL)
		return TRUE;

	return FALSE;
	}

template<class type>
void CHashTable<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    AM_U32 idx;
    for(idx=0; idx<tblsize; idx++)
    {
        list[idx].SetCompareFunc(func);
    }
}

template<class type>
void CHashTable<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    AM_U32 idx;
    for(idx=0; idx<tblsize; idx++)
    {
        list[idx].SetCopyFunc(func);
    }
}

template<class type>
void CHashTable<type>::SetPrintFunc(void (*func)(type* src))
{
    AM_U32 idx;
    for(idx=0; idx<tblsize; idx++)
    {
        list[idx].SetPrintFunc(func);
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

    if(list[pos].Search(elem)!=NULL)
        return RETURN_SUCCESS;

    list[pos].Insert(elem);

    return RETURN_SUCCESS;
}

template<class type>
Err_t CHashTable<type>::Delete(type * elem)
{
    AM_U32 pos;
    if(elem==NULL)
        return INVALIDE_PARAMET;

    pos=m_hashRoute(elem);

    if(list[pos].Delete(elem)==OPERATOR_FAILED)
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

    return list[pos].Search(elem);
}

template<class type>
void CHashTable<type>::PrintOut(void)
{
    AM_U32 idx;
    for(idx=0; idx<tblsize; idx++)
    {
        printf("[%d]:",idx);
        list[idx].PrintOut();
        //printf("\n");
    }
}

template<class type>
CHashTable<type>& CHashTable<type>::operator=(const CHashTable<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return *this;

    tblsize=0;
    list=NULL;

    if(!object.IsEmpty())
    {
    	tblsize=object.tblsize;
    	list=new CList<type>[tblsize];
        for(idx=0; idx<object.tblsize; idx++)
        {
            list[idx]=object.list[idx];
        }
    }
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& output,const CHashTable<T>& object)
{
    AM_U32 idx;
    for(idx=0; idx<object.tblsize; idx++)
    {
        output<<"["<<idx<<"]";
        object.list[idx].PrintOut();
        //output<<std::endl;
    }
    return output;
}

#endif
