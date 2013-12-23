#ifndef __CTREE_HPP__
#define __CTREE_HPP__

/******************************************
*
*   This file implements the normal tree
*   @Author: Amos.Zhu
*
******************************************/

#include "AmosType.hpp"

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
    CTree(void (*func)(TreeNode_t<type>));
    CTree(const CTree<type>* object);
    ~CTree(void);

public:
    Err_t AddFirstChild(TreeNode_t<type>* pParent,TreeNode_t<type>* pChild);
    Err_t AddNextSibling(TreeNode_t<type>* pCurrent,TreeNode_t<type>* pNext);
    Err_t SetRoot(TreeNode_t<type>* pRoot);

private:
    TreeNode_t<type>* root;
    /*
    *   print pointer here
    */
    void (*printNode)(TreeNode_t<type>);

};

template<class type>
CTree<type>::CTree(void)
{
    root=NULL;
    printNode=NULL;
}

template<class type>
CTree<type>::CTree(void (*func)(TreeNode_t<type>))
{
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

#endif
