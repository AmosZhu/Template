#ifndef _CBINARY_SEARCH_AVLTREE_HPP_
#define _CBINARY_SEARCH_AVLTREE_HPP_

/**********************************************************************
*
*   This file implement the template of Adelson-Velskii & Landis Tree
*   @Author: Amos.zhu
*
***********************************************************************/

#define DB_PRINT(fmt,args...) printf("\033[33m [%s][%d]"fmt"\033[0m",__FUNCTION__,__LINE__,##args)

#include "AmosType.hpp"
#include "CBSearchTree.hpp"
#include "CStack.hpp"
#include <string.h>
#include <iostream>

template<class type>
class CBSAvlTree:public CBSearchTree<type>
{

public:
    Err_t Insert(type* elem);
    Err_t Delete(type* elem);

private:
    nodeType<type>* singleRotationWithLeft(nodeType<type>* k2);
    nodeType<type>* singleRotationWithRight(nodeType<type>* k2);
    nodeType<type>* doubleRotationWithLeft(nodeType<type>* k3);
    nodeType<type>* doubleRotationWithRight(nodeType<type>* k3);
    /*************************************************
    *       recursive insert
    *************************************************/
    nodeType<type>* insert(type elem,nodeType<type>** node);
    /*************************************************
    *       non-recursive insert
    *       excersise:4-19
    *************************************************/
    nodeType<type>* insert(type elem);
    void deleteFromTree(nodeType<type>** p,CStack<nodeType<type>* >* nodeStack,type* elem,BOOL* isDelay);

    AM_S32 height(nodeType<type>* p);

};

template<class type>
Err_t CBSAvlTree<type>::Insert(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;
#if 1
    if(insert(*elem,&this->m_root)==NULL)
        return OPERATOR_FAILED;
#endif /* Modify by Amos.zhu */

    /*
    *   Test non-recursive insert function
    */

#if 0
    if(insert(*elem)==NULL)
        return OPERATOR_FAILED;
#endif

    return RETURN_SUCCESS;
}

/******************************************
*
*   Non-lazy delete implement
*
*   exercise: 4-20
*
******************************************/

