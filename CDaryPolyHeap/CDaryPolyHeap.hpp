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
    Err_t GetRootElem(type* elem);

    /*
    *   This function will merge tree1 and tree2 and return a new tree.
    *   And the tree1 and tree2 will be no effect;
    */
    static CDaryPolyHeap<type>* Merge(CDaryPolyHeap<type>* tree1,CDaryPolyHeap<type>* tree2);
    /*
    *   @src is the input tree who want to be delete min.
    *   and output is a tree list, and the minimum element will output by the elem.
    *   @src will remain same as before. This will copy src first then do operation.
    */
    static CDaryPolyHeap<type>** DeleteMin(CDaryPolyHeap<type>* src,AM_U32* len,type* elem=nullptr);

public:
    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));

public:
    const CDaryPolyHeap<type>& operator=(const CDaryPolyHeap<type>& object);
    const CDaryPolyHeap<type>& operator=(CDaryPolyHeap<type>&& object);

private:
    void increaseDeepth(void);
    void decreaseDeepth(void);
private:
    AM_U32 m_deepth=0;
    /*
    *   Function pointer;
    */
private:
    static cmp_t (*m_cmpRoute)(type* k1,type* k2);

};

template<class type>
cmp_t (*CDaryPolyHeap<type>::m_cmpRoute)(type* k1,type* k2)=nullptr;

template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap()
{}

template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap(const CDaryPolyHeap<type>& otherObject):CTree<type>(otherObject)
{
    m_deepth=otherObject.m_deepth;
}

template<class type>
CDaryPolyHeap<type>::CDaryPolyHeap(CDaryPolyHeap<type>&& otherObject):CTree<type>(otherObject)
{
    m_deepth=otherObject.m_deepth;

    otherObject.m_deepth=0;
}

template<class type>
CDaryPolyHeap<type>::~CDaryPolyHeap()
{
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
CDaryPolyHeap<type>* CDaryPolyHeap<type>::Merge(CDaryPolyHeap<type>* tree1,CDaryPolyHeap<type>* tree2)
{
    CDaryPolyHeap<type> t1,t2;
    CDaryPolyHeap<type>* resHeap=nullptr;
    cmp_t cmpRes;
    if(tree1==nullptr||tree2==nullptr)
    {
        return nullptr;
    }

    if((tree1->m_deepth!=tree2->m_deepth)||(tree1->IsEmpty())||(tree2->IsEmpty()))
    {
        return nullptr;
    }

    resHeap=new CDaryPolyHeap<type>();

    t1=*tree1;    //Make a copy
    t2=*tree2;

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
Err_t CDaryPolyHeap<type>::GetRootElem(type* elem)
{
    if(elem==nullptr)
        return INVALIDE_PARAMET;

    if(this->IsEmpty())
        return OPERATOR_FAILED;

    m_cpyRoute(elem,&this->m_root->elem);

    return RETURN_SUCCESS;
}

template<class type>
CDaryPolyHeap<type>** CDaryPolyHeap<type>::DeleteMin(CDaryPolyHeap<type>* src, AM_U32* len,type* elem)
{
    AM_U32 length=0,idx;
    CDaryPolyHeap<type>** resForest;
    CDaryPolyHeap<type> tree;
    TreeNode_t<type>* current;
    TreeNode_t<type>* root;
    if((src==nullptr)||(len==nullptr))
        return nullptr;

    if(src->IsEmpty())
        return nullptr;

    tree=*src;

    if(elem!=nullptr)
    {
        tree.m_cpyRoute(elem,&tree.m_root->elem);
    }

    if(tree.m_root->firstChild==nullptr)
    {
        *len=0;
        return nullptr;
    }

    current=tree.m_root->firstChild;
    while(current!=nullptr)
    {
        length++;
        current=current->nextSibling;
    }

    resForest=new CDaryPolyHeap<type>*[length];
    for(idx=0; idx<length; idx++)
    {
        resForest[idx]=new CDaryPolyHeap<type>();
    }

    current=tree.m_root->firstChild;
    tree.m_root->firstChild=nullptr; //Break the link;
    for(idx=length-1; idx>=0&&current!=nullptr; idx--)
    {
        root=current;
        resForest[idx]->SetRoot(root);
        resForest[idx]->m_deepth=idx;
        current=current->nextSibling;
        root->nextSibling=nullptr;
    }
    *len=length;

    return resForest;
}

template<class type>
const CDaryPolyHeap<type>& CDaryPolyHeap<type>::operator=(const CDaryPolyHeap<type>& object)
{
    CTree<type>* m1=dynamic_cast<CTree<type>* >(this);
    *m1=object;
    m_deepth=object.m_deepth;

}

template<class type>
const CDaryPolyHeap<type>& CDaryPolyHeap<type>::operator=(CDaryPolyHeap<type>&& object)
{
    CTree<type>* m1=dynamic_cast<CTree<type>* >(this);
    *m1=std::move(object);

    m_deepth=object.m_deepth;

    object.m_deepth=0;
}

template<class type>
void CDaryPolyHeap<type>::increaseDeepth(void)
{
    m_deepth++;
}

template<class type>
void CDaryPolyHeap<type>::decreaseDeepth(void)
{
    m_deepth--;
}


#endif
