#ifndef _AMOS_CLIST_H
#define _AMOS_CLIST_H

#include <iostream>
#include "AmosType.hpp"


template<class type>
class CList
{
public:
    CList(void);
    CList(const CList<type>& object);
    ~CList(void);
    void Destroy(void);

    Err_t Insert(type elem);
    Err_t Delete(type elem);
    /***********************************************
    *   Return the position of the element
    ***********************************************/
    AM_S32 Search(type elem);
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
public:
    CList<type>& operator=(const CList<type>& object);

private:
    node_t<type>* m_head;
    node_t<type>* m_pSearch;  //Use for search

public:
    template<typename T> friend std::ostream& operator<<(std::ostream& output,const CList<T>& object);
};


template<class type>
CList<type>::CList(void)
{
    m_head=NULL;
    m_pSearch=NULL;
}

template<class type>
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
        newNode->element=current->element;
        newNode->next=NULL;

        m_head=newNode;
        previous=m_head;
        current=current->next;
        for(; current!=NULL; current=current->next)
        {
            newNode=new node_t<type>;
            newNode->element=current->element;
            newNode->next=NULL;
            previous->next=newNode;
            previous=previous->next;
        }
    }
    else
    {
        m_head=NULL;
        m_pSearch=NULL;
    }

    return;
}

template<class type>
CList<type>::~CList(void)
{
    Destroy();
}

template<class type>
Err_t CList<type>::Insert(type elem)
{
    if(&elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    memcpy(&newNode->element,&elem,sizeof(type));
    newNode->next=NULL;

    if(m_head!=NULL) //Nothing in CList,put the elem as first node;
    {
        newNode->next=m_head;
    }

    m_head=newNode;
    return RETURN_SUCCESS;
}

template<class type>
AM_S32 CList<type>::Search(type elem)
{
    node_t<type>* current;
    AM_S32 position = 0;

    if(&elem==NULL)
        return -1;

    if(!this->IsEmpty())
    {
        for(current=this->m_head; current!=NULL; current=current->next,position++)
            if(current->element==elem)
                return position;
    }

    return -1;
}

/***********************************************************
*
*   There two different strategy to delete an element
*
*
***********************************************************/
template<class type>
Err_t CList<type>::Delete(type elem)
{
    if(&elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* current;
    node_t<type>* temp;
    current=m_head;

    BOOL found=FALSE;
    if(m_head->element==elem)
    {
        m_head=m_head->next;
        delete current;
        current=NULL;
        return RETURN_SUCCESS;
    }
    while(!found&&current!=NULL)
    {
        if(current->element==elem)
        {
            found=TRUE;
            continue;
        }
        temp=current;
        current=current->next;
    }

    if(current==NULL)
        return OPERATOR_FAILED;

#if 0 //This method will leave a zero value... Why?
    if(current->next!=NULL)
    {
        printf("Next!=NULL, elem=%d\n",elem);
        current->element=current->next->element;
        temp=current->next;
        current->next=current->next->next;
        delete temp;
        temp=NULL;
    }
    else
    {
        printf("Next==NULL, elem=%d\n",elem);
        temp=current;
        delete current;
        current=NULL;
    }
    return RETURN_SUCCESS;
#else
    if(current->next!=NULL)
    {
        temp->next=current->next;
    }
    else
    {
        temp->next=NULL;
    }
    delete current;
    return RETURN_SUCCESS;

#endif /* Modify by Amos.zhu */
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

    memcpy(elem,&m_pSearch->element,sizeof(type));
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
    AM_U32 count=0;
    node_t<type>* current;
    if(!this->IsEmpty())
    {
        current=m_head;
        while(current!=NULL)
        {
            count++;
            current=current->next;
        }
    }
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
    return;
}

template<class type>
void CList<type>::PrintOut(void)
{
    if(this->IsEmpty())
    {
        std::cout<<"It is an empty list!"<<std::endl;
    }

    node_t<type>* current;

    for(current=this->m_head; current!=NULL; current=current->next)
    {
        std::cout<<current->element<<" ";
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
        output<<current->element<<" ";
    }
    output<<std::endl;
    return output;
}

#endif