template<class type>
Err_t CBSAvlTree<type>::Delete(type* elem)
{

    printf("CBSAvlTree::%s\n",__FUNCTION__);
    cmp_t r1,r2;
    nodeType<type>* current,*tailNode,*p1,*p2;
    CStack<nodeType<type>* > nodeStack; /*Use for the backtracking the node*/
    type deleteElem;
    BOOL delayReplace=FALSE;
    BOOL found=FALSE;


    if(elem==NULL)
        return INVALIDE_PARAMET;

    memcpy(&deleteElem,elem,sizeof(type));

    /*
    *   Firstly,find out the position in the tree
    */

    if(this->m_root==NULL)
    {
        std::cout<<"This tree is empty!"<<std::endl;
        return OPERATOR_FAILED;
    }

    current=this->m_root;
    tailNode=NULL;
    while((current!=NULL)&&(found!=TRUE))
    {
        if((r1=this->m_cmpRoute(&deleteElem,&current->elem))==EQUAL)
        {
            found=TRUE;
            continue;
        }

        tailNode=current;
        nodeStack.PushNoCopy(&current);
        if(r1==LARGER)
        {
            current=current->rLink;
        }
        else if(r1==SMALLER)
        {
            current=current->lLink;
        }
    }

    if((current==NULL)||(found!=TRUE))
    {
        std::cout<<"Element not find in the tree!"<<std::endl;
        return OPERATOR_FAILED;
    }

    /*
    *   Secondly, delete the element from the tree
    */

    if(tailNode!=NULL)
    {
        DB_PRINT("\n");
        if((r1=this->m_cmpRoute(&deleteElem,&tailNode->elem))==LARGER)
            deleteFromTree(&tailNode->rLink,&nodeStack,&deleteElem,&delayReplace);
        else if(r1==SMALLER)
            deleteFromTree(&tailNode->lLink,&nodeStack,&deleteElem,&delayReplace);
    }
    else
    {
        DB_PRINT("\n");
        deleteFromTree(&this->m_root,&nodeStack,&deleteElem,&delayReplace);
    }

    /*
    *   Thirdly, backtracing the node to update its height and do rotation
    */

    if(!nodeStack.IsEmpty())
    {
        nodeStack.PopNoCopy(&p2);
    }

    while(!nodeStack.IsEmpty())
    {
        p1=p2;
        nodeStack.PopNoCopy(&p2);
        p1->height=this->max(height(p1->lLink),height(p1->rLink))+1;
        if((r1=this->m_cmpRoute(&deleteElem,&p1->elem))==SMALLER)
        {
            if((height(p1->rLink)-height(p1->lLink))==2)
            {
                if(height(p1->rLink->lLink)>height(p1->rLink->rLink))
                {
                    if((r2=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=doubleRotationWithLeft(p1);
                    else if(r2==SMALLER)
                        p2->lLink=doubleRotationWithLeft(p1);
                }
                else
                {
                    if((r2=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=singleRotationWithRight(p1);
                    else if(r2==SMALLER)
                        p2->lLink=singleRotationWithRight(p1);
                }
            }
        }
        else if(r1==LARGER)
        {
            if((height(p1->lLink)-height(p1->rLink))==2)
            {
                if(height(p1->lLink->lLink)>height(p1->lLink->rLink))
                {
                    if((r2=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=singleRotationWithLeft(p1);
                    else if(r2==SMALLER)
                        p2->lLink=singleRotationWithLeft(p1);

                }
                else
                {
                    if((r2=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=doubleRotationWithRight(p1);
                    else if(r2==SMALLER)
                        p2->lLink=doubleRotationWithRight(p1);
                }
            }
        }
    }

    /*
    *   Process the last node p2 also is the root:
    */

    p2->height=this->max(height(p2->lLink),height(p2->rLink))+1;
    if((r1=this->m_cmpRoute(&deleteElem,&p2->elem))==SMALLER)
    {
        if((height(p2->rLink)-height(p2->lLink))==2)
        {
            if(height(p2->rLink->lLink)>height(p2->rLink->rLink))
            {
                this->m_root=doubleRotationWithLeft(p2);
            }
            else
            {
                this->m_root=singleRotationWithRight(p2);
            }
        }
    }
    else if(r1==LARGER)
    {
        if((height(p2->lLink)-height(p2->rLink))==2)
        {
            if(height(p2->lLink->lLink)>height(p2->lLink->rLink))
            {
                this->m_root=singleRotationWithLeft(p2);
            }
            else if(r2==SMALLER)
            {
                this->m_root=doubleRotationWithRight(p2);
            }
        }
    }

    /*
    *   Lastly, if the delete node have the left and right child,replay it with the deleteElem
    */

    if(delayReplace==TRUE)
    {
        memcpy(&current->elem,&deleteElem,sizeof(type));
    }

    return RETURN_SUCCESS;
}

template<class type>
AM_S32 CBSAvlTree<type>::height(nodeType<type>* p)
{
    if(p==NULL)
        return -1;

    return p->height;
}


template<class type>
nodeType<type>* CBSAvlTree<type>::insert(type elem,nodeType<type>** node)
{
    cmp_t result;
    if(*node==NULL)
    {
        *node = new nodeType<type>;
        memcpy(&(*node)->elem,&elem,sizeof(type));
        (*node)->lLink=NULL;
        (*node)->rLink=NULL;
        (*node)->height=0;
    }
    else if((result=this->m_cmpRoute(&elem,&(*node)->elem))==SMALLER)
    {
        (*node)->lLink=insert(elem,&((*node)->lLink));
        if((height((*node)->lLink)-height((*node)->rLink))==2)
        {
            if(this->m_cmpRoute(&elem,&(*node)->lLink->elem)==SMALLER)
                *node=singleRotationWithLeft(*node);
            else
                *node=doubleRotationWithRight(*node);
        }
    }
    else if(result==LARGER)
    {
        (*node)->rLink=insert(elem,&((*node)->rLink));
        if((height((*node)->rLink)-height((*node)->lLink))==2)
        {
            if(this->m_cmpRoute(&elem,&(*node)->rLink->elem)==LARGER)
                *node=singleRotationWithRight(*node);
            else
                *node=doubleRotationWithLeft(*node);
        }
    }
    else if(result==EQUAL)
    {
        std::cout<<"elem is in the AVL tree!"<<std::endl;
        return *node;
    }
    (*node)->height=this->max(height((*node)->lLink),height((*node)->rLink))+1;
    return *node;
}


/******************************************
*
*   Non-recursive implement
*
*   exercise: 4-19
*
******************************************/

template<class type>
nodeType<type>* CBSAvlTree<type>::insert(type elem)
{
    nodeType<type>* newNode,*current,*tailNode,*temp;
    nodeType<type>* p1,*p2; /*Using to store the address of the node*/
    CStack<nodeType<type>* > nodeStack; /*Use for the backtracking the node*/

    cmp_t result,r1,r2,r3;

    /*
     *  Insert root firstly;
     */

    if(this->m_root==NULL)
    {
        this->m_root=new nodeType<type>;
        this->m_root->lLink=NULL;
        this->m_root->rLink=NULL;
        memcpy(&this->m_root->elem,&elem,sizeof(type));
        this->m_root->height=0;
        return this->m_root;
    }

    /*
    *   Search from the root
    */

    current=this->m_root;
    while(current!=NULL)
    {
        tailNode=current;
        if((result=this->m_cmpRoute(&elem,&current->elem))==LARGER)
        {
            nodeStack.PushNoCopy(&current);
            current=current->rLink;
        }
        else if(result==SMALLER)
        {
            nodeStack.PushNoCopy(&current);
            current=current->lLink;
        }
        else if((result==EQUAL)||(result==INVALID))
        {
            return NULL;
        }
    }

    /*
    *   Insert the new node;
    */

    newNode=new nodeType<type>;
    newNode->lLink=NULL;
    newNode->rLink=NULL;
    newNode->height=0;
    memcpy(&newNode->elem,&elem,sizeof(type)); /* always use copy */

    if((result=this->m_cmpRoute(&elem,&tailNode->elem))==LARGER)
        tailNode->rLink=newNode;
    else if(result==SMALLER)
        tailNode->lLink=newNode;
    else
    {
        std::cout<<"Node is already in the tree!"<<std::endl;
    }

    /*
    *   Backtracking the node in order to update its height and do rotations
    */

    if(!nodeStack.IsEmpty())
    {
        nodeStack.PopNoCopy(&p2);
    }

    while(!nodeStack.IsEmpty())
    {
        p1=p2;
        nodeStack.PopNoCopy(&p2);
        p1->height=this->max(height(p1->lLink),height(p1->rLink))+1;
        if((r1=this->m_cmpRoute(&elem,&p1->elem))==LARGER)
        {
            if((height(p1->rLink)-height(p1->lLink))==2)
            {
                if((r2=this->m_cmpRoute(&elem,&p1->rLink->elem))==LARGER)
                {
                    if((r3=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=singleRotationWithRight(p1);
                    else if(r3==SMALLER)
                        p2->lLink=singleRotationWithRight(p1);
                }
                else if(r2==SMALLER)
                {
                    if((r3=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=doubleRotationWithLeft(p1);
                    else if(r3==SMALLER)
                        p2->lLink=doubleRotationWithLeft(p1);
                }
            }
        }
        else if(r1==SMALLER)
        {
            if((height(p1->lLink)-height(p1->rLink))==2)
            {
                if((r2=this->m_cmpRoute(&elem,&p1->lLink->elem))==LARGER)
                {
                    if((r3=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=doubleRotationWithRight(p1);
                    else if(r3==SMALLER)
                        p2->lLink=doubleRotationWithRight(p1);
                }
                else if(r2==SMALLER)
                {
                    if((r3=this->m_cmpRoute(&p1->elem,&p2->elem))==LARGER)
                        p2->rLink=singleRotationWithLeft(p1);
                    else if(r3==SMALLER)
                        p2->lLink=singleRotationWithLeft(p1);
                }
            }
        }
    }

    /*
    *   Process the last node p2 also is the root:
    */

    p2->height=this->max(height(p2->lLink),height(p2->rLink))+1;
    if((r1=this->m_cmpRoute(&elem,&p2->elem))==LARGER)
    {
        if((height(p2->rLink)-height(p2->lLink))==2)
        {
            if((r2=this->m_cmpRoute(&elem,&p2->rLink->elem))==LARGER)
            {
                this->m_root=singleRotationWithRight(p2);
            }
            else if(r2==SMALLER)
            {
                this->m_root=doubleRotationWithLeft(p2);
            }
        }
    }
    else if(r1==SMALLER)
    {
        if((height(p2->lLink)-height(p2->rLink))==2)
        {
            if((r2=this->m_cmpRoute(&elem,&p2->lLink->elem))==LARGER)
            {
                this->m_root=doubleRotationWithRight(p2);
            }
            else if(r2==SMALLER)
            {
                this->m_root=singleRotationWithLeft(p2);
            }
        }
    }

    return this->m_root;
}


template<class type>
nodeType<type>* CBSAvlTree<type>::singleRotationWithLeft(nodeType<type>* k2)
{
    nodeType<type>* k1;

    if(k2==NULL)
        return NULL;

    k1=k2->lLink;
    k2->lLink=k1->rLink;
    k1->rLink=k2;

    printf("[%s]:",__FUNCTION__);
    this->m_printRoute(k2->elem);
    printf(" & ");
    this->m_printRoute(k1->elem);
    printf("\n");

    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;
    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;
    return k1;
}

template<class type>
nodeType<type>* CBSAvlTree<type>::singleRotationWithRight(nodeType<type>* k2)
{
    nodeType<type>* k1;

    if(k2==NULL)
        return NULL;

    k1=k2->rLink;
    k2->rLink=k1->lLink;
    k1->lLink=k2;

    printf("[%s]:",__FUNCTION__);
    this->m_printRoute(k2->elem);
    printf(" & ");
    this->m_printRoute(k1->elem);
    printf("\n");

    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;
    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;

    return k1;
}

#if 0
template<class type>
nodeType<type>* CBSAvlTree<type>::doubleRotationWithLeft(nodeType<type>* k3)
{
    nodeType<type>* k2;

    if(k3==NULL)
        return NULL;

    k2=k3->rLink;
    k3->rLink=singleRotationWithLeft(k2);

    return singleRotationWithRight(k3);
}

template<class type>
nodeType<type>* CBSAvlTree<type>::doubleRotationWithRight(nodeType<type>* k3)
{
    nodeType<type>* k2;

    if(k3==NULL)
        return NULL;

    k2=k3->lLink;
    k3->lLink=singleRotationWithRight(k2);

    return singleRotationWithLeft(k3);
}
#endif /* Modify by Amos.zhu */

template<class type>
nodeType<type>* CBSAvlTree<type>::doubleRotationWithLeft(nodeType<type>* k3)
{
    nodeType<type>* k2,*k1;

    if(k3==NULL)
        return NULL;

    k2=k3->rLink;
    k1=k2->lLink;

    k2->lLink=k1->rLink;
    k1->rLink=k2;

    k3->rLink=k1->lLink;
    k1->lLink=k3;

    k3->height=this->max(height(k3->lLink),height(k3->rLink))+1;
    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;
    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;
    return k1;
}

template<class type>
nodeType<type>* CBSAvlTree<type>::doubleRotationWithRight(nodeType<type>* k3)
{
    nodeType<type>* k2,*k1;

    if(k3==NULL)
        return NULL;

    k2=k3->lLink;
    k1=k2->rLink;

    k2->rLink=k1->lLink;
    k1->lLink=k2;

    k3->lLink=k1->rLink;
    k1->rLink=k3;

    k3->height=this->max(height(k3->lLink),height(k3->rLink))+1;
    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;
    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;

    return k1;
}

template<class type>
void CBSAvlTree<type>::deleteFromTree(nodeType<type>** p,CStack<nodeType<type>* >* nodeStack,type* elem,BOOL* isDelay)
{
    nodeType<type>* temp;
    nodeType<type>* tailNode;
    nodeType<type>* current;

    if((p==NULL)||(*p==NULL)||(nodeStack==NULL))
        return;

    if(((*p)->rLink==NULL)&&((*p)->lLink==NULL))
    {
        temp=*p;
        delete temp;
        *p=NULL;
    }
    else if((*p)->rLink==NULL)
    {
        temp=*p;
        *p=(*p)->lLink;
        delete temp;
    }
    else if((*p)->lLink==NULL)
    {
        temp=*p;
        *p=(*p)->rLink;
        delete temp;
    }
    else
    {
        /*
        *   Look for the largest element from its left child to replace itself
        */
        current=(*p)->lLink;
        nodeStack->PushNoCopy(p);
        tailNode=NULL;
        while(current->rLink!=NULL)
        {
            nodeStack->PushNoCopy(&current);
            tailNode=current;
            current=current->rLink;
        }
        *isDelay=TRUE;
        //memcpy(&(*p)->elem,&current->elem,sizeof(type));
        if(tailNode==NULL)
            (*p)->lLink=current->lLink;
        else
            tailNode->rLink=current->lLink;

        memcpy(elem,&current->elem,sizeof(type));
        delete current;
    }
}


#endif
