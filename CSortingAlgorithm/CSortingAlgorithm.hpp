#ifndef _CSORTING_ALGORITHM_HPP
#define _CSORTING_ALGORITHM_HPP

#include "AmosType.hpp"
#include "CBinaryHeap.hpp"
#include <string.h>

#define QSORT_CUTOFF (3)

template<typename type>
class CSortingAlgorithm
{
public:
    CSortingAlgorithm(void)=delete;
    CSortingAlgorithm(const CSortingAlgorithm<type>& other)=delete;
    CSortingAlgorithm(CSortingAlgorithm<type>&& other)=delete;
    ~CSortingAlgorithm(void)=delete;

public:
    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    static void SetCopyFunc(void (*func)(type* dst,type* src));
    static void SetDummyFunc(void (*dummyFunc)(type* src),BOOL(*isDummyFunc)(type* src));

    static Err_t InsertionSort(type* array, AM_U32 len);
    static Err_t ShellSort(type* array, AM_U32 len);
    static Err_t HeapSort(type* array, AM_U32 len);
    static Err_t MergeSort(type* array, AM_U32 len);
    static Err_t QuickSort(type* array, AM_U32 len);

private:
    /*
    *   For merge sort
    */
    static void msort(type* array,AM_U32 lPos,AM_U32 rPos);
    static void merge(type* array, AM_U32 lPos, AM_U32 cPos, AM_U32 rPos);
    static void qsort(type* array,AM_U32 lPos,AM_U32 rPos);

private:
    static cmp_t (*m_cmpRoute)(type* k1, type* k2);
    static void (*m_cpyRoute)(type* dst,type* src);


};

template<class type>
void (*CSortingAlgorithm<type>::m_cpyRoute)(type* dst,type* src)=nullptr;

template<class type>
cmp_t (*CSortingAlgorithm<type>::m_cmpRoute)(type* k1,type* k2)=nullptr;


template<class type>
void CSortingAlgorithm<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{

    if(func==nullptr)
        return;

    m_cmpRoute=func;

    CBinaryHeap<type>::SetCompareFunc(func);
}

template<class type>
void CSortingAlgorithm<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    if(func==nullptr)
        return;

    m_cpyRoute=func;

    CBinaryHeap<type>::SetCopyFunc(func);
}

template<class type>
void CSortingAlgorithm<type>::SetDummyFunc( void (*dummyFunc)(type* src),BOOL(*isDummyFunc)(type* src))
{
    CBinaryHeap<type>::SetDummyFunc(dummyFunc,isDummyFunc);
}

template<class type>
Err_t CSortingAlgorithm<type>::InsertionSort(type* array, AM_U32 len)
{
    AM_U32 i,j;
    type temp;
    if(array==nullptr||len<=1)
    {
        return INVALIDE_PARAMET;
    }


    for(i=1; i<len; i++)
    {
        m_cpyRoute(&temp,&array[i]);
        for(j=i; j>0; j--)
        {
            if(m_cmpRoute(&array[j-1],&temp)==LARGER)
            {
                array[j]=array[j-1];
            }
            else
            {
                break;
            }
        }

        m_cpyRoute(&array[j],&temp);
    }

    return RETURN_SUCCESS;
}

template<class type>
Err_t CSortingAlgorithm<type>::ShellSort(type* array, AM_U32 len)
{
    AM_U32 i,j,increment;
    type temp;
    if(array==nullptr||len<=1)
    {
        return INVALIDE_PARAMET;
    }

    for(increment=len/2; increment>0; increment/=2)
    {
        for(i=increment; i<len; i++)
        {
            m_cpyRoute(&temp,&array[i]);
            for(j=i; j>=increment; j-=increment)
            {
                if(m_cmpRoute(&array[j-increment],&temp)==LARGER)
                {
                    array[j]=array[j-increment];
                }
                else
                {
                    break;
                }
            }
            m_cpyRoute(&array[j],&temp);
        }
    }

    return RETURN_SUCCESS;

}

