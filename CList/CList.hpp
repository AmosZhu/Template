#ifndef _AMOS_CLIST_H
#define _AMOS_CLIST_H

#include <iostream>
#include <string.h>
#include "AmosType.hpp"


template<class type>
class CList
{
public:
    CList(void);
    CList(const CList<type>& object);
    ~CList(void);
    void Destroy(void);

    Err_t Insert(type* elem);
    Err_t Delete(type* elem);
    /***********************************************
    *   Return the position of the element
    ***********************************************/
    type* Search(type* elem);
    /**********************************************
    *   This function set the m_pSearch to m_head
    **********************************************/
    void ResetElemNext(void);
    /**********************************************
    *   Get the current Elem and point to next
    **********************************************/
    Err_t GetElemNext(type* elem);
    BOOL IsEmpty(void) const;
    /***************************************
    *   Return the total number in the List
    ***************************************/
    AM_U32 CountNo(void);
    Err_t Reverse(void);
    void PrintOut(void);

    void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    void SetCopyFunc(void (*func)(type* dst,type* src));
    void SetPrintFunc(void (*func)(type* src));

public:
    CList<type>& operator=(const CList<type>& object);

public:
    template<typename T> friend std::ostream& operator<<(std::ostream& output,const CList<T>& object);

private:
    cmp_t (*m_cmpRoute)(type* k1,type* k2);
    void (*m_cpyRoute)(type* dst,type* src); //Copy function for the node,set by different type;
    void (*m_printRoute)(type* src);
private:
    node_t<type>* m_head;
    node_t<type>* m_pSearch;  //Use for search
    AM_U32 count; //Number of elements;

};


template<class type>
CList<type>::CList(void)
{
    m_head=NULL;
    m_pSearch=NULL;
    m_cmpRoute=NULL;
    m_cpyRoute=NULL;
    m_printRoute=NULL;
    count=0;
}

template<class type>
CList<type>::CList(const CList<type>& object)
{
    if(this==&object)
        return;

    m_head=NULL;
    m_pSearch=NULL;
    count=0;
    m_cmpRoute=object.m_cmpRoute;
    m_cpyRoute=object.m_cpyRoute;
    m_printRoute=object.m_printRoute;

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;
        node_t<type>* previous;

        current=object.m_head;

        newNode=new node_t<type>;
        m_cpyRoute(&newNode->element,&current->element);
        newNode->next=NULL;

        m_head=newNode;
        previous=m_head;
        current=current->next;
        count=object.count;
        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            m_cpyRoute(&newNode->element,&current->element);
            newNode->next=NULL;
            previous->next=newNode;
            previous=previous->next;
        }
    }

    return;
}

template<class type>
CList<type>::~CList(void)
{
    Destroy();
}

template<class type>
Err_t CList<type>::Insert(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    m_cpyRoute(&newNode->element,elem);
    newNode->next=NULL;

    if(m_head!=NULL) //Nothing in CList,put the elem as first node;
    {
        newNode->next=m_head;
    }

    m_head=newNode;
    count++;
    return RETURN_SUCCESS;
}

template<class type>
type* CList<type>::Search(type* elem)
{
    node_t<type>* current;

    if(elem==NULL)
        return NULL;

    if(!this->IsEmpty())
    {
        for(current=this->m_head; current!=NULL; current=current->next)
            if(m_cmpRoute(&current->element,elem)==EQUAL)
                return &current->element;
    }

    return NULL;
}

