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
    CPolynomialHeap(const CPolynomialHeap<type>&& object);
    ~CPolynomialHeap(void);

    Err_t Add(type* elem);
    void PrintOut(void);

public:
    static CPolynomialHeap<type>* Merge(CPolynomialHeap<type>* heap1,CPolynomialHeap<type>* heap2);

    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    static void SetCopyFunc(void (*func)(type* dst,type* src));
    static void SetPrintFunc(void (*func)(type* src));

private:
    CDaryPolyHeap<type>* combineTree(CDaryPolyHeap<type>* t1, CDaryPolyHeap<type>* t2);

private:
    AM_U32 m_deepth;
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
    m_deepth=0;
}

template<class type>
CPolynomialHeap<type>::CPolynomialHeap(const CPolynomialHeap <type> & object)
{}

template<class type>
CPolynomialHeap<type>::CPolynomialHeap(const CPolynomialHeap <type>&& object)
{}

template<class type>
CPolynomialHeap<type>::~CPolynomialHeap(void)
{
    if(m_root!=nullptr)
    {
        m_root->clear();
        delete m_root;
    }
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
CPolynomialHeap<type>* CPolynomialHeap<type>::Merge(CPolynomialHeap<type>* heap1,CPolynomialHeap<type>* heap2)
{
    return nullptr;
}


template<class type>
CDaryPolyHeap<type>* CPolynomialHeap<type>::combineTree(CDaryPolyHeap<type>* t1, CDaryPolyHeap<type>* t2)
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


#endif
