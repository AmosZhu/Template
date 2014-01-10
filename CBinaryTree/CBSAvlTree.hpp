#ifndef _CBINARY_SEARCH_AVLTREE_HPP_
#define _CBINARY_SEARCH_AVLTREE_HPP_

/**********************************************************************
*
*   This file implement the template of Adelson-Velskii & Landis Tree
*   @Author: Amos.zhu
*
***********************************************************************/

#include "AmosType.hpp"
#include "CBSearchTree.hpp"
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
    nodeType<type>* insert(type elem,nodeType<type>** node);
    AM_S32 height(nodeType<type>* p);
};

template<class type>
Err_t CBSAvlTree<type>::Insert(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;
    if(insert(*elem,&this->m_root)==NULL)
        return OPERATOR_FAILED;

    return RETURN_SUCCESS;
}

/*
*   Not implement yet
*/

template<class type>
Err_t CBSAvlTree<type>::Delete(type* elem)
{

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

    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;
    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;
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

    k1->height=this->max(height(k1->lLink),height(k1->rLink))+1;
    k2->height=this->max(height(k2->lLink),height(k2->rLink))+1;

    return k1;
}

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


#endif
