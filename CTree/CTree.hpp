#ifndef __CTREE_HPP__
#define __CTREE_HPP__

/******************************************
*
*   This file implements the normal tree
*   @Author: Amos.Zhu
*
******************************************/

#include "AmosType.hpp"
#include "CStack.hpp"
#include <iostream>

template<typename type>
struct TreeNode_t
{
    type elem;
    TreeNode_t<type>* firstChild;
    TreeNode_t<type>* nextSibling;
};


template<class type>
class CTree
{
public:
    CTree(void);
    CTree(void (*func)(type* src));
    CTree(const CTree<type>& object);
    CTree(CTree<type>&& object);
    ~CTree(void);

public:
    Err_t AddFirstChild(TreeNode_t<type>* pParent,TreeNode_t<type>* pChild);
    Err_t AddNextSibling(TreeNode_t<type>* pCurrent,TreeNode_t<type>* pNext);
    Err_t SetRoot(TreeNode_t<type>* pRoot);
    Err_t SetRoot(type* elem);
    void SetPrintFunc(void (*func)(type* src));
    void SetCopyFunc(void (*func)(type* dst,type* src));

    BOOL IsEmpty(void) const;
    void PreOrderTraversal(void);

public:
    const CTree<type>& operator=(const CTree<type>& object);
    const CTree<type>& operator=(CTree<type>&& object);



private:
    void preOrder(TreeNode_t<type>* current,AM_U32 height);
    void copyTree(TreeNode_t<type>** dst,TreeNode_t<type>* src);
    void destroy(TreeNode_t<type>** current);

protected:
    TreeNode_t<type>* m_root=nullptr;
    /*
    *   print pointer here
    */
    void (*m_printNode)(type* src)=nullptr;
    void (*m_cpyRoute)(type* dst,type* src)=nullptr;

};

template<class type>
CTree<type>::CTree(void)
{
    m_root=nullptr;
    m_printNode=nullptr;
    m_cpyRoute=nullptr;
}

template<class type>
CTree<type>::CTree(void (*func)(type* src))
{
    m_root=nullptr;
    m_cpyRoute=nullptr;
    m_printNode=func;
}

template<class type>
CTree<type>::CTree(const CTree<type>& object)
{
    if(object.IsEmpty())
    {
        m_root=nullptr;
        m_printNode=nullptr;
        m_cpyRoute=nullptr;
        return;
    }

    m_printNode=object.m_printNode;
    m_cpyRoute=object.m_cpyRoute;
    copyTree(&m_root,object.m_root);

    return;
}

template<class type>
CTree<type>::CTree(CTree<type>&& object)
{
    m_printNode=object.m_printNode;
    m_cpyRoute=object.m_cpyRoute;
    m_root=object.m_root;

    object.m_printNode=nullptr;
    object.m_root=nullptr;
    object.m_cpyRoute=nullptr;

    return;
}


template<class type>
CTree<type>::~CTree(void)
{
    destroy(&m_root);
    m_root=nullptr;
    m_printNode=nullptr;
    m_cpyRoute=nullptr;
}

template<class type>
void CTree<type>::SetPrintFunc(void (*func)(type* src))
{
    if(func==nullptr)
        return;

    m_printNode=func;
}

template<class type>
void CTree<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    if(func==nullptr)
        return;
    m_cpyRoute=func;
}

template<class type>
Err_t CTree<type>::SetRoot(TreeNode_t<type>* pRoot)
{
    if(pRoot==nullptr)
        return INVALIDE_PARAMET;

    m_root=pRoot;
    return RETURN_SUCCESS;
}

template<class type>
Err_t CTree<type>::SetRoot(type* elem)
{
    TreeNode_t<type>* newNode;

    if(elem==nullptr)
        return INVALIDE_PARAMET;

    newNode=new TreeNode_t<type>;
    newNode->firstChild=nullptr;
    newNode->nextSibling=nullptr;
    m_cpyRoute(&newNode->elem,elem);

    m_root=newNode;

    return RETURN_SUCCESS;
}


template<class type>
Err_t CTree<type>::AddFirstChild(TreeNode_t<type>* pParent,TreeNode_t<type>* pChild)
{
    if((pParent==nullptr)||(pChild==nullptr))
        return INVALIDE_PARAMET;

    pParent->firstChild=pChild;
    return RETURN_SUCCESS;
}

template<class type>
Err_t CTree<type>::AddNextSibling(TreeNode_t<type>* pCurrent,TreeNode_t<type>* pNext)
{
    if((pCurrent==nullptr)||(pNext==nullptr))
        return INVALIDE_PARAMET;

    pCurrent->nextSibling=pNext;
    return RETURN_SUCCESS;
}

template<class type>
BOOL CTree<type>::IsEmpty(void) const
{
    if(m_root==nullptr)
        return TRUE;

    return FALSE;
}

template<class type>
void CTree<type>::PreOrderTraversal(void)
{
    if(IsEmpty())
        return;

    preOrder(this->m_root,0);
}

template<class type>
void CTree<type>::preOrder(TreeNode_t<type>* current,AM_U32 height)
{
    AM_U32 count=0;
    AM_U32 nextHeight=0;
    if(current==nullptr)
        return;

    for(count=0; count<height; count++)
        std::cout<<"	";

    if(m_printNode!=nullptr)
    {
        m_printNode(&current->elem);
    }
    nextHeight=height+1;
    preOrder(current->firstChild,nextHeight);
    preOrder(current->nextSibling,height);

    return;
}

template<class type>
void CTree<type>::copyTree(TreeNode_t<type>** dst,TreeNode_t<type>* src)
{
    TreeNode_t<type>* newNode;
    if(src==nullptr)
    {
        *dst=nullptr;
        return;
    }
    *dst=new TreeNode_t<type>;
    m_cpyRoute(&(*dst)->elem,&src->elem);
    copyTree(&(*dst)->firstChild,src->firstChild);
    copyTree(&(*dst)->nextSibling,src->nextSibling);
}

template<class type>
void CTree<type>::destroy(TreeNode_t<type>** current)
{
    if((current==nullptr)||(*current==nullptr))
    {
        return;
    }

    destroy(&(*current)->firstChild);
    destroy(&(*current)->nextSibling);
    delete *current;
    *current=nullptr;
    return;
}

template<class type>
const CTree<type>& CTree<type>::operator=(const CTree<type>& object)
{
    if(this==&object)
        return *this;

    destroy(&m_root);

    if(object.IsEmpty())
    {
        m_root=nullptr;
        m_printNode=nullptr;
        m_cpyRoute=nullptr;
        return *this;
    }

    m_printNode=object.m_printNode;
    m_cpyRoute=object.m_cpyRoute;
    copyTree(&m_root,object.m_root);

    return *this;
}

template<class type>
const CTree<type>& CTree<type>::operator=(CTree<type>&& object)
{
    if(this==&object)
        return *this;

    destroy(&m_root);

    if(object.IsEmpty())
    {
        m_root=nullptr;
        m_printNode=nullptr;
        m_cpyRoute=nullptr;
        return *this;
    }

    m_printNode=object.m_printNode;
    m_cpyRoute=object.m_cpyRoute;
    m_root=object.m_root;

    object.m_printNode=nullptr;
    object.m_root=nullptr;
    object.m_cpyRoute=nullptr;

    return *this;
}

#endif
