#ifndef _CDaryPoly_HEAP_HPP
#define _CDaryPoly_HEAP_HPP

#include "CTree.hpp"

template<class type>
class CDaryPolyHeap : public CTree<type>
{
public:
    CDaryPolyHeap(void);
    CDaryPolyHeap(const CDaryPolyHeap<type>& otherObject);
    CDaryPolyHeap(CDaryPolyHeap<type>&& otherObject);
    ~CDaryPolyHeap(void);

    const AM_U32 Deepth(void);
    CDaryPolyHeap<type>* Merge(CDaryPolyHeap<type>* otherHeap);
    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));

public:
    const CDaryPolyHeap<type>& operator=(const CDaryPolyHeap<type>& object);
    const CDaryPolyHeap<type>& operator=(CDaryPolyHeap<type>&& object);

private:
    void increaseDeepth(void);
private:
    AM_U32 m_deepth=0;
    /*
    *   Function pointer;
    */
private:
    cmp_t (*m_cmpRoute)(type* k1,type* k2)=nullptr;

};


template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap()
{}

template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap(const CDaryPolyHeap<type>& otherObject):CTree<type>(otherObject)
{
    m_deepth=otherObject.m_deepth;
    m_cmpRoute=otherObject.m_cmpRoute;
}

template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap(CDaryPolyHeap<type>&& otherObject):CTree<type>(otherObject)
{
    m_deepth=otherObject.m_deepth;
    m_cmpRoute=otherObject.m_cmpRoute;

    otherObject.m_deepth=0;
    otherObject.m_cmpRote=nullptr;
}

template<class type>
CDaryPolyHeap<type>::~CDaryPolyHeap()
{
    m_cmpRoute=nullptr;
    m_deepth=0;
}

template<class type>
void CDaryPolyHeap<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    if(func==nullptr)
        return;

    m_cmpRoute=func;
}


template<class type>
const AM_U32 CDaryPolyHeap<type>::Deepth(void)
{
    return m_deepth;
}

template<class type>
CDaryPolyHeap<type>* CDaryPolyHeap<type>::Merge(CDaryPolyHeap<type>* otherHeap)
{
    CDaryPolyHeap<type> t1,t2;
    CDaryPolyHeap<type>* resHeap=nullptr;
    cmp_t cmpRes;
    if(otherHeap==nullptr)
    {
        return nullptr;
    }

    if((otherHeap->m_deepth!=m_deepth)||(this->m_root==nullptr)||(otherHeap->IsEmpty()))
    {
        return nullptr;
    }

    resHeap=new CDaryPolyHeap<type>();

    t1=*this;    //Make a copy
    t2=*otherHeap;

    if((cmpRes=m_cmpRoute(&t1.m_root->elem,&t2.m_root->elem))==SMALLER)
    {
        t2.m_root->nextSibling=t1.m_root->firstChild;
        t1.m_root->firstChild=t2.m_root;
        *resHeap=t1;
        t2.m_root=nullptr; //Do not double free here;
    }
    else
    {
        t1.m_root->nextSibling=t2.m_root->firstChild;
        t2.m_root->firstChild=t1.m_root;
        *resHeap=t2;
        t1.m_root=nullptr;
    }

    resHeap->increaseDeepth();
    return resHeap;

}


template<class type>
const CDaryPolyHeap<type>& CDaryPolyHeap<type>::operator=(const CDaryPolyHeap<type>& object)
{
    CTree<type>* m1=dynamic_cast<CTree<type>* >(this);
    *m1=object;
    m_deepth=object.m_deepth;
    m_cmpRoute=object.m_cmpRoute;

}

template<class type>
const CDaryPolyHeap<type>& CDaryPolyHeap<type>::operator=(CDaryPolyHeap<type>&& object)
{
    CTree<type>* m1=dynamic_cast<CTree<type>* >(this);
    *m1=std::move(object);

    m_deepth=object.m_deepth;
    m_cmpRoute=object.m_cmpRoute;

    object.m_deepth=0;
    object.m_cmpRoute=nullptr;
}

template<class type>
void CDaryPolyHeap<type>::increaseDeepth(void)
{
    m_deepth++;
}

#endif

