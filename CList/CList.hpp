#ifndef _AMOS_CLIST_H
#define _AMOS_CLIST_H


typedef enum ERROR_TYPE
{
    RETURN_SUCCESS=0,
    OPERATOR_FAILED=1,
    INVALIDE_PARAMET=2,
} Err_t;

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


    CList<type>& operator=(const CList<type>& object);
    Err_t Insert(type* elem);
#if 0
    Err_t Delete(type* elem);
    Err_t Delete(int pos);
#endif /* Modify by Amos.zhu */
    Err_t Search(type* elem);
    BOOL IsEmpty(void);
    void Destroy(void);

private:
    node_t* m_head;
    node_t* m_tail;

private:
    Err_t copy(const CList<type>& object);
};


template<typename type>
CList<type>::CList(void)
{
    m_head=NULL;
    m_tail=NULL;
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

    return;
}

template<typename type>
CList<type>::~CList(void)
{
    Destroy();
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
Err_t CList<type>::Insert(type* elem)
{
    if(elem==NULL)
        return INVALIDE_PARAMET;

    node_t<type>* newNode=new node_t<type>;
    newNode->element=*elem;
    newNode->next=NULL;

    if(m_head==NULL||m_tail==NULL) //Nothing in CList,put the elem as first node;
    {
        m_head=newNode;
        m_tail=newNode;
    }

    m_tail->next=newNode;
    m_tail=newNode;

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
BOOL CList<type>::IsEmpty(void)
{
    if(m_head==NULL||m_tail==NULL)
        return true;

    return false;
}

template<typename type>
void CList<type>::Destroy(void)
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

#endif
