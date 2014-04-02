#ifndef _CBALANCED_TREE_HPP_
#define _CBALANCED_TREE_HPP_

/**************************************************************************
*
*   B-Tree Implement template
*   @Author: Amos.Zhu
*   @Reference: http://blog.csdn.net/v_july_v/article/details/6735293
*   @Reference: $July
*
**************************************************************************/

#include "AmosType.hpp"
#include <iostream>

#define BTREE_D 2
#define ORDER BTREE_D*2

template<typename type>
struct BTNode
{
    AM_U32 keynum;
    type key[ORDER-1];
    struct BTNode* child[ORDER];
    BOOL isLeaf;
};

template<class type>
class CBalancedTree
{
public:
    CBalancedTree();
    ~CBalancedTree();
    Err_t Insert(type* key);
    AM_U32 TreeDepth(void);

    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    void SetCopyFunc(void (*func)(type* dst,type* src));
    void SetClearFunc(void (*func)(type* src));
    void SetPrintFunc(void (*func)(type* src));
    void PrintTree(void);

private:
    void spiltChild(BTNode<type>* parent,AM_S32 position,BTNode<type>* child);
    void insertKey(BTNode<type>* node,type* key);

    void destroy(BTNode<type>* node);
    void print(BTNode<type>* node);
    AM_U32 depth(BTNode<type>* node);

private:
    BTNode<type>* m_root;
    cmp_t (*m_cmpRoute)(type* k1,type* k2);
    void (*m_cpyRoute)(type* dst,type* src); //Copy function for the node,set by different type;
    void (*m_clearRoute)(type* src); //Clear function for the node
    void (*m_printRoute)(type* src);
};

template<class type>
CBalancedTree<type>::CBalancedTree()
{
    m_root=NULL;
    m_cmpRoute=NULL;
    m_cpyRoute=NULL;
    m_clearRoute=NULL;
    m_printRoute=NULL;
}

template<class type>
CBalancedTree<type>::~CBalancedTree()
{
    destroy(m_root);
}

template<class type>
void CBalancedTree<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    m_cmpRoute=func;
}

template<class type>
void CBalancedTree<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    m_cpyRoute=func;
}

template<class type>
void CBalancedTree<type>::SetClearFunc(void (*func)(type* src))
{
    m_clearRoute=func;
}

template<class type>
void CBalancedTree<type>::SetPrintFunc(void (*func)(type* src))
{
    m_printRoute=func;
}

template<class type>
Err_t CBalancedTree<type>::Insert(type* key)
{

    BTNode<type>* newNode=NULL;
    BTNode<type>* newChild=NULL;
    AM_U32 i;

    if(key==NULL)
        return INVALIDE_PARAMET;

    if(m_root==NULL)
    {
        m_root=new BTNode<type>;
        m_root->keynum=1;
        m_root->isLeaf=TRUE;
        m_cpyRoute(&m_root->key[0],key);
        for(i=0; i<ORDER; i++)
        {
            m_root->child[i]=NULL;
        }
        return RETURN_SUCCESS;
    }

    /*
    *   If the root node is full,spilt first,then try to add the new key to its child
    */
    if(m_root->keynum==ORDER-1)
    {
        newChild=m_root;

        newNode=new BTNode<type>;
        newNode->keynum=0;
        newNode->isLeaf=FALSE;
        for(i=0; i<ORDER; i++)
        {
            newNode->child[i]=NULL;
        }

        newNode->child[0]=newChild;
        m_root=newNode;
        spiltChild(m_root,0,newChild);

        insertKey(m_root,key);
    }
    else
    {
        insertKey(m_root,key);
    }

    return RETURN_SUCCESS;
}

template<class type>
AM_U32 CBalancedTree<type>::TreeDepth(void)
{
    return depth(m_root);
}

template<class type>
void CBalancedTree<type>::PrintTree(void)
{
    print(m_root);
    std::cout<<std::endl;
}

template<class type>
void CBalancedTree<type>::spiltChild(BTNode<type>* parent,AM_S32 position,BTNode<type>* kid)
{
    BTNode<type>* newNode=NULL;
    AM_S32 i;

    if(parent==NULL||kid==NULL)
        return;

    newNode=new BTNode<type>;
    for(i=0; i<ORDER; i++)
    {
        newNode->child[i]=NULL;
    }
    newNode->isLeaf=kid->isLeaf;
    newNode->keynum=ORDER/2-1;

    /*
    *   copy half-1 of the child's key to newnode
    */

    for(i=0; i<newNode->keynum; i++)
    {
        m_cpyRoute(&newNode->key[i],&kid->key[i+BTREE_D]);
        m_clearRoute(&kid->key[i+BTREE_D]);
    }

    /*
    *   If the child is not the leaf,copy its child pointer
    */

    if(!newNode->isLeaf)
    {
        for(i=0; i<ORDER/2; i++)
        {
            newNode->child[i]=kid->child[i+BTREE_D];
            kid->child[i+BTREE_D]=NULL;
        }
    }

    /*
    *   Adjust parent key and child pointer
    */

    for(i=parent->keynum; i>position; i--)
    {
        parent->child[i+1]=parent->child[i];
    }

    parent->child[position+1]=newNode;

    for(i=parent->keynum-1; i>=position; i--)
    {
        m_cpyRoute(&parent->key[i+1],&parent->key[i]);
    }

    m_cpyRoute(&parent->key[position],&kid->key[BTREE_D-1]);
    parent->keynum++;

    m_clearRoute(&kid->key[BTREE_D-1]);
    kid->keynum=BTREE_D-1;
}

template<class type>
void CBalancedTree<type>::insertKey(BTNode<type>* node,type* key)
{
    AM_S32 i;
    AM_U32 pos;
    if(node==NULL||key==NULL)
        return;

    /*
    *   If node is a leaf,insert directly.
    *   Because the leaf node is not a full node.
    *   Before insert a key,it will spilt a full node first.
    *   So do not judge the keynum again.
    */

    if(node->isLeaf)
    {
        i=node->keynum-1;
        while((i>=0)&&(m_cmpRoute(&node->key[i],key)==LARGER))
        {
            m_cpyRoute(&node->key[i+1],&node->key[i]);
            i--;
        }
        m_cpyRoute(&node->key[i+1],key);
        node->keynum++;
    }
    else
    {
        for(i=0; i<node->keynum; i++)
        {
            if(m_cmpRoute(&node->key[i],key)==LARGER)
                break;
        }


        if(node->child[i]->keynum==ORDER-1)
        {
            spiltChild(node,i,node->child[i]);

            if(m_cmpRoute(&node->key[i],key)==SMALLER)
                i++;
        }

        insertKey(node->child[i],key);
    }
}

template<class type>
void CBalancedTree<type>::destroy(BTNode<type>* node)
{
    AM_U32 i;

    if(node==NULL)
        return;

    for(i=0; i<=node->keynum; i++)
    {
        if(node->child[i]!=NULL)
            destroy(node->child[i]);
    }

    delete node;
    node=NULL;

    return;
}

template<class type>
void CBalancedTree<type>::print(BTNode<type>* node)
{
    AM_U32 i;
    if(node==NULL)
        return;

    for(i=0; i<node->keynum; i++)
    {
        print(node->child[i]);
        m_printRoute(&node->key[i]);
    }

    print(node->child[i]);
}

template<class type>
AM_U32 CBalancedTree<type>::depth(BTNode<type>* node)
{
    AM_U32 height=0;
    BTNode<type>* current;
    if(node==NULL)
        return 0;

    current=node;

    while(current!=NULL)
    {
        height++;
        current=current->child[0];
    }

    return height;
}

#endif