template<class type>
Err_t CSortingAlgorithm<type>::HeapSort(type* array, AM_U32 len)
{
    AM_U32 i,length;
    CBinaryHeap<type> binHeap;
    if(array==nullptr||len<=1)
    {
        return INVALIDE_PARAMET;
    }

    binHeap.Create(len);
    binHeap.Initialize();

    length=len;
    for(i=0; i<length; i++)
    {
        binHeap.Insert(&array[i]);
    }

    length=binHeap.ElemCount();
    for(i=0; i<length; i++ )
    {
        binHeap.DeleteMin(&array[i]);
    }


    return RETURN_SUCCESS;
}

template<class type>
Err_t CSortingAlgorithm<type>::MergeSort(type* array, AM_U32 len)
{
    if(array==nullptr||len<=1)
    {
        return INVALIDE_PARAMET;
    }

    msort(array,0,len-1);

    return RETURN_SUCCESS;
}

template<class type>
Err_t CSortingAlgorithm<type>::QuickSort(type* array, AM_U32 len)
{
    if(array==nullptr||len<=1)
    {
        return INVALIDE_PARAMET;
    }

    qsort(array,0,len-1);

    return RETURN_SUCCESS;
}


template<class type>
void CSortingAlgorithm<type>::msort(type* array,AM_U32 lPos,AM_U32 rPos)
{
    AM_U32 centerPos;
    if(lPos<rPos)
    {
        centerPos=(lPos+rPos)/2;
        msort(array,lPos,centerPos);
        msort(array,centerPos+1,rPos);
        merge(array,lPos,centerPos,rPos);
    }
}

template<class type>
void CSortingAlgorithm<type>::merge(type* array, AM_U32 lPos, AM_U32 cPos, AM_U32 rPos)
{
    AM_U32 len=rPos-lPos+1;
    type tmpArray[len];
    AM_U32 i;
    AM_U32 lP,rP,tP; //left Pointer, right Pointer and temp array pointer;
    AM_U32 leftEnd,rightEnd;

    leftEnd=cPos;
    rightEnd=rPos;

    lP=lPos;
    rP=cPos+1;
    tP=0;

    while(lP<=leftEnd&&rP<=rightEnd)
    {
        if(m_cmpRoute(&array[lP],&array[rP])==SMALLER)
        {
            m_cpyRoute(&tmpArray[tP++],&array[lP++]);
        }
        else
        {
            m_cpyRoute(&tmpArray[tP++],&array[rP++]);
        }
    }

    for(; lP<=leftEnd; lP++,tP++)
    {
        m_cpyRoute(&tmpArray[tP],&array[lP]);
    }

    for(; rP<=rightEnd; rP++,tP++)
    {
        m_cpyRoute(&tmpArray[tP],&array[rP]);
    }

    for(i=0; i<len; i++)
    {
        m_cpyRoute(&array[lPos+i],&tmpArray[i]);
    }

}

template<class type>
void CSortingAlgorithm<type>::qsort(type* array,AM_U32 lPos,AM_U32 rPos)
{
    AM_U32 i,j;
    AM_U32 center;
    type pivot;

    if(lPos+QSORT_CUTOFF<rPos)
    {

        /*
        *   find the pivot,and swap
        */

        center=(lPos+rPos)/2;
        pivot=array[center];
        std::swap(array[center],array[rPos]);

        /*
        *   Sorting the left array and right array
        */
        i=lPos;
        j=rPos-1;
        for(;;)
        {
            while(m_cmpRoute(&array[i],&pivot)==SMALLER)
            {
                i++;
            }
            while(m_cmpRoute(&array[j],&pivot)==LARGER)
            {
                j--;
            }
            if(i<j)
            {
                std::swap(array[i],array[j]);
            }
            else
            {
                break;
            }
        }
        std::swap(array[i],array[rPos]); //restore the pivot
        qsort(array,lPos,i);
        qsort(array,i+1,rPos);
    }
    else
    {
        InsertionSort(&array[lPos],rPos-lPos+1);
    }
}


#endif
