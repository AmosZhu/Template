#ifndef _CLEFT_HEAP_HPP_
#define _CLEFT_HEAP_HPP_

#include "AmosType.hpp"
#include "CStack.hpp"
#include "CQueue.hpp"
#include <iostream>

template<typename type>
struct nodeType
{
    type elem;
    nodeType<type>* lLink;
    nodeType<type>* rLink;
    AM_S32 npl;  //Null path lenght
};

template<class type>
class CLeftHeap
{
public:
    CLeftHeap();
    CLeftHeap(const CLeftHeap<type>& object);
    virtual ~CLeftHeap();

    Err_t Insert(type* elem);
    Err_t Merge(CLeftHeap<type>* object);
    Err_t DeleteMin(type* elem);
    AM_U32 ElemCount(void);
    void PrintOut(void);
    void Destroy(void);
    BOOL IsEmpty(void);

    void SetCopyFunc(void (*func)(type* dst,type* src));
    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    void SetPrintFunc(void (*func)(type* src));


private:
    nodeType<type>* merge(nodeType<type>* k1,nodeType<type>* k2);
    void copyTree(nodeType<type>** copiedTreeNode,nodeType<type>* otherTreeNode);
    void destroy(nodeType<type>** p);
    void levelorder(nodeType<type>* p,CQueue<nodeType<type>* >* queue);
    void swapChildren(nodeType<type>** k1,nodeType<type>** k2);

public:
    const CLeftHeap<type>& operator=(const CLeftHeap<type>& otherTree);


protected:
    nodeType<type>* m_root;
    AM_U32 m_size;

    void (*m_cpyRoute)(type* dst,type* src);
    cmp_t (*m_cmpRoute)(type* k1,type* k2);
    void (*m_printRoute)(type* src);

};

template<class type>
CLeftHeap<type>::CLeftHeap()
{
    m_root=NULL;
    m_cpyRoute=NULL;
    m_cmpRoute=NULL;
    m_printRoute=NULL;
    m_size=0;
}

template<class type>
CLeftHeap<type>::CLeftHeap(const CLeftHeap<type>& object)
{
    if(this==&object)
        return;

    m_size=object.m_size;
    m_cpyRoute=object.m_cpyRoute;
    m_cmpRoute=object.m_cmpRoute;
    m_printRoute=object.m_printRoute;

    if(object.m_root==NULL)
    {
        m_root=NULL;
        return;
    }

    copyTree(&m_root,object.m_root);

    return;

}

template<class type>
CLeftHeap<type>::~CLeftHeap()
{
    Destroy();
}


template<class type>
Err_t CLeftHeap<type>::Insert(type* elem)
{
    nodeType<type>* newNode,*tailNode;

    if(elem==NULL)
        return INVALIDE_PARAMET;

    newNode=new nodeType<type>;
    m_cpyRoute(&newNode->elem,elem);
    newNode->npl=0;
    newNode->lLink=NULL;
    newNode->rLink=NULL;

    if(m_root==NULL)
    {
        m_root=newNode;
        m_size=1;

        return RETURN_SUCCESS;
    }


    if((tailNode=merge(m_root,newNode))==NULL)
    {
        return OPERATOR_FAILED;
    }

    m_size++;

    m_root=tailNode;

    return RETURN_SUCCESS;

}

template<class type>
Err_t CLeftHeap<type>::DeleteMin(type* elem)
{
    nodeType<type>* h1,*h2,*current;
    if(elem==NULL)
        return INVALIDE_PARAMET;

    if(m_root==NULL)
        return OPERATOR_FAILED;

    current=m_root;
    this->m_cpyRoute(elem,&current->elem);


    h1=m_root->lLink;
    h2=m_root->rLink;
    m_root=merge(h1,h2);

    delete current;
    m_size--;

    return RETURN_SUCCESS;
}


template<class type>
Err_t CLeftHeap<type>::Merge(CLeftHeap<type>* object)
{
    nodeType<type>*tailNode=NULL;
    if(object==NULL)
    {
        return INVALIDE_PARAMET;
    }

    m_size+=object->m_size;

    tailNode=merge(m_root,object->m_root);
    object->m_root=NULL;

    if(tailNode!=NULL)
        m_root=tailNode;

    return RETURN_SUCCESS;
}

template<class type>
void CLeftHeap<type>::PrintOut(void)
{
    CQueue<nodeType<type>*> queue;

    if(IsEmpty())
    {
        std::cout<<"The Heap is Empty!"<<std::endl;
        return;
    }

    std::cout<<"Levelorder: ";
    levelorder(m_root,&queue);
    std::cout<<std::endl;
}

template<class type>
AM_U32 CLeftHeap<type>::ElemCount(void)
{
    return m_size;
}

template<class type>
BOOL CLeftHeap<type>::IsEmpty(void)
{
    if(m_root==NULL)
        return TRUE;

    return FALSE;
}


