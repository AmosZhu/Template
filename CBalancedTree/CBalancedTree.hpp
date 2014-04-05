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
#include "CStack.hpp"
#include <iostream>

#define BTREE_D 3
#define ORDER (BTREE_D*2-1)

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
    Err_t Delete(type* key);
    AM_U32 TreeDepth(void);

    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    void SetCopyFunc(void (*func)(type* dst,type* src));
    void SetClearFunc(void (*func)(type* src));
    void SetPrintFunc(void (*func)(type* src));
    void PrintTree(void);

private:
    void spiltChild(BTNode<type>* parent,AM_S32 position,BTNode<type>* child);
    void insertKey(BTNode<type>* node,type* key,CStack<BTNode<type>*>* stack);

    void deleteNodeFromTree(BTNode<type>* node, AM_S32 position,CStack<BTNode<type>*>* stack);
    void processNodeBoundary(BTNode<type>* node);
    void nodeMerge(BTNode<type>* node,AM_S32 position);
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
    BTNode<type>* current=NULL;
    CStack<BTNode<type>*> stack;
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
        stack.PushNoCopy(&m_root);
        spiltChild(m_root,0,newChild);
    }

    insertKey(m_root,key,&stack);

    /*
     *  Back trace the node to ensure the identity of BTree
     *  Note:@ Spilt first then insert the key
     *         and insert then spilt the node both need to back trace the node;
     *         Amos use spilt then insert that will easier to implement.
     *         But it will cost more because sometimes it will merge a
     *         node just spilt.
     */

    while(!stack.IsEmpty())
    {
        stack.PopNoCopy(&current);
        processNodeBoundary(current);
    }

    return RETURN_SUCCESS;
}

