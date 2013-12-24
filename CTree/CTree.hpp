#ifndef __CTREE_HPP__
#define __CTREE_HPP__

/******************************************
*
*   This file implements the normal tree
*   @Author: Amos.Zhu
*
******************************************/

#include "AmosType.hpp"
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

    BOOL IsEmpty(void);
    void PreOrderTraversal(void);


private:
    void preOrder(TreeNode_t<type>* current,AM_U32 height);

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
    /*
    *   Not implement yet
    */
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
BOOL CTree<type>::IsEmpty(void)
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

    for(count=0;count<height;count++)
        std::cout<<"	";

	if(printNode!=NULL)
	{
		//printf("height:%d:",height);
		printNode(current->elem);
	}
	nextHeight=height+1;
    preOrder(current->firstChild,nextHeight);
    preOrder(current->nextSibling,height);

    return;
}


#endif
