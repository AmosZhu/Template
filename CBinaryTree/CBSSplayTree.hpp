#ifndef __CBSEARCH_SPLAYTREE_HPP__
#define __CBSEARCH_SPLAYTREE_HPP__

#include "CBSearchTree.hpp"


template<class type>
class CBSSplayTree:public CBSearchTree<type>
{
public:
    nodeType<type>* Find(type* item);
    Err_t Delete(type* item);
private:
    nodeType<type>* find(type* elem,nodeType<type>** node);
    nodeType<type>* findWithNode(type* elem,nodeType<type>** node);
    nodeType<type>* leftOnlyRotation(nodeType<type>* k3);
    nodeType<type>* rightOnlyRotation(nodeType<type>* k3);
    nodeType<type>* leftZigzagRotation(nodeType<type>* k3);
    nodeType<type>* rightZigzagRotation(nodeType<type>* k3);
    nodeType<type>* singleRotationWithRight(nodeType<type>* k2);
    nodeType<type>* singleRotationWithLeft(nodeType<type>* k2);
};


template<class type>
nodeType<type>* CBSSplayTree<type>::Find(type* item)
{
    if(item==NULL)
        return NULL;

    findWithNode(item,&this->m_root);

    return this->m_root;
}

template<class type>
Err_t CBSSplayTree<type>::Delete(type* item)
{
    nodeType<type>* p;
    nodeType<type>* root;
    if(item==NULL)
        return INVALIDE_PARAMET;

    if((p=Find(item))!=NULL)
    {
        if(p->lLink!=NULL)
        {
            root=this->findMax(p->lLink); //Find the maxmuim node in the left tree
            root=findWithNode(&root->elem,&p->lLink); //Push it to the root;
            root->rLink=p->rLink;
            this->m_root=root;
        }
        else
        {
            root=this->findMax(p->rLink); //Find the minimum node in the right tree
            root=findWithNode(&root->elem,&p->rLink); //Push it to the root;
            root->lLink=p->lLink;
            this->m_root=root;
        }
        delete p;
        p=NULL;
    }

    return RETURN_SUCCESS;
}

/*****************************************
*   Private functions below
*****************************************/

template<class type>
nodeType<type>* CBSSplayTree<type>::findWithNode(type* elem,nodeType<type>** node)
{
    cmp_t result;
    if((elem==NULL)||(node==NULL)||(*node==NULL))
        return NULL;

    /*
     *  process double rotation only
     */
    if((find(elem,node))==NULL)
        return NULL;

    /*
     *  maybe the found element is the left or right child of the root
     *  Push it to the root if neccessary!
     */

    if((result=this->m_cmpRoute(elem,&(*node)->elem))==LARGER)
    {
        *node=singleRotationWithRight(*node);
    }
    else if(result==SMALLER)
    {
        *node=singleRotationWithLeft(*node);
    }

    return *node;

}


template<class type>
nodeType<type>* CBSSplayTree<type>::find(type *elem,nodeType<type>** node)
{

    cmp_t r1,r2;
    nodeType<type>* p;
    /*
    *   Element not found it the spaly tree!
    */
    if((node==NULL)||(*node==NULL))
        return NULL;

    /*
    *   process recursive
    */

    if((r1=this->m_cmpRoute(elem,&(*node)->elem))==LARGER)
    {
        if((p=find(elem,&(*node)->rLink))==NULL) /*Cannot find the element*/
            return NULL;
        (*node)->rLink=p;
        if((r2=this->m_cmpRoute(elem,&(*node)->rLink->elem))==LARGER)
            *node=rightOnlyRotation(*node);
        else if(r2==SMALLER)
            *node=leftZigzagRotation(*node);
    }
    else if(r1==SMALLER)
    {
        if((p=find(elem,&(*node)->lLink))==NULL) /*Cannot find the element*/
            return NULL;
        (*node)->lLink=p;
        if((r2=this->m_cmpRoute(elem,&(*node)->lLink->elem))==LARGER)
            *node=rightZigzagRotation(*node);
        else if(r2==SMALLER)
            *node=leftOnlyRotation(*node);
    }

    /*
    *   find the equal just return ,do nothing
    */

    return *node;
}



template<class type>
nodeType<type>* CBSSplayTree<type>:: leftOnlyRotation(nodeType<type>* k3)
{
    nodeType<type>* k2,*k1;

    if(k3==NULL)
        return NULL;

    k2=k3->lLink;
    k1=k2->lLink;

    k3->lLink=k2->rLink;
    k2->rLink=k3;

    k2->lLink=k1->rLink;
    k1->rLink=k2;

    return k1;
}

template<class type>
nodeType<type>* CBSSplayTree<type>:: rightOnlyRotation(nodeType<type>* k3)
{
    nodeType<type>* k2,*k1;

    if(k3==NULL)
        return NULL;

    k2=k3->rLink;
    k1=k2->rLink;

    k3->rLink=k2->lLink;
    k2->lLink=k3;

    k2->rLink=k1->lLink;
    k1->lLink=k2;

    return k1;
}


template<class type>
nodeType<type>* CBSSplayTree<type>:: leftZigzagRotation(nodeType<type>* k3)
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

    return k1;
}


template<class type>
nodeType<type>* CBSSplayTree<type>:: rightZigzagRotation(nodeType<type>* k3)
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

    return k1;
}


template<class type>
nodeType<type>* CBSSplayTree<type>::singleRotationWithLeft(nodeType<type>* k2)
{
    printf("[%s]\n",__FUNCTION__);
    nodeType<type>* k1;

    if(k2==NULL)
        return NULL;

    k1=k2->lLink;
    k2->lLink=k1->rLink;
    k1->rLink=k2;

    return k1;
}

template<class type>
nodeType<type>* CBSSplayTree<type>::singleRotationWithRight(nodeType<type>* k2)
{
    printf("[%s]\n",__FUNCTION__);
    nodeType<type>* k1;

    if(k2==NULL)
        return NULL;

    k1=k2->rLink;
    k2->rLink=k1->lLink;
    k1->lLink=k2;

    return k1;
}

#endif