/***********************************************************
*
*   There two different strategy to delete an element
*
***********************************************************/
template<class type>
Err_t CList<type>::Delete(type* elem)
{
    if(&elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* current;
    node_t<type>* temp;
    current=m_head;

    BOOL found=FALSE;
    if(m_cmpRoute(&current->element,elem)==EQUAL)
    {
        m_head=m_head->next;
        delete current;
        current=NULL;
        count--;
        return RETURN_SUCCESS;
    }
    while(!found&&current!=NULL)
    {
        if(m_cmpRoute(&current->element,elem)==EQUAL)
        {
            found=TRUE;
            continue;
        }
        temp=current;
        current=current->next;
    }

    if(current==NULL)
        return OPERATOR_FAILED;

    if(current->next!=NULL)
    {
        temp->next=current->next;
    }
    else
    {
        temp->next=NULL;
    }
    delete current;
    count--;
    return RETURN_SUCCESS;

}

/*********************************************************************
*
*   Call this function before using the GetElemNext func;
*
**********************************************************************/
template<class type>
void CList<type>::ResetElemNext(void)
{
    m_pSearch=m_head;
}
/*********************************************************************
*
*   @Param:elem store the result
*
*   -------------        -------------       -------------
*   | data | *n |   -->  | data | *n |  -->  | data | *n |  -->  .....
*   -------------        -------------       -------------
*     m_pSerach    --->    m_pSearch
*  (Get this data)    (Move to next elem)
*
***********************************************************************/
template<class type>
Err_t CList<type>::GetElemNext(type* elem)
{
    if((this->m_pSearch==NULL)||(this->IsEmpty()))
        return OPERATOR_FAILED;
    if(elem==NULL)
        return INVALIDE_PARAMET;

    m_cpyRoute(elem,&m_pSearch->element);
    m_pSearch=m_pSearch->next;
    return RETURN_SUCCESS;
}

template<class type>
BOOL CList<type>::IsEmpty(void) const
{
    if(m_head==NULL)
        return TRUE;

    return FALSE;
}

template<class type>
AM_U32 CList<type>::CountNo(void)
{
    return count;
}

template<class type>
void CList<type>::Destroy(void)
{
    if(this->m_head==NULL)
        return;

    node_t<type>* current;

    while(this->m_head!=NULL)
    {
        current=this->m_head;
        this->m_head=this->m_head->next;
        delete current;
    }
    this->m_head=NULL;
    count=0;
    m_pSearch=NULL;
    return;
}

template<class type>
void CList<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{
    m_cmpRoute=func;
}

template<class type>
void CList<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    m_cpyRoute=func;
}

template<class type>
void CList<type>::SetPrintFunc(void (*func)(type* src))
{
    m_printRoute=func;
}


template<class type>
void CList<type>::PrintOut(void)
{
    if(this->IsEmpty())
    {
        std::cout<<"It is an empty list!"<<std::endl;
        return;
    }

    node_t<type>* current;

    for(current=this->m_head; current!=NULL; current=current->next)
    {
        m_printRoute(&current->element);
    }
    std::cout<<std::endl;
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

template<class type>
Err_t CList<type>::Reverse(void)
{
    if(this->IsEmpty())
        return OPERATOR_FAILED;

    node_t<type>* previous;
    node_t<type>* next;
    next=m_head->next;
    previous=m_head;
    m_head->next=NULL;

    while(next!=NULL)
    {
        previous=m_head;
        m_head=next;
        next=m_head->next;
        m_head->next=previous;
    }

    return RETURN_SUCCESS;
}


template<class type>
CList<type>& CList<type>::operator=(const CList<type>& object)
{
    if(this==&object)
        return *this;

    m_head=NULL;
    m_pSearch=NULL;
    count=0;
    m_cmpRoute=object.m_cmpRoute;
    m_cpyRoute=object.m_cpyRoute;
    m_printRoute=object.m_printRoute;

    if(!object.IsEmpty())
    {
        node_t<type>* current;
        node_t<type>* newNode;
        node_t<type>* previous;

        current=object.m_head;


        newNode=new node_t<type>;
        newNode->element=current->element;
        newNode->next=NULL;

        m_head=newNode;
        previous=m_head;
        current=current->next;
        count=object.count;

        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            newNode->element=current->element;
            newNode->next=NULL;
            previous->next=newNode;
            previous=newNode;
        }
    }

    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& output,const CList<T>& object)
{
    if(object.IsEmpty())
    {
        output<<"It is an empty list!"<<std::endl;
        return output;
    }

    node_t<T>* current;

    for(current=object.m_head; current!=NULL; current=current->next)
    {
        object.m_printRoute(&current->element);
    }
    output<<std::endl;
    return output;
}

#endif
