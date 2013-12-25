#ifndef _AMOS_CSTACK_HPP
#define _AMOS_CSTACK_HPP

#include "AmosType.hpp"
#include <string.h>



/***********************************************************************
*
*   This is stack structure and operation
*
*
*       ++++++++++++
*       | new Node |  <-- new node push then become head
*       ++++++++++++
*           Link
*           Link
*       ++++++++++++
*       |   top    |  <-- m_head
*       ++++++++++++
*            |
*            v
*       ++++++++++++
*       |   Elem   |  <-- top elem pop out and this will become head
*       ++++++++++++
*            |
*            v
*       ++++++++++++
*       |  Bottom  |
*       ++++++++++++
*
*
**********************************************************************/
template<class type>
class CStack
{
public:
    CStack(void);
    CStack(const CStack<type>& object);
    ~CStack(void);

    void Destroy(void);

    Err_t Push(type* elem);
    Err_t Pop(type* elem);

    Err_t PushNoCopy(type* elem);
    Err_t PopNoCopy(type* elem);


    Err_t Top(type* elem);

    AM_U32 Size(void);
    AM_U32 CountNo(void);
    BOOL IsEmpty(void) const;

public:
    CStack<type>& operator=(const CStack<type>& object);


private:
    node_t<type>* m_head;
    AM_U32 count;
    AM_U32 size;
};

template<class type>
CStack<type>::CStack(void)
{
    m_head=NULL;
    count=0;
    size=0;
}

template<class type>
CStack<type>::CStack(const CStack<type>& object)
{
    node_t<type>* newNode;
    node_t<type>* current;
    node_t<type>* tail;
    m_head=NULL;
    count=0;
    size=0;
    if(!object.IsEmpty())
    {
        /***************************
        *   operate m_head first
        ***************************/
        current=object.m_head;
        newNode=new node_t<type>;
        newNode->next=NULL;
        memcpy(&newNode->element,&current->element,sizeof(type));
        m_head=newNode;
        tail=m_head;
        while((current=current->next)!=NULL)
        {
            newNode=new node_t<type>;
            newNode->next=NULL;
            memcpy(&newNode->element,&current->element,sizeof(type));
            tail->next=newNode;
            tail=tail->next;
        }
        count=object.count;
        size=object.size;
    }
    return;
}

template<class type>
CStack<type>::~CStack(void)
{
    Destroy();
}

template<class type>
void CStack<type>::Destroy(void)
{
    node_t<type>* current;
    if(!IsEmpty())
    {
        current=m_head;
        while(current!=NULL)
        {
            m_head=m_head->next;
            delete current;
            current=m_head;
        }
        count=0;
        size=0;
    }
    return;
}

template<class type>
Err_t CStack<type>::Push(type* elem)
{
    node_t<type>* newNode;

    if(elem==NULL)
        return INVALIDE_PARAMET;
    newNode=new node_t<type>;
    newNode->next=m_head;
    memcpy(&newNode->element,elem,sizeof(type));
    m_head=newNode;

    count++;  //Use to count the number of elements
    size+=sizeof(node_t<type>); //Use to count the stack size;

    return RETURN_SUCCESS;
}

template<class type>
Err_t CStack<type>::PushNoCopy(type* elem)
{
    node_t<type>* newNode;

    if(elem==NULL)
        return INVALIDE_PARAMET;
    newNode=new node_t<type>;
    newNode->next=m_head;
    memcpy(&newNode->element,elem,sizeof(type*));
    m_head=newNode;

    count++;  //Use to count the number of elements
    size+=sizeof(node_t<type*>); //Use to count the stack size;

    return RETURN_SUCCESS;
}

template<class type>
Err_t CStack<type>::PopNoCopy(type* elem)
{
    node_t<type>* current;
    if(elem==NULL)
        return INVALIDE_PARAMET;
    if(IsEmpty())
        return RESOURCE_UNAVAILABLE;

    current=m_head;
    m_head=m_head->next;
    memcpy(elem,&current->element,sizeof(type*));
    if(current!=NULL)
        delete current;
    current=NULL;

    count--;
    size-=sizeof(node_t<type>*);

    return RETURN_SUCCESS;
}


template<class type>
Err_t CStack<type>::Pop(type* elem)
{
    node_t<type>* current;
    if(elem==NULL)
        return INVALIDE_PARAMET;
    if(IsEmpty())
        return RESOURCE_UNAVAILABLE;

    current=m_head;
    m_head=m_head->next;
    memcpy(elem,&current->element,sizeof(type));
    if(current!=NULL)
        delete current;
    current=NULL;

    count--;
    size-=sizeof(node_t<type>);

    return RETURN_SUCCESS;
}

template<class type>
Err_t CStack<type>::Top(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;
    if(IsEmpty())
        return RESOURCE_UNAVAILABLE;

    memcpy(elem,&m_head->element,sizeof(type));

    return RETURN_SUCCESS;
}

template<class type>
AM_U32 CStack<type>::Size(void)
{
    return size;
}

template<class type>
AM_U32 CStack<type>::CountNo(void)
{
    return count;
}

template<class type>
BOOL CStack<type>::IsEmpty(void) const
{
    if(m_head==NULL)  //Or count==0, Or Both
        return TRUE;

    return FALSE;
}

template<class type>
CStack<type>& CStack<type>::operator=(const CStack<type>& object)
{
    node_t<type>* newNode;
    node_t<type>* current;
    node_t<type>* tail;

    if(this==&object)
        return *this;

    m_head=NULL;
    count=0;
    size=0;
    if(!object.IsEmpty())
    {
        /***************************
        *   operate m_head first
        ***************************/
        current=object.m_head;
        newNode=new node_t<type>;
        newNode->next=NULL;
        memcpy(&newNode->element,&current->element,sizeof(type));
        m_head=newNode;
        tail=m_head;
        while((current=current->next)!=NULL)
        {
            newNode=new node_t<type>;
            newNode->next=NULL;
            memcpy(&newNode->element,&current->element,sizeof(type));
            tail->next=newNode;
            tail=tail->next;
        }
        count=object.count;
        size=object.size;
    }
    return *this;
}

#endif
