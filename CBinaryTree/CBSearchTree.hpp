#ifndef __CBSEARCHTREE_HPP__
#define __CBSEARCHTREE_HPP__

#include "AmosType.hpp"
#include "CBinaryTree.hpp"
#include <iostream>
#include <string.h>

template<class type>
class CBSearchTree:public CBinaryTree<type>
{
public:
    CBSearchTree();
    CBSearchTree(const CBSearchTree<type>& object);
    ~CBSearchTree();
    virtual nodeType<type>* Find(type* item);
    nodeType<type>* FindMin(void);
    nodeType<type>* FindMax(void);

    /*
    *   Different type must supply different compare function;
    */
    BOOL SetCmpFunc(cmp_t (*func)(type*,type*));

    virtual Err_t Insert(type* item);
    virtual Err_t Delete(type* item);

protected:
    nodeType<type>* findMin(nodeType<type>* root);
    nodeType<type>* findMax(nodeType<type>* root);

private:
    void deleteFromTree(nodeType<type>** p);
protected:
    cmp_t (*m_cmpRoute)(type*,type*);
};

template<class type>
CBSearchTree<type>::CBSearchTree()
{
    m_cmpRoute=NULL;
}

template<class type>
CBSearchTree<type>::CBSearchTree(const CBSearchTree<type>& object)
{
    m_cmpRoute=object.m_cmpRoute;
}

template<class type>
CBSearchTree<type>::~CBSearchTree()
{
    m_cmpRoute=NULL;
}


template<class type>
BOOL CBSearchTree<type>::SetCmpFunc(cmp_t (*func)(type*,type*))
{
    if(func==NULL)
        return FALSE;

    m_cmpRoute=func;
    return TRUE;
}

template<class type>
nodeType<type>* CBSearchTree<type>::FindMin()
{
    nodeType<type>* current=NULL;

    if(this->m_root!=NULL)
    {
        current=this->m_root;
        while(current->lLink!=NULL)
        {
            current=current->lLink;
        }
    }

    return current;
}

template<class type>
nodeType<type>* CBSearchTree<type>::FindMax()
{
    nodeType<type>* current=NULL;

    if(this->m_root!=NULL)
    {
        current=this->m_root;
        while(current->rLink!=NULL)
        {
            current=current->rLink;
        }
    }

    return current;
}


template<class type>
nodeType<type>* CBSearchTree<type>::Find(type* item)
{
    BOOL found=FALSE;
    nodeType<type>* current=NULL;
    cmp_t result;

    if(item==NULL)
        return NULL;

    if(this->m_root!=NULL)
    {
        current=this->m_root;
        while(current!=NULL&&found==FALSE)
        {
            if((result=m_cmpRoute(item,&current->elem))==EQUAL)
            {
                found=TRUE;
                continue;
            }
            else if(result==LARGER)
            {
                current=current->rLink;
            }
            else if(result==SMALLER)
            {
                current=current->lLink;
            }
        }
    }

    if(!found)
    {
        printf("element not found!\n");
        return NULL;
    }

    return current;
}

template<class type>
Err_t CBSearchTree<type>::Insert(type* item)
{

    nodeType<type>* current;
    nodeType<type>* newNode;
    nodeType<type>* tailNode;
    cmp_t result;

    if(item==NULL)
        return INVALIDE_PARAMET;

    newNode=new nodeType<type>;
    memcpy(&newNode->elem,item,sizeof(type));
    newNode->lLink=NULL;
    newNode->rLink=NULL;

    if(this->m_root==NULL)
        this->m_root=newNode;
    else
    {
        current=this->m_root;
        while(current!=NULL)
        {
            tailNode=current;
            if((result=m_cmpRoute(&newNode->elem,&current->elem))==EQUAL)
            {
                std::cout<<"Not allowed to Insert same node!"<<std::endl;
                delete newNode;
                return OPERATOR_FAILED;
            }
            else if(result==LARGER)
                current=current->rLink;
            else if(result==SMALLER)
                current=current->lLink;
        }

        if((result=m_cmpRoute(&newNode->elem,&tailNode->elem))==LARGER)
        {
            tailNode->rLink=newNode;
        }
        else if(result==SMALLER)
        {
            tailNode->lLink=newNode;
        }

    }

    return RETURN_SUCCESS;
}

template<class type>
Err_t CBSearchTree<type>::Delete(type* item)
{
    BOOL found=FALSE;
    cmp_t result;
    nodeType<type>* current;
    nodeType<type>* tailNode;

    if(item==NULL)
        return INVALIDE_PARAMET;

    if(this->m_root!=NULL)
    {
        current=this->m_root;
        tailNode=NULL;
        while(current!=NULL&&found==FALSE)
        {
            if((result=m_cmpRoute(item,&current->elem))==EQUAL)
            {
                found=TRUE;
                continue;
            }
            tailNode=current;
            if(result==LARGER)
            {
                current=current->rLink;
            }
            else if(result==SMALLER)
            {
                current=current->lLink;
            }
        }

        if(current==NULL)
        {
            std::cout<<"item not in the binary tree!"<<std::endl;
            return OPERATOR_FAILED;
        }
        else
        {
            if(tailNode!=NULL)
            {
                if(found)
                {
                    this->m_printRoute(tailNode->elem);
                    if((result=m_cmpRoute(&tailNode->elem,item))==LARGER)
                    {
                        deleteFromTree(&tailNode->lLink);
                    }
                    else if(result==SMALLER)
                    {
                        deleteFromTree(&tailNode->rLink);
                    }
                }
            }
            else
                deleteFromTree(&this->m_root);
        }
    }

    return RETURN_SUCCESS;

}

template<class type>
void CBSearchTree<type>::deleteFromTree(nodeType<type>** p)
{
    nodeType<type>* temp;
    nodeType<type>* tailNode;
    nodeType<type>* current;

    if((p==NULL)||(*p==NULL))
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
        current=(*p)->lLink;
        tailNode=NULL;
        while(current->rLink!=NULL)
        {
            tailNode=current;
            current=current->rLink;
        }
        memcpy(&(*p)->elem,&current->elem,sizeof(type));
        if(tailNode==NULL)
            (*p)->lLink=current->lLink;
        else
            tailNode->rLink=current->lLink;

        delete current;
    }
}

template<class type>
nodeType<type>* CBSearchTree<type>::findMin(nodeType<type>* root)
{
    nodeType<type>* current=NULL;

    if(root==NULL)
        return NULL;

    current=root;
    while(current->lLink!=NULL)
    {
        current=current->lLink;
    }

    return current;
}

template<class type>
nodeType<type>* CBSearchTree<type>::findMax(nodeType<type>* root)
{
    nodeType<type>* current=NULL;

    if(root==NULL)
        return NULL;

    current=root;
    while(current->rLink!=NULL)
    {
        current=current->rLink;
    }

    return current;
}


#endif
