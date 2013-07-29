#ifndef _AMOS_CQueue_H
#define _AMOS_CQueue_H

#include <iostream>
#include <iomanip>
#include "AmosType.h"


template<typename type>
typedef struct NODE_TYPE
{
    type element;
    NODE_TYPE* next;
} node_t;


template<typename type>
class CQueue
{
public:
    CQueue(void);
    CQueue(const CQueue<type>& object);
    ~CQueue(void);


    Err_t Enqueue(type* elem);
    type* Dequeue(void);
    Err_t Search(type* elem);
    BOOL IsEmpty(void);
    void Destroy(void);
    void PrintOut(void);

public:
    CQueue<type>& operator=(const CQueue<type>& object);

private:
    node_t* m_head;
    node_t* m_tail;


public:
    template<typename type> friend std::ostream& operator<<(std::ostream& output,const CQueue<type>& object)

};


template<typename type>
CQueue<type>::CQueue(void)
{
    m_head=NULL;
    m_tail=NULL;
}

template<typename type>
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

template<typename type>
CQueue<type>::~CQueue(void)
{
    Destroy();
}

template<typename type>
CQueue<type>& CQueue<type>::operator=(const CQueue<type>& object)
{
    if(this==&object)
        return *this;

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

        if((current=current->next)!=NULL)
        {
            for(; current!=NULL; current=current->next)
            {
                newNode=new node_t<type>;
                newNode->element=current.element;
                newNode->next=NULL;
                m_tail->next=newNode;
                m_tail=newNode;
            }
        }
    }

    return *this;
}

template<typename type>
Err_t CQueue<type>::Enqueue(type* elem)
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
    }

    m_tail->next=newNode;
    m_tail=newNode;

    return RETURN_SUCCESS;
}

//FIFO
template<typename type>
type* CQueue<type>::Dequeue(void)
{
    if(this->IsEmpty())
        return NULL;

    node_t<type>* current=this->m_head;
    m_head=m_head->next;
    current->next=NULL;
    return current;
}

template<typename type>
Err_t CQueue<type>::Search(type* elem)
{
    node_t<type>* current;

    if(elem==NULL)
        return INVALIDE_PARAMET;

    if(!this->IsEmpty())
    {
        for(current=this->m_head; current!=NULL; current=current->next)
            if(current->element==*elem)
                return RETURN_SUCCESS;
    }

    std::cout<<"element not found!"<<std::endl;

    return OPERATOR_FAILED;
}

template<typename type>
BOOL CQueue<type>::IsEmpty(void)
{
    if(m_head==NULL||m_tail==NULL)
        return TRUE;

    return FALSE;
}

template<typename type>
void CQueue<type>::Destroy(void)
{
    if(m_head==NULL||m_tail==NULL)
        return;

    node_t<type>* current;
    current=m_head;
    while(m_head!=NULL)
    {
        m_head=m_head->next;
        delete current;
    }
    m_head=NULL;
    m_tail=NULL;
    return;
}

template<typename type>
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

template<typename type>
std::ostream& operator<<(std::ostream& output,const CQueue<type>& object)
{
    if(object.IsEmpty())
    {
        output<<"It is an empty queue!"<<std::endl;
        return output;
    }

    node_t<type>* current;

    for(current=this->m_head; current!=NULL; current=current->next)
    {
        output<<current->element<<" ";
    }
    output<<std::endl;
    return output;
}


#endif
