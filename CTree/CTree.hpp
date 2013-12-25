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

    BOOL IsEmpty(void) const;
    void PreOrderTraversal(void);


private:
    void preOrder(TreeNode_t<type>* current,AM_U32 height);
    void copyTree(TreeNode_t<type>* dst,TreeNode_t<type>* src);

private:
    TreeNode_t<type>* root;
    /*
    *   print pointer here
    */
    void (*printNode)(type);

};

template<class type>
CTree<type>::CTree(void)
{
    root=NULL;
    printNode=NULL;
}

template<class type>
CTree<type>::CTree(void (*func)(type))
{
    root=NULL;
    printNode=func;
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
        root=NULL;
        printNode=NULL;
        return;
    }

    printNode=object->printNode;
    /*
    *   Process root first;
    */

    copyTree(root,object->root);

    return;
#if 0
    src=object->root;
    newNode=new TreeNode_t<type>;
    memcpy(newNode,src,sizeof(TreeNode_t<type>));
    newNode->firstChild=NULL;
    newNode->nextSibling=NULL;
    dst=this->root=newNode;

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

}

template<class type>
Err_t CTree<type>::SetRoot(TreeNode_t<type>* pRoot)
{
    if(pRoot==NULL)
        return INVALIDE_PARAMET;

    root=pRoot;
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
    if(root==NULL)
        return TRUE;

    return FALSE;
}

template<class type>
void CTree<type>::PreOrderTraversal(void)
{
    if(IsEmpty())
        return;

    preOrder(root,0);
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

    if(printNode!=NULL)
    {
        printNode(current->elem);
    }
    nextHeight=height+1;
    preOrder(current->firstChild,nextHeight);
    preOrder(current->nextSibling,height);

    return;
}

template<class type>
void CTree<type>::copyTree(TreeNode_t<type>* dst,TreeNode_t<type>* src)
{
    TreeNode_t<type>* newNode;
    if(src==NULL)
    {
        dst=NULL;
        return;
    }
    static int i=0;
    i++;
    printf("%d\n",i);
    if(printNode!=NULL)
        printNode(src->elem);
    dst=new TreeNode_t<type>;
    memcpy(&dst->elem,&src->elem,sizeof(type));
    copyTree(dst->firstChild,src->firstChild);
    copyTree(dst->nextSibling,src->nextSibling);
}


#endif
