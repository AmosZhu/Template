#ifndef _AMOS_CQueue_H
#define _AMOS_CQueue_H

#include <iostream>
#include <iomanip>
#include "AmosType.hpp"

/************************************************************
*
*           Author: Amos.zhu
*           @This is a FIFO queue template
*           @Data: 2013/7/30
*
************************************************************/


template<class type>
class CQueue
{
public:
    CQueue(void);
    CQueue(const CQueue<type>& object);
    ~CQueue(void);


    Err_t Enqueue(type elem);
    Err_t Dequeue(type* elem);
    Err_t EnqueueNoCopy(type* elem);
    Err_t DequeueNoCopy(type* elem);
    AM_S32 Search(type elem);
    AM_U32 CountNo(void);
    BOOL IsEmpty(void) const;
    void PrintOut(void);
    void Destroy(void);


public:
    CQueue<type>& operator=(const CQueue<type>& object);

private:
    node_t<type>* m_head;
    node_t<type>* m_tail;

public:
    template<typename T> friend std::ostream& operator<<(std::ostream& output,const CQueue<T>& object);

};


template<class type>
CQueue<type>::CQueue(void)
{
    m_head=NULL;
    m_tail=NULL;
}

template<class type>
CQueue<type>::CQueue(const CQueue<type>& object)
{
    if(this==&object)
        return;

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;

        current=object.m_head;

        newNode=new node_t<type>;
        newNode->element=current.element;
        newNode->next=NULL;

        m_head=newNode;
        m_tail=newNode;


        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            newNode->element=current.element;
            newNode->next=NULL;
            m_tail->next=newNode;
            m_tail=newNode;
        }

    }

    return;
}

template<class type>
CQueue<type>::~CQueue(void)
{
    Destroy();
}

template<class type>
Err_t CQueue<type>::Enqueue(type elem)
{
    if(&elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    memcpy(&newNode->element,&elem,sizeof(type));
    //newNode->element=elem;
    newNode->next=NULL;

    if(m_head==NULL||m_tail==NULL) //Nothing in CQueue,put the elem as first node;
    {
        m_head=newNode;
        m_tail=newNode;
    }

    m_tail->next=newNode;
    m_tail=newNode;

    return RETURN_SUCCESS;
}


template<class type>
Err_t CQueue<type>::Dequeue(type* elem)
{
    if(this->IsEmpty())
    {
        std::cout<<"No data in this queue!"<<std::endl;
        return OPERATOR_FAILED;
    }

    node_t<type>* current=this->m_head;
    if(m_head==m_tail)
        m_tail=NULL;
    m_head=m_head->next;
    current->next=NULL;
    memcpy(elem,&current->element,sizeof(type));
    delete current;

    return RETURN_SUCCESS;
}

template<class type>
Err_t CQueue<type>::EnqueueNoCopy(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    newNode->element=*elem;
    newNode->next=NULL;

    if(m_head==NULL||m_tail==NULL) //Nothing in CQueue,put the elem as first node;
    {
        m_head=newNode;
        m_tail=newNode;
        return RETURN_SUCCESS;
    }

    m_tail->next=newNode;
    m_tail=newNode;

    return RETURN_SUCCESS;
}


template<class type>
Err_t CQueue<type>::DequeueNoCopy(type* elem)
{
    if(this->IsEmpty())
    {
        std::cout<<"No data in this queue!"<<std::endl;
        return OPERATOR_FAILED;
    }

    node_t<type>* current=this->m_head;

    m_head=m_head->next;
    current->next=NULL;
    *elem=current->element;

    return RETURN_SUCCESS;
}


template<class type>
AM_S32 CQueue<type>::Search(type elem)
{
    node_t<type>* current;
    AM_S32 pos=0;
    if(&elem==NULL)
        return -1;

    if(!this->IsEmpty())
    {
        for(current=this->m_head; current!=NULL; current=current->next,pos++)
            if(current->element==elem)
                return pos;
    }

    return -1;
}

template<class type>
AM_U32 CQueue<type>::CountNo(void)
{
    AM_U32 count=0;
    node_t<type>* current;
    if(!this->IsEmpty())
    {
        current=this->m_head;
        while(current!=NULL)
        {
            count++;
            current=current->next;
        }
    }
    return count;
}

template<class type>
BOOL CQueue<type>::IsEmpty(void) const
{
    if(m_head==NULL||m_tail==NULL)
        return TRUE;

    return FALSE;
}

template<class type>
void CQueue<type>::Destroy(void)
{
    if(m_head==NULL||m_tail==NULL)
        return;

    node_t<type>* current;

    while(m_head!=NULL)
    {
        current=m_head;
        m_head=m_head->next;
        delete current;
    }
    m_head=NULL;
    m_tail=NULL;
    return;
}

template<class type>
void CQueue<type>::PrintOut(void)
{
    if(this->IsEmpty())
    {
        std::cout<<"It is an empty queue!"<<std::endl;
    }

    node_t<type>* current;

    for(current=this->m_head; current!=NULL; current=current->next)
    {
        std::cout<<current->element<<" ";
    }
    std::cout<<std::endl;
}

template<class type>
CQueue<type>& CQueue<type>::operator=(const CQueue<type>& object)
{
    if(this==&object)
        return *this;

    if(!this->IsEmpty())
        Destroy();

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;

        current=object.m_head;

        newNode=new node_t<type>;
        newNode->element=current->element;
        newNode->next=NULL;

        m_head=newNode;
        m_tail=newNode;

        if((current=current->next)!=NULL)
        {
            for(; current!=NULL; current=current->next)
            {
                newNode=new node_t<type>;
                newNode->element=current->element;
                newNode->next=NULL;
                m_tail->next=newNode;
                m_tail=newNode;
            }
        }
    }

    return *this;
}


template<typename T>
std::ostream& operator<<(std::ostream& output,const CQueue<T>& object)
{
    if(object.IsEmpty())
    {
        output<<"It is an empty queue!"<<std::endl;
        return output;
    }

    node_t<T>* current;

    for(current=object.m_head; current!=NULL; current=current->next)
    {
        output<<current->element<<" ";
    }
    output<<std::endl;
    return output;
}

#endif