template<class type>
Err_t CBalancedTree<type>::Delete(type* key)
{
    CStack<BTNode<type>*> stack;
    BTNode<type>* current;
    AM_S32 i;
    cmp_t res;
    BOOL found=FALSE;

    if(key==NULL)
        return INVALIDE_PARAMET;

    if(m_root==NULL)
        return OPERATOR_FAILED;

    current=m_root;

    while(current!=NULL&&!found)
    {
        for(i=0; i<current->keynum; i++)
        {
            if((res=m_cmpRoute(key,&current->key[i]))==SMALLER)
            {
                break;
            }
            else if(res==EQUAL)
            {
                stack.PushNoCopy(&current);
                deleteNodeFromTree(current,i,&stack);
                found=TRUE;
            }
        }

        if(found==TRUE)
            continue;

        stack.PushNoCopy(&current);
        current=current->child[i];
    }

    if(found==FALSE)
    {
        std::cout<<"Key is not in the tree!"<<std::endl;
        return OPERATOR_FAILED;
    }

    while(!stack.IsEmpty())
    {
        stack.PopNoCopy(&current);
        processNodeBoundary(current);
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
void CBalancedTree<type>::insertKey(BTNode<type>* node,type* key,CStack<BTNode<type>*>* stack)
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
            stack->PushNoCopy(&node);
            spiltChild(node,i,node->child[i]);

            if(m_cmpRoute(&node->key[i],key)==SMALLER)
                i++;
        }

        insertKey(node->child[i],key,stack);
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
void CBalancedTree<type>::deleteNodeFromTree(BTNode<type>* node, AM_S32 position,CStack<BTNode<type>*>* stack)
{
    AM_S32 i;
    BTNode<type>* leftChild=NULL,*rightChild=NULL,*current=NULL;

    if(node==NULL)
        return;

    if(node->isLeaf)
    {
        for(i=position; i<node->keynum; i++)
        {
            m_cpyRoute(&node->key[i],&node->key[i+1]);
        }
        m_clearRoute(&node->key[node->keynum]);
        node->keynum--;

        return;
    }
    else
    {
        leftChild=node->child[position];
        rightChild=node->child[position+1];

        /*
        *   check left child or right child is rich enough?
        */

        if(rightChild->keynum>(BTREE_D-1))
        {
            /*
            *   If the right child is rich enough get first the key from the rightchild,
            *   and delete the key from the rightchild.
            */
            current=rightChild;
            while(current!=NULL)
            {
                rightChild=current;
                stack->PushNoCopy(&current);
                current=current->child[0];
            }
            m_cpyRoute(&node->key[position],&rightChild->key[0]);
            //stack->PushNoCopy(&rightChild);
            deleteNodeFromTree(rightChild,0,stack);
        }
        else
        {
            /*
            *   Get key from leftchild default;
            */
            current=leftChild;
            while(current!=NULL)
            {
                leftChild=current;
                stack->PushNoCopy(&current);
                current=current->child[current->keynum];
            }
            m_cpyRoute(&node->key[position],&leftChild->key[leftChild->keynum-1]);
            //stack->PushNoCopy(&leftChild);
            deleteNodeFromTree(leftChild,leftChild->keynum-1,stack);
        }

    }
}

template<class type>
void CBalancedTree<type>::processNodeBoundary(BTNode<type>* node)
{
    AM_S32 pos,i;
    BTNode<type>* leftChild=NULL,*rightChild=NULL,*poorChild=NULL;
    if(node==NULL)
        return;

    /*
    *   Leaf node no need to do this process
    */
    if(node->isLeaf)
        return;

    for(pos=0; pos<=node->keynum; pos++)
    {
        if(node->child[pos]->keynum<(BTREE_D-1))
            break;
    }

    /*
    *   Every child was good,return;
    */
    if(pos>node->keynum)
        return;

    /*
    *   the child[pos] was poor,go process
    */

    poorChild=node->child[pos];

    if(pos>0)
        leftChild=node->child[pos-1];

    if(pos<node->keynum)
        rightChild=node->child[pos+1];

    /*
    *   1.Once the node can merge,merge first.Or go to 2.
    */

    if((leftChild!=NULL)&&(poorChild->keynum+leftChild->keynum)<ORDER-1)
    {
        nodeMerge(node,pos);
        return;
    }
    else if((rightChild!=NULL)&&(poorChild->keynum+rightChild->keynum)<ORDER-1)
    {
        nodeMerge(node,pos+1);
        return;
    }
    /*
    *   2.If left child was exist and rich
    */

    if(leftChild!=NULL&&leftChild->keynum>(BTREE_D-1))
    {
        for(i=0; i<poorChild->keynum; i++)
        {
            m_cpyRoute(&poorChild->key[i+1],&poorChild->key[i]);
        }
        m_cpyRoute(&poorChild->key[0],&node->key[pos-1]);
        poorChild->keynum++;
        m_cpyRoute(&node->key[pos-1],&leftChild->key[leftChild->keynum-1]);
        deleteNodeFromTree(leftChild,leftChild->keynum-1,NULL);
    }
    /*
    *   3.The same process the right child
    */
    else if(rightChild!=NULL&&rightChild->keynum>(BTREE_D-1))
    {
        m_cpyRoute(&poorChild->key[poorChild->keynum],&node->key[pos]);
        poorChild->keynum++;
        m_cpyRoute(&node->key[pos],&rightChild->key[0]);
        deleteNodeFromTree(rightChild,0,NULL);
    }
    /*
    *   4.left & right child are both not rich
    */
    else
    {
        /*
        *   the poor node Merge with his sibling
        */
        if(leftChild!=NULL)
            nodeMerge(node,pos);
        else if(rightChild!=NULL)
            nodeMerge(node,pos+1);
    }
    return;
}

template<class type>
void CBalancedTree<type>::nodeMerge(BTNode<type>* node,AM_S32 position)
{
    BTNode<type>* leftChild=NULL,*rightChild=NULL;
    AM_S32 i;
    if(node==NULL)
        return;

    leftChild=node->child[position-1];
    rightChild=node->child[position];

    /*
    *   Get the key from the parent.
    */

    m_cpyRoute(&leftChild->key[leftChild->keynum],&node->key[position-1]);
    leftChild->child[leftChild->keynum+1]=rightChild->child[0];
    leftChild->keynum++;

    /*
    *   Copy the key from the right child.
    */

    for(i=0; i<rightChild->keynum; i++)
    {
        m_cpyRoute(&leftChild->key[leftChild->keynum],&rightChild->key[i]);
        leftChild->child[leftChild->keynum+1]=rightChild->child[i+1];
        leftChild->keynum++;
    }

    /*
    *   Adjust the elements in the node
    */

    for(i=position; i<node->keynum; i++)
    {
        m_cpyRoute(&node->key[i-1],&node->key[i]);
        node->child[i]=node->child[i+1];
    }

    m_clearRoute(&node->key[node->keynum-1]);
    node->child[node->keynum]=NULL;
    node->keynum--;

    /*
     *  The node is the root node,and the node has not any key.
     *  Move the root node to its left child,and release the node.
     */
    if(m_root->keynum==0)
    {
        m_root=leftChild;
        delete node;
        node=NULL;
    }

    delete rightChild;
    rightChild=NULL;

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
