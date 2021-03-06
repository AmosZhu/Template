#ifndef _CBINARY_HEAP_HPP_
#define _CBINARY_HEAP_HPP_

#include "AmosType.hpp"
#include <iostream>

/**************************************************
*
*   Binary Heap Implement
*   @Amos.Zhu
*   @2014-4-18
*
**************************************************/

template<class type>
class CBinaryHeap
{
public:
    CBinaryHeap();
    CBinaryHeap(AM_U32 maxSize);
    CBinaryHeap(const CBinaryHeap<type>& object);
    ~CBinaryHeap();

    Err_t Initialize(void);
    Err_t Create(AM_U32 maxSize);
    Err_t Insert(type* elem);
    Err_t DeleteMin(type* elem);
    AM_U32 ElemCount(void);
    BOOL IsEmpty() const;
    void Destroy();
    void PrintOut(void);

    static void SetDummyFunc(void (*dummyFunc)(type* src),BOOL(*isDummyFunc)(type* src));
    static void SetCopyFunc(void (*func)(type* dst,type* src));
    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    static void SetPrintFunc(void (*func)(type* src));

public:
    CBinaryHeap<type>& operator=(const CBinaryHeap<type>& object);

public:
    template<typename T> friend std::ostream& operator<<(std::ostream& output,const CBinaryHeap<T>& object);

private:
    AM_U32 m_capacity;
    AM_U32 m_size;
    type* m_elemArray=nullptr;

private:
    static void (*m_cpyRoute)(type* dst,type* src); //Copy function for the node,set by different type;
    static BOOL (*m_isDummy)(type* src);
    static void (*m_dummyRoute)(type* src);
    static cmp_t (*m_cmpRoute)(type* k1,type* k2);
    static void (*m_printRoute)(type* src);

};

template<class type>
void (*CBinaryHeap<type>::m_cpyRoute)(type* dst,type* src)=nullptr;

template<class type>
BOOL (*CBinaryHeap<type>::m_isDummy)(type* src)=nullptr;

template<class type>
void (*CBinaryHeap<type>::m_dummyRoute)(type* src)=nullptr;

template<class type>
cmp_t (*CBinaryHeap<type>::m_cmpRoute)(type* k1,type* k2)=nullptr;

template<class type>
void (*CBinaryHeap<type>::m_printRoute)(type* src)=nullptr;


template<class type>
CBinaryHeap<type>::CBinaryHeap()
{
    m_capacity=0;
    m_size=0;
    m_elemArray=nullptr;
}

template<class type>
CBinaryHeap<type>::CBinaryHeap(AM_U32 maxSize)
{
    m_capacity=maxSize+1;
    m_size=0;
    m_elemArray=new type[m_capacity];
}

template<class type>
CBinaryHeap<type>::CBinaryHeap(const CBinaryHeap<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return;

    m_capacity=object.m_capacity;
    m_size=object.m_size;
    m_elemArray=nullptr;

    if(!object.IsEmpty())
    {
        m_elemArray=new type[m_capacity];

        for(idx=0; idx<m_capacity; idx++)
            m_cpyRoute(&m_elemArray[idx],&object.m_elemArray[idx]);
    }

    return;
}


template<class type>
CBinaryHeap<type>::~CBinaryHeap()
{
    Destroy();
}

template<class type>
Err_t CBinaryHeap<type>::Initialize(void)
{
    AM_U32 idx;
    if(m_capacity==0||m_elemArray==nullptr||m_dummyRoute==nullptr||m_isDummy==nullptr)
        return OPERATOR_FAILED;

    for(idx=0; idx<m_capacity; idx++)
    {
        m_dummyRoute(&m_elemArray[idx]);
    }

    return RETURN_SUCCESS;
}

template<class type>
Err_t CBinaryHeap<type>::Create(AM_U32 maxSize)
{
    if(maxSize==0)
        return INVALIDE_PARAMET;

    if(!IsEmpty())
        Destroy();

    m_capacity=maxSize+1;
    m_size=0;
    m_elemArray=new type[m_capacity];

    return RETURN_SUCCESS;
}

