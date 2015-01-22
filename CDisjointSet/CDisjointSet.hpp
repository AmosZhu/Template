#ifndef _CDISJOINT_SET_HPP_
#define _CDISJOINT_SET_HPP_

#include "AmosType.hpp"

#include <map>
#include <vector>

template<class type>
class CDisjointSet
{
public:
    CDisjointSet(void);
    CDisjointSet(type* elemArray,AM_U32 len);
    CDisjointSet(const CDisjointSet<type>& other);
    CDisjointSet(CDisjointSet<type>&& other);
    ~CDisjointSet(void);

    void Initialize(void);
    Err_t Add(type* elem);

    AM_U32 Count(void) const;
    type Find(type* elem);
    type Union(type* e1,type* e2);

public:
    static void SetCompareFunc(cmp_t (*func)(type* k1,type* k2));
    static void SetCopyFunc(void (*func)(type* dst,type* src));

private:
    AM_S32 find(AM_S32 idx);
    AM_S32 setUnion(AM_S32 root1,AM_S32 root2);

protected:
    static cmp_t (*m_cmpRoute)(type* k1, type* k2);
    static void (*m_cpyRoute)(type* dst,type* src);


protected:
    std::vector<AM_S32> m_disjSet;
    std::map<type,AM_U32> m_type2idx;
    std::map<AM_U32,type> m_idx2type;
    AM_U32 m_count=0;
};

template<class type>
void (*CDisjointSet<type>::m_cpyRoute)(type* dst,type* src)=nullptr;

template<class type>
cmp_t (*CDisjointSet<type>::m_cmpRoute)(type* k1,type* k2)=nullptr;

template<class type>
CDisjointSet<type>::CDisjointSet(void)
{
    m_disjSet.clear();
    m_type2idx.clear();
    m_idx2type.clear();
}

template<class type>
CDisjointSet<type>::CDisjointSet(type* elemArray,AM_U32 len)
{
    AM_U32 idx;
    if(elemArray==nullptr||len==0)
    {
        return;
    }

    for(idx=0; idx<len; idx++)
    {
        m_type2idx.insert(std::pair<type,AM_U32>(elemArray[idx],m_count));
        m_idx2type.insert(std::pair<AM_U32,type>(m_count,elemArray[idx]));
        m_disjSet.push_back(-1);
        m_count++;
    }
}

template<class type>
CDisjointSet<type>::~CDisjointSet(void)
{
    m_disjSet.clear();
    m_type2idx.clear();
    m_idx2type.clear();
    m_count=0;
}

template<class type>
void CDisjointSet<type>::Initialize(void)
{
    std::vector<AM_S32>::iterator it=m_disjSet.begin();
    for(; it!=m_disjSet.end(); it++)
    {
        *it=-1;
    }
}

template<class type>
Err_t CDisjointSet<type>::Add(type* elem)
{
    if(elem==nullptr)
    {
        return INVALIDE_PARAMET;
    }

    m_disjSet.push_back(-1);
    m_type2idx.insert(std::pair<type,AM_U32>(*elem,m_count));
    m_idx2type.insert(std::pair<AM_U32,type>(m_count,*elem));
    m_count++;
    return RETURN_SUCCESS;
}

template<class type>
void CDisjointSet<type>::SetCompareFunc(cmp_t (*func)(type* k1,type* k2))
{

    if(func==nullptr)
        return;

    m_cmpRoute=func;

}

template<class type>
void CDisjointSet<type>::SetCopyFunc(void (*func)(type* dst,type* src))
{
    if(func==nullptr)
        return;

    m_cpyRoute=func;

}

template<class type>
AM_U32 CDisjointSet<type>::Count(void) const
{
    return m_count;
}

template<class type>
type CDisjointSet<type>::Find(type* elem)
{
    AM_U32 idx;
    AM_S32 root;
    type result;

    idx=m_type2idx[*elem];
    if(m_disjSet[idx]<=0)
    {
        root=idx;
    }
    else
    {
        root=find(idx);
        m_disjSet[idx]=root;
    }
    m_cpyRoute(&result,&m_idx2type[root]);

    return result;
}

template<class type>
AM_S32 CDisjointSet<type>::find(AM_S32 idx)
{
    AM_S32 root;
    root=m_disjSet[idx];
    if(root>=0)
    {
        return find(m_disjSet[idx]);
    }
    else
    {
        return idx;
    }
}

template<class type>
type CDisjointSet<type>::Union(type* e1,type* e2)
{
    AM_S32 idx1,idx2;
    AM_S32 root1,root2;
    AM_S32 idx;
    type result;
    idx1=m_type2idx[*e1];
    idx2=m_type2idx[*e2];
    root1=find(idx1);
    root2=find(idx2);
    if(root1==root2)
    {
        m_cpyRoute(&result,&m_idx2type[root1]);
    }
    else
    {
        idx=setUnion(root1,root2);
        m_cpyRoute(&result,&m_idx2type[idx]);
    }
    return result;
}

template<class type>
AM_S32 CDisjointSet<type>::setUnion(AM_S32 root1,AM_S32 root2)
{
    AM_S32 root;
    if(m_disjSet[root1]<m_disjSet[root2])
    {
        m_disjSet[root1]+=m_disjSet[root2];
        m_disjSet[root2]=root1;
        root=root1;
    }
    else
    {
        m_disjSet[root2]+=m_disjSet[root1];
        m_disjSet[root1]=root2;
        root=root2;
    }
    return root;
}

#endif
