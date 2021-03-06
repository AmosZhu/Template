#ifndef _CPOLYNOMIAL_HEAP_HPP_
#define _CPOLYNOMIAL_HEAP_HPP_

#include "AmosType.hpp"
#include "CDaryPolyHeap.hpp"
#include <vector>


template<class type>
class CPolynomialHeap
{
public:
    CPolynomialHeap(void);
    CPolynomialHeap(const CPolynomialHeap<type>& object);
    CPolynomialHeap(CPolynomialHeap<type>&& object);
    ~CPolynomialHeap(void);

    BOOL IsEmpty() const;
    Err_t Add(type* elem);
    void PrintOut(void);

public:
    static CPolynomialHeap<type>* Merge(CPolynomialHeap<type>* heap1,CPolynomialHeap<type>* heap2);
    static CPolynomialHeap<type>* DeleteMin(CPolynomialHeap<type>* src,type* elem=nullptr);

    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    static void SetCopyFunc(void (*func)(type* dst,type* src));
    static void SetPrintFunc(void (*func)(type* src));

public:
    const CPolynomialHeap<type>& operator=(const CPolynomialHeap<type>& object);
    const CPolynomialHeap<type>& operator=(CPolynomialHeap<type>&& object);

private:
    void destroy(void);

private:
    static CDaryPolyHeap<type>* combineTree(CDaryPolyHeap<type>* t1,CDaryPolyHeap<type>* t2);

private:
    std::vector<CDaryPolyHeap<type>*>* m_root=nullptr;

private:
    static void (*m_cpyRoute)(type* dst,type* src);
    static cmp_t (*m_cmpRoute)(type* k1,type* k2);
    static void (*m_printRoute)(type* src);
};

template<class type>
void (*CPolynomialHeap<type>::m_cpyRoute)(type* dst,type* src)=nullptr;

template<class type>
cmp_t (*CPolynomialHeap<type>::m_cmpRoute)(type* k1,type* k2)=nullptr;

template<class type>
void (*CPolynomialHeap<type>::m_printRoute)(type* src)=nullptr;

template<class type>
CPolynomialHeap<type>::CPolynomialHeap(void)
{
}

template<class type>
CPolynomialHeap<type>::CPolynomialHeap(const CPolynomialHeap<type>& object)
{
    AM_U32 idx;
    CDaryPolyHeap<type>* subHeap=nullptr;
    if(object.IsEmpty())
    {
        m_root=nullptr;
        return;
    }

    m_root=new std::vector<CDaryPolyHeap<type>*>();

    for(idx=0; idx<object.m_root->size(); idx++)
    {
        if(object.m_root->at(idx)!=nullptr)
        {
            subHeap=new CDaryPolyHeap<type>();
            *subHeap=*object.m_root->at(idx);
            m_root->push_back(subHeap);
        }
        else
        {
            m_root->push_back(nullptr);
        }
    }

    return;

}

template<class type>
CPolynomialHeap<type>::CPolynomialHeap(CPolynomialHeap<type>&& object)
{
    m_root=object.m_root;

    object.m_root=nullptr;
}

template<class type>
CPolynomialHeap<type>::~CPolynomialHeap(void)
{
    destroy();
}

template<class type>
BOOL CPolynomialHeap<type>::IsEmpty() const
{
    if(m_root==nullptr)
        return TRUE;

    if(m_root->size()==0)
        return TRUE;

    return FALSE;
}


template<class type>
void CPolynomialHeap<type>::PrintOut(void)
{
    AM_U32 i;
    if(m_root==nullptr)
        return;

    for(i=0; i<m_root->size(); i++)
    {
        if(m_root->at(i)!=nullptr)
        {
            std::cout<<"node "<<i<<" deepth is: "<<m_root->at(i)->Deepth()<<std::endl;
            m_root->at(i)->PreOrderTraversal();
        }
    }

}