template<class type>
AM_U32 CBinaryHeap<type>::ElemCount(void)
{
    return m_size;
}

template<class type>
BOOL CBinaryHeap<type>::IsEmpty(void) const
{
    if(m_size==0)
        return TRUE;

    return FALSE;
}

template<class type>
Err_t CBinaryHeap<type>::Insert(type* elem)
{
    AM_U32 idx;
    if(elem==nullptr)
        return INVALIDE_PARAMET;

    for(idx=++m_size; SMALLER==m_cmpRoute(elem,&m_elemArray[idx/2]); idx/=2)
    {
        m_cpyRoute(&m_elemArray[idx],&m_elemArray[idx/2]);
    }

    m_cpyRoute(&m_elemArray[idx],elem);

    return RETURN_SUCCESS;
}

template<class type>
Err_t CBinaryHeap<type>::DeleteMin(type* elem)
{
    AM_U32 idx,childIdx;
    type lastElem;
    if(elem==nullptr)
        return INVALIDE_PARAMET;

    if(IsEmpty())
        return OPERATOR_FAILED;

    m_cpyRoute(elem,&m_elemArray[1]); //Get the minimum element

    m_cpyRoute(&lastElem,&m_elemArray[m_size]);
    m_dummyRoute(&m_elemArray[m_size--]); //Set the last elem dummy

    for(idx=1; idx*2<=m_size; idx=childIdx)
    {
        childIdx=idx*2;

        if(!m_isDummy(&m_elemArray[childIdx]))
        {
            if(!m_isDummy(&m_elemArray[childIdx+1]))
            {
                if(m_cmpRoute(&m_elemArray[childIdx],&m_elemArray[childIdx+1])==LARGER)
                    childIdx++;
            }

            if(m_cmpRoute(&lastElem,&m_elemArray[childIdx])==LARGER)
                m_cpyRoute(&m_elemArray[idx],&m_elemArray[childIdx]);
            else
                break;
        }
        else
        {
            break;
        }
    }

    m_cpyRoute(&m_elemArray[idx],&lastElem);
    return RETURN_SUCCESS;
}

template<class type>
void CBinaryHeap<type>::Destroy(void)
{
    if(m_elemArray!=nullptr)
    {
        delete[] m_elemArray;
        m_elemArray=nullptr;
        m_capacity=0;
        m_size=0;
    }
}

template<class type>
void CBinaryHeap<type>::PrintOut()
{
    AM_U32 idx;
    if(!IsEmpty())
    {
        for(idx=1; idx<=m_size; idx++)
            m_printRoute(&m_elemArray[idx]);

        printf("\n");
    }
    else
    {
        printf("This heap is empty!\n");
    }

}

template<class type>
void CBinaryHeap<type>::SetDummyFunc( void (*dummyFunc)(type* src),BOOL(*isDummyFunc)(type* src))
{
    m_dummyRoute=dummyFunc;
    m_isDummy=isDummyFunc;
}


template<class type>
void CBinaryHeap<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    m_cmpRoute=func;
}

template<class type>
void CBinaryHeap<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    m_cpyRoute=func;
}


template<class type>
void CBinaryHeap<type>::SetPrintFunc(void (*func)(type* src))
{
    m_printRoute=func;
}


template<class type>
CBinaryHeap<type>& CBinaryHeap<type>::operator=(const CBinaryHeap<type>& object)
{
    AM_U32 idx;
    if(this==&object)
        return *this;

    m_capacity=object.m_capacity;
    m_size=object.m_size;
    m_elemArray=nullptr;

    if(!object.IsEmpty())
    {
        m_elemArray=new type[m_capacity];

        for(idx=0; idx<m_capacity; idx++)
            m_cpyRoute(&m_elemArray[idx],&object.m_elemArray[idx]);
    }

    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& output,const CBinaryHeap<T>& object)
{
    AM_U32 idx;
    if(!object.IsEmpty())
    {
        for(idx=1; idx<=object.m_size; idx++)
            object.m_printRoute(&object.m_elemArray[idx]);

        output<<std::endl;
    }
    else
    {
        output<<"This is an empty heap!"<<std::endl;
    }
}


#endif
