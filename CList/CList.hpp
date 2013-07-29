#ifndef _AMOS_CLIST_H
#define _AMOS_CLIST_H

#include <iostream>
#include <ionamp>
#include "AmosType.h"

template<typename type>
typedef struct NODE_TYPE
{
    type element;
    NODE_TYPE* next;
} node_t;


template<typename type>
class CList
{
public:
    CList(void);
    CList(const CList<type>& object);
    ~CList(void);

    Err_t Insert(type* elem);
    Err_t Delete(type* elem);
    Err_t Search(type* elem);
    BOOL IsEmpty(void);
    void Destroy(void);
    Err_t Reverse(void);

public:
    CList<type>& operator=(const CList<type>& object);

private:
    node_t* m_head;

public:
    template<typename type> friend std::ostream& operator<<(std::ostream& output,const CList<type>& object);
};


template<typename type>
CList<type>::CList(void)
{
    m_head=NULL;
}

template<typename type>
CList<type>::CList(const CList<type>& object)
{
    if(this==&object)
        return;

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;
        node_t<type>* previous;

        current=object.m_head;

        newNode=new node_t<type>;
        newNode->element=current.element;
        newNode->next=NULL;

        m_head=newNode;
        previous=m_head;
        current=current->next;
        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            newNode->element=current.element;
            newNode->next=NULL;
            previous->next=newNode;
            previous=previous->next;
        }

    }

    return;
}

template<typename type>
CList<type>::~CList(void)
{
    Destroy();
}



template<typename type>
Err_t CList<type>::Insert(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    newNode->element=*elem;
    newNode->next=NULL;

    if(m_head!=NULL) //Nothing in CList,put the elem as first node;
    {
        newNode->next=m_head;
        m_head=m_head->next;

    }
    else
    {
        m_head=newNode;
    }
    return RETURN_SUCCESS;
}

template<typename type>
Err_t CList<type>::Search(type* elem)
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
Err_t CList<type>::Delete(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* current;
    node_t<type>* temp;
    BOOL found=FALSE;
    if(m_head->element==*elem)
    {
        current=m_head;
        m_head=m_head->next;
        delete current;
        current=NULL;
        return RETURN_SUCCESS;
    }
    while(!found&&current!=NULL)
    {
        if(current->element==*elem)
        {
            found=TRUE;
            continue;
        }
        current=current->next;
    }
    if(current!=NULL)
    {
        current->element=current->next->element;
        temp=current->next;
        current->next=current->next->next;
        delete temp;
    }
    return RETURN_SUCCESS;
}

template<typename type>
BOOL CList<type>::IsEmpty(void)
{
    if(m_head==NULL)
        return true;

    return false;
}

template<typename type>
void CList<type>::Destroy(void)
{
    if(m_head==NULL)
        return;

    node_t<type>* current;
    current=m_head;
    while(m_head!=NULL)
    {
        m_head=m_head->next;
        delete current;
    }
    m_head=NULL;
    return;
}

/******************************************************************************
*
*   -------------        -------------       -------------
*   | data | *n |   -->  | data | *n |  -->  | data | *n |  -->  .....
*   -------------        -------------       -------------
*     previous              m_head               next
*
*   This function reverse list data
*******************************************************************************/

template<typename type>
Err_t CList<type>::Reverse(void)
{
    if(this->IsEmpty())
        return OPERATOR_FAILED;

    node_t<type>* previous;
    node_t<type>* next;
    next=m_head->next;
    previous=m_head;

    while(next!=NULL)
    {
        m_head->next=NULL;
        previous=m_head;
        m_head=next;
        next=m_head->next;
        m_head->next=previous;
    }

    return RETURN_SUCCESS;

}


template<typename type>
CList<type>& CList<type>::operator=(const CList<type>& object)
{
    if(this==&object)
        return *this;

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;
        node_t<type>* previous;

        current=object.m_head;

        newNode=new node_t<type>;
        newNode->element=current.element;
        newNode->next=NULL;

        m_head=newNode;
        previous=m_head;
        current=current->next;
        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            newNode->element=current.element;
            newNode->next=NULL;
            previous->next=newNode;
            previous=newNode;
        }
    }

    return *this;
}

template<typename type>
std::otream& operator<<(std::ostream& output,const CList<type>& object)
{
    if(!object.IsEmpty())
    {
        node_t<type>* current;
        current=m_head;
        while(current!=NULL)
        {
            output<<current->element<<" ";
            current=current->next;
        }
        output<<std::endl;
    }
    return output;
}

#endif
