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
    CTree(void (*func)(type));
    CTree(const CTree<type>* object);
    ~CTree(void);

public:
    Err_t AddFirstChild(TreeNode_t<type>* pParent,TreeNode_t<type>* pChild);
    Err_t AddNextSibling(TreeNode_t<type>* pCurrent,TreeNode_t<type>* pNext);
    Err_t SetRoot(TreeNode_t<type>* pRoot);
    void SetPrintFunc(void (*func)(type));

    BOOL IsEmpty(void) const;
    void PreOrderTraversal(void);

public:
    const CTree<type>& operator=(const CTree<type>& object);



private:
    void preOrder(TreeNode_t<type>* current,AM_U32 height);
    void copyTree(TreeNode_t<type>** dst,TreeNode_t<type>* src);
    void destroy(TreeNode_t<type>** current);

private:
    TreeNode_t<type>* m_root;
    /*
    *   print pointer here
    */
    void (*m_printNode)(type);

};

template<class type>
CTree<type>::CTree(void)
{
    m_root=NULL;
    m_printNode=NULL;
}

template<class type>
CTree<type>::CTree(void (*func)(type))
{
    m_root=NULL;
    m_printNode=func;
}

template<class type>
CTree<type>::CTree(const CTree<type>* object)
{
#if 0
    CStack<TreeNode_t<type> > srcStack,dstStack;
    TreeNode_t<type>* newNode;
    TreeNode_t<type>* src;
    TreeNode_t<type>* dst;
#endif /* Modify by Amos.zhu */

    if((object==NULL)||(object->IsEmpty()))
    {
        m_root=NULL;
        m_printNode=NULL;
        return;
    }

    m_printNode=object->m_printNode;
    copyTree(&m_root,object->m_root);

    return;
#if 0
    src=object->m_root;
    newNode=new TreeNode_t<type>;
    memcpy(newNode,src,sizeof(TreeNode_t<type>));
    newNode->firstChild=NULL;
    newNode->nextSibling=NULL;
    dst=this->m_root=newNode;

    srcStack.PushNoCopy(&src); /* Save the address of the pointer */
    dstStack.PushNoCopy(&dst);

    /*
    *   Begin Construct a tree
    */

    while(!srcStack.IsEmpty())
    {
        srcStack.PopNoCopy(&src);
        dstStack.PopNoCopy(&dst);

        if(src->nextSibling!=NULL)
        {
            newNode=new TreeNode_t<type>;
            memcpy(newNode,src->nextSibling,sizeof(TreeNode_t<type>));
            newNode->firstChild=NULL;
            newNode->nextSibling=NULL;
            dst->nextSibling=newNode;
            src=src->nextSibling;
            dst=dst->nextSibling;
        }


    }

#endif /* Modify by Amos.zhu */

}

template<class type>
CTree<type>::~CTree(void)
{
    destroy(&m_root);
    m_root=NULL;
    m_printNode=NULL;
}

template<class type>
void CTree<type>::SetPrintFunc(void (*func)(type))
{
    if(func==NULL)
        return;
    m_printNode=func;
}


template<class type>
Err_t CTree<type>::SetRoot(TreeNode_t<type>* pRoot)
{
    if(pRoot==NULL)
        return INVALIDE_PARAMET;

    m_root=pRoot;
    return RETURN_SUCCESS;
}


template<class type>
Err_t CTree<type>::AddFirstChild(TreeNode_t<type>* pParent,TreeNode_t<type>* pChild)
{
    if((pParent==NULL)||(pChild==NULL))
        return INVALIDE_PARAMET;

    pParent->firstChild=pChild;
    return RETURN_SUCCESS;
}

template<class type>
Err_t CTree<type>::AddNextSibling(TreeNode_t<type>* pCurrent,TreeNode_t<type>* pNext)
{
    if((pCurrent==NULL)||(pNext==NULL))
        return INVALIDE_PARAMET;

    pCurrent->nextSibling=pNext;
    return RETURN_SUCCESS;
}

template<class type>
BOOL CTree<type>::IsEmpty(void) const
{
    if(m_root==NULL)
        return TRUE;

    return FALSE;
}

template<class type>
void CTree<type>::PreOrderTraversal(void)
{
    if(IsEmpty())
        return;

    preOrder(m_root,0);
}

template<class type>
void CTree<type>::preOrder(TreeNode_t<type>* current,AM_U32 height)
{
    AM_U32 count=0;
    AM_U32 nextHeight=0;
    if(current==NULL)
        return;

    for(count=0; count<height; count++)
        std::cout<<"	";

    if(m_printNode!=NULL)
    {
        m_printNode(current->elem);
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
    if(src==NULL)
    {
        *dst=NULL;
        return;
    }
    *dst=new TreeNode_t<type>;
    memcpy(&(*dst)->elem,&src->elem,sizeof(type));
    copyTree(&(*dst)->firstChild,src->firstChild);
    copyTree(&(*dst)->nextSibling,src->nextSibling);
}

template<class type>
void CTree<type>::destroy(TreeNode_t<type>** current)
{
    if((current==NULL)||(*current==NULL))
    {
        return;
    }

    destroy(&(*current)->firstChild);
    destroy(&(*current)->nextSibling);
    delete *current;
    *current=NULL;
    return;
}

template<class type>
const CTree<type>& CTree<type>::operator=(const CTree<type>& object)
{
    if(this==&object)
        return *this;

    if(object.IsEmpty())
    {
        m_root=NULL;
        m_printNode=NULL;
        return *this;
    }

    m_printNode=object.m_printNode;
    copyTree(&m_root,object.m_root);

    return *this;
}


#endif
