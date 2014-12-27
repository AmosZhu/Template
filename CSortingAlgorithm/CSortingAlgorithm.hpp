#ifndef _CSORTING_ALGORITHM_HPP
#define _CSORTING_ALGORITHM_HPP

#include "AmosType.hpp"
#include <string.h>

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

    static Err_t InsertionSort(type* array, AM_U32 len);
    static Err_t ShellSort(type* array, AM_U32 len);

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
}

template<class type>
void CSortingAlgorithm<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    if(func==nullptr)
        return;

    m_cpyRoute=func;
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

}

#endif