template<class type>
Err_t CPolynomialHeap<type>::Add(type* elem)
{
    CDaryPolyHeap<type>* newSubTree;
    CDaryPolyHeap<type>* carry;
    AM_U32 idx;
    if(elem==nullptr)
    {
        return INVALIDE_PARAMET;
    }

    newSubTree=new CDaryPolyHeap<type>();

    newSubTree->SetRoot(elem);

    if(m_root==nullptr)
    {
        m_root=new std::vector<CDaryPolyHeap<type>*>();
        m_root->push_back(newSubTree);
    }
    else
    {
        if(m_root->at(0)==nullptr)
        {
            m_root->at(0)=newSubTree;
        }
        else
        {
            for(idx=0; idx<m_root->size()&&m_root->at(idx)!=nullptr; idx++)
            {
                carry=CDaryPolyHeap<type>::Merge(newSubTree,m_root->at(idx));
                delete m_root->at(idx);
                m_root->at(idx)=nullptr;
                delete newSubTree;
                newSubTree=carry;
            }

            if(idx==m_root->size())
            {
                m_root->push_back(carry);
            }
            else
            {
                m_root->at(idx)=carry;
            }
        }
    }


    return RETURN_SUCCESS;
}

template<class type>
CPolynomialHeap<type>* CPolynomialHeap<type>::DeleteMin(CPolynomialHeap<type>* src,type* elem)
{
    AM_U32 len;
    AM_U32 idx,i;
    cmp_t cmpRes;
    type min,current;
    CDaryPolyHeap<type>* deleteHeap;
    CDaryPolyHeap<AM_U32>** subForest;
    CPolynomialHeap<type> sourceHeap;
    CPolynomialHeap<type>* newHeap;
    CPolynomialHeap<type>* resHeap;


    if(src==nullptr)
    {
        return nullptr;
    }

    if(src->IsEmpty())
    {
        return nullptr;
    }

    sourceHeap=*src;
    /*
    *   Init the min first
    */
    for(i=0; i<sourceHeap.m_root->size(); i++)
    {
        if(sourceHeap.m_root->at(i)!=nullptr)
        {
            sourceHeap.m_root->at(i)->GetRootElem(&min);
            idx=i;
            break;
        }
    }

    for(; i<sourceHeap.m_root->size(); i++)
    {
        if(sourceHeap.m_root->at(i)!=nullptr)
        {
            sourceHeap.m_root->at(i)->GetRootElem(&current);
            if(m_cmpRoute(&min,&current)==LARGER)
            {
                m_cpyRoute(&min,&current);
                idx=i;
            }
        }
    }

    if(elem!=nullptr)
    {
        m_cpyRoute(elem,&min);
    }

    /*
    *   Point to the deleteHeap;
    */

    deleteHeap=sourceHeap.m_root->at(idx);
    sourceHeap.m_root->at(idx)==nullptr;
    if(idx==sourceHeap.m_root->size()-1)
    {
        sourceHeap.m_root->pop_back();
    }

    subForest=CDaryPolyHeap<type>::DeleteMin(deleteHeap,&len);
    delete deleteHeap;
    deleteHeap=nullptr;

    if(len>0)
    {
        newHeap=new CPolynomialHeap<type>();
        newHeap->m_root=new std::vector<CDaryPolyHeap<type>*>();

        for(i=0; i<len; i++)
        {
            newHeap->m_root->push_back(subForest[i]);
        }

        resHeap=CPolynomialHeap<type>::Merge(&sourceHeap,newHeap);
    }
    else
    {
        resHeap=new CPolynomialHeap<type>();
        *resHeap=std::move(sourceHeap);
    }

    return resHeap;
}


