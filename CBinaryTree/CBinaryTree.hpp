#ifndef _CBINARY_TREE_H_
#define _CBINARY_TREE_H_

/******************************************************
*
*   BinaryTree template
*   @Author: Amos.zhu
*
*******************************************************/

#include <iostream>
#include "AmosType.hpp"

template<typename type>
struct nodeType
{
    type elem;
    nodeType<type>* lLink;
    nodeType<type>* rLink;
};

template<class type>
class CBinaryTree
{
public:
    CBinaryTree(void);
    CBinaryTree(const CBinaryTree<type>& otherTree);
    virtual ~CBinaryTree(void);

    BOOL IsEmpty(void);

    void PreorderTraversal(void);
    void InorderTraversal(void);
    void PostorderTraversal(void);
    AM_U32 TreeHeight(void);
    AM_U32 TreeNodeCount(void);
    AM_U32 TreeleavesCount(void);
    void DestroyTree(void);
    void SetPrintFunc(void (*func)(type));


public:
    const CBinaryTree<type>& operator=(const CBinaryTree<type>& otherTree);

private:
    void copyTree(nodeType<type>* &copiedTreeNode,nodeType<type>* otherTreeNode);
    void destroy(nodeType<type>* &p);
    void inorder(nodeType<type>* p);
    void preorder(nodeType<type>* p);
    void postorder(nodeType<type>* p);
    AM_U32 height(nodeType<type>* p);
    AM_U32 max(AM_U32 x,AM_U32 y);
    AM_U32 nodeCount(nodeType<type>* p);
    AM_U32 leavesCount(nodeType<type>* p);


protected:
    nodeType<type>* m_root;
    void (*m_printFunc)(type);

};


template<class type>
CBinaryTree<type>::CBinaryTree(void)
{
    m_root=NULL;
    m_printFunc=NULL;
}

template<class type>
CBinaryTree<type>::CBinaryTree(const CBinaryTree<type>& otherTree)
{
    if(otherTree.m_root==NULL)
    {
        m_root=NULL;
        return;
    }
    m_printFunc=otherTree.m_printFunc;
    copyTree(m_root,otherTree);
}

template<class type>
CBinaryTree<type>::~CBinaryTree(void)
{
    destroy(m_root);
    m_root=NULL;
    m_printFunc=NULL;
}

template<class type>
const CBinaryTree<type>& CBinaryTree<type>::operator=(const CBinaryTree<type>& otherTree)
{
    if(this==otherTree)
        return this;

    if(m_root!=NULL)
        destroy(m_root);

    if(otherTree.m_root==NULL)
    {
        m_root=NULL;
        return;
    }

    m_printFunc=otherTree.m_printFunc;
    copyTree(m_root,otherTree);
}


template<class type>
BOOL CBinaryTree<type>::IsEmpty(void)
{
    return(m_root==NULL);
}

template<class type>
void CBinaryTree<type>::PreorderTraversal(void)
{
    std::cout<<"Preorder: ";
    preorder(m_root);
    std::cout<<std::endl;
}

template<class type>
void CBinaryTree<type>::InorderTraversal(void)
{
    std::cout<<"Inorder: ";
    inorder(m_root);
    std::cout<<std::endl;
}

template<class type>
void CBinaryTree<type>::PostorderTraversal(void)
{
    std::cout<<"Postorder: ";
    postorder(m_root);
    std::cout<<std::endl;
}

template<class type>
AM_U32 CBinaryTree<type>::TreeHeight(void)
{
    return height(m_root);
}

template<class type>
AM_U32 CBinaryTree<type>::TreeNodeCount(void)
{
    return nodeCount(m_root);
}

template<class type>
AM_U32 CBinaryTree<type>::TreeleavesCount(void)
{
    return leavesCount(m_root);
}

template<class type>
void CBinaryTree<type>::DestroyTree(void)
{
    destroy(m_root);
}

template<class type>
void CBinaryTree<type>::SetPrintFunc(void (*func)(type))
{
    m_printFunc=func;
}


template<class type>
void CBinaryTree<type>::copyTree(nodeType<type>* &copiedTreeNode,nodeType<type>* otherTreeNode)
{
    if(otherTreeNode==NULL)
    {
        copiedTreeNode=NULL;
        return;
    }


    copiedTreeNode=new nodeType<type>;
    copiedTreeNode->info=otherTreeNode->info;
    copyTree(copiedTreeNode->lLink,otherTreeNode->lLink);
    copyTree(copiedTreeNode->rLink,otherTreeNode->rLink);

    return;
}

template<class type>
void CBinaryTree<type>::destroy(nodeType<type>* &p)
{
    if(p==NULL)
        return;

    destroy(p->lLink);
    destroy(p->rLink);
    delete p;
    p=NULL;
    return;
}

template<class type>
void CBinaryTree<type>::inorder(nodeType<type>* p)
{
    if(p==NULL)
        return;

    inorder(p->lLink);
    if(m_printFunc!=NULL)
        m_printFunc(p->elem);
    inorder(p->rLink);
    return;
}

template<class type>
void CBinaryTree<type>::preorder(nodeType<type>* p)
{
    if(p==NULL)
        return;

    if(m_printFunc!=NULL)
        m_printFunc(p->elem);
    preorder(p->lLink);
    preorder(p->rLink);
    return;
}

template<class type>
void CBinaryTree<type>::postorder(nodeType<type>* p)
{
    if(p==NULL)
        return;

    postorder(p->lLink);
    postorder(p->rLink);
    if(m_printFunc!=NULL)
        m_printFunc(p->elem);

    return;
}

template<class type>
AM_U32 CBinaryTree<type>::height(nodeType<type>* p)
{
    if(p==NULL)
        return 0;

    return 1+max(height(p->lLink),height(p->rLink));
}

template<class type>
AM_U32 CBinaryTree<type>::max(AM_U32 x,AM_U32 y)
{
    return x?y:x>=y;
}

template<class type>
AM_U32 CBinaryTree<type>::nodeCount(nodeType<type>* p)
{
    if(p==NULL)
        return 0;
    return 1+nodeCount(p->lLink)+nodeCount(p->rLink);
}

template<class type>
AM_U32 CBinaryTree<type>::leavesCount(nodeType<type>* p)
{
    if(p==NULL)
        return 0;

    if(p->rLink==NULL&&p->lLink==NULL)
        return 1;

    return leavesCount(p->rLink)+leavesCount(p->lLink);
}

#endif