template<class type>
void CLeftHeap<type>::Destroy(void)
{
    if(m_root==NULL)
        return;

    destroy(&m_root);
}


template<class type>
void CLeftHeap<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    m_cpyRoute=func;
}

template<class type>
void CLeftHeap<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    m_cmpRoute=func;
}

template<class type>
void CLeftHeap<type>::SetPrintFunc(void (*func)(type* src))
{
    m_printRoute=func;
}

template<class type>
void CLeftHeap<type>::swapChildren(nodeType<type>** k1,nodeType<type>** k2)
{
    nodeType<type>* temp;
    if(*k1==NULL||*k2==NULL)
        return;

    temp=*k1;
    *k1=*k2;
    *k2=temp;
    return;
}

template<class type>
nodeType<type>* CLeftHeap<type>::merge(nodeType<type>* k1,nodeType<type>* k2)
{
    nodeType<type>* h1,*h2,*temp,*current,*tailNode=NULL;
    CStack<nodeType<type>*> stack;
    cmp_t result;

    if(k1==NULL&&k2==NULL)
        return NULL;

    if(k1==NULL)
        return k2;
    else if(k2==NULL)
        return k1;

    h1=k1;
    h2=k2;

    while(h1!=NULL&&h2!=NULL)
    {
        if((result=m_cmpRoute(&h1->elem,&h2->elem))==SMALLER)
        {
            stack.PushNoCopy(&h1);
            h1=h1->rLink;
        }
        else
        {
            stack.PushNoCopy(&h2);
            temp=h1;
            h1=h2->rLink;
            h2=temp;
        }
    }

    if(!stack.IsEmpty())
    {
        stack.PopNoCopy(&tailNode);
        if(tailNode->lLink==NULL)
        {
            tailNode->lLink=h2;
        }
        else
        {
            if(h1==NULL)
            {
                tailNode->rLink=h2;
            }
            else
            {
                tailNode->rLink=h1;
            }

            if(tailNode->lLink->npl<tailNode->rLink->npl)
            {
                swapChildren(&tailNode->lLink,&tailNode->rLink);
            }

            if(tailNode->rLink==NULL)
                tailNode->npl=0;
            else
                tailNode->npl=tailNode->rLink->npl+1;
        }


    }

    while(!stack.IsEmpty())
    {
        stack.PopNoCopy(&current);
        current->rLink=tailNode;
        if(current->lLink->npl<current->rLink->npl)
        {
            swapChildren(&current->lLink,&current->rLink);
        }
        if(current->rLink==NULL)
            current->npl=0;
        else
            current->npl=current->rLink->npl+1;
        tailNode=current;
    }

    return tailNode;
}

template<typename type>
void CLeftHeap<type>::copyTree(nodeType<type>** copiedTreeNode,nodeType<type>* otherTreeNode)
{
    if(otherTreeNode==NULL)
    {
        *copiedTreeNode=NULL;
        return;
    }

    *copiedTreeNode=new nodeType<type>;
    this->m_cpyRoute(&(*copiedTreeNode)->elem,&otherTreeNode->elem);
    copyTree(&(*copiedTreeNode)->lLink,otherTreeNode->lLink);
    copyTree(&(*copiedTreeNode)->rLink,otherTreeNode->rLink);

    return;
}

template<class type>
void CLeftHeap<type>::destroy(nodeType<type>** p)
{
    if(*p==NULL)
        return;

    destroy(&(*p)->lLink);
    destroy(&(*p)->rLink);
    delete *p;

    return;
}


template<class type>
void CLeftHeap<type>::levelorder(nodeType<type>* p,CQueue<nodeType<type>* >* queue)
{
    nodeType<type>* current;
    if((p==NULL)||(queue==NULL))
        return;

    queue->EnqueueNoCopy(&p);

    while(!queue->IsEmpty())
    {
        queue->DequeueNoCopy(&current);
        this->m_printRoute(&current->elem);
        if(current->lLink!=NULL)
            queue->EnqueueNoCopy(&current->lLink);
        if(current->rLink!=NULL)
            queue->EnqueueNoCopy(&current->rLink);
    }

    return;
}

template<typename type>
const CLeftHeap<type>& CLeftHeap<type>::operator=(const CLeftHeap<type>& otherTree)
{
    if(this==&otherTree)
        return *this;

    if(m_root!=NULL)
        destroy(&m_root);

    if(otherTree.m_root==NULL)
    {
        m_root=NULL;
        return *this;
    }

    m_size=otherTree.m_size;
    m_cpyRoute=otherTree.m_cpyRoute;
    m_cmpRoute=otherTree.m_cmpRoute;
    m_printRoute=otherTree.m_printRoute;
    copyTree(&m_root,otherTree.m_root);

    return *this;
}

#endif