template<class type>
CPolynomialHeap<type>* CPolynomialHeap<type>::Merge(CPolynomialHeap<type>* heap1,CPolynomialHeap<type>* heap2)
{
    CPolynomialHeap<type>* resHeap;
    AM_U32 valC,val1,val2;
    AM_U32 i=0,j=0;
    AM_U32 idx;
    CDaryPolyHeap<type>* newSubHeap=nullptr;
    CDaryPolyHeap<type>* carry=nullptr;
    if(heap1==nullptr||heap2==nullptr)
        return nullptr;

    if(heap1->IsEmpty()||heap2->IsEmpty())
        return nullptr;

    resHeap=new CPolynomialHeap<type>();
    resHeap->m_root=new std::vector<CDaryPolyHeap<type>*>();
    while(i<heap1->m_root->size()||j<heap2->m_root->size())
    {
        val1=0;
        val2=0;
        valC=0;
        if(i<heap1->m_root->size()&&heap1->m_root->at(i)!=nullptr)
        {
            val1=1;
        }

        if(j<heap2->m_root->size()&&heap2->m_root->at(j)!=nullptr)
        {
            val2=2;
        }

        if(carry!=nullptr)
        {
            valC=4;
        }

        switch(val1+val2+valC)
        {
            case 0:
            {
                /*
                *   Do nothing;
                */
            }
            break;
            case 1:
            {
                newSubHeap=new CDaryPolyHeap<type>();
                *newSubHeap=*heap1->m_root->at(i); //copy
                resHeap->m_root->push_back(newSubHeap);
            }
            break;
            case 2:
            {
                newSubHeap=new CDaryPolyHeap<type>();
                *newSubHeap=*heap2->m_root->at(j); //copy
                resHeap->m_root->push_back(newSubHeap);
            }
            break;
            case 3:
            {
                carry=combineTree(heap1->m_root->at(i),heap2->m_root->at(j)); //copy
                resHeap->m_root->push_back(nullptr);
            }
            break;
            case 4:
            {
                resHeap->m_root->push_back(carry);
                carry=nullptr;
            }
            break;
            case 5:
            {
                newSubHeap=combineTree(heap1->m_root->at(i),carry); //newSubHeap for temporary carry
                delete carry;
                carry=newSubHeap;
                resHeap->m_root->push_back(nullptr);
            }
            break;
            case 6:
            {
                newSubHeap=combineTree(heap2->m_root->at(j),carry); //newSubHeap for temporary carry
                delete carry;
                carry=newSubHeap;
                resHeap->m_root->push_back(nullptr);
            }
            break;
            case 7:
            {
                newSubHeap=combineTree(heap1->m_root->at(i),heap2->m_root->at(j)); //newSubHeap for temporary carry
                resHeap->m_root->push_back(carry);
                carry=newSubHeap;
            }
            break;

        }

        if(i<heap1->m_root->size())
        {
            i++;
        }
        if(j<heap2->m_root->size())
        {
            j++;
        }

    }

    if(carry!=nullptr)
    {
        resHeap->m_root->push_back(carry);
    }

    return resHeap;
}

template<class type>
void CPolynomialHeap<type>::destroy(void)
{
    if(m_root!=nullptr)
    {
        m_root->clear();
        delete m_root;
        m_root=nullptr;
    }

}

template<class type>
CDaryPolyHeap<type>* CPolynomialHeap<type>::combineTree(CDaryPolyHeap<type>* t1,CDaryPolyHeap<type>* t2)
{
    cmp_t cmpRes;
    if(t1==nullptr)
        return t2;

    if(t2==nullptr)
        return t1;

    return CDaryPolyHeap<type>::Merge(t1,t2);
}

template<class type>
void CPolynomialHeap<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{

    if(func==nullptr)
        return;

    m_cmpRoute=func;
    CDaryPolyHeap<type>::SetCompareFunc(func);

}

template<class type>
void CPolynomialHeap<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    if(func==nullptr)
        return;

    m_cpyRoute=func;
    CDaryPolyHeap<type>::SetCopyFunc(func);
}

template<class type>
void CPolynomialHeap<type>::SetPrintFunc(void (*func)(type* src))
{
    if(func==nullptr)
        return;

    m_printRoute=func;
    CDaryPolyHeap<type>::SetPrintFunc(func);
}

template<class type>
const CPolynomialHeap<type>& CPolynomialHeap<type>::operator=(const CPolynomialHeap<type>& object)
{
    AM_U32 idx;
    CDaryPolyHeap<type>* subHeap=nullptr;

    if(this==&object)
        return *this;

    destroy();

    if(object.IsEmpty())
    {
        m_root=nullptr;
        return *this;
    }

    m_root=new std::vector<CDaryPolyHeap<type>*>();

    for(idx=0; idx<object.m_root->size(); idx++)
    {
        if(object.m_root->at(idx)!=nullptr)
        {
            subHeap=new CDaryPolyHeap<type>();
            *subHeap=*object.m_root->at(idx);
            m_root->push_back(subHeap);
        }
        else
        {
            m_root->push_back(nullptr);
        }
    }

    return *this;
}

template<class type>
const CPolynomialHeap<type>& CPolynomialHeap<type>::operator=(CPolynomialHeap<type>&& object)
{
    if(this==&object)
        return *this;

    destroy();

    m_root=object.m_root;

    object.m_root=nullptr;

    return *this;
}


#endif
