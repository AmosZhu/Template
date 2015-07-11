#ifndef _CDISJOINT_SET_HPP_
#define _CDISJOINT_SET_HPP_

#include <map>
#include <vector>

/*****************************************************************************
*
*   @ Author: Amos.Zhu (DIZHONG ZHU)
*   @ Revised: 2015/7/11
*
*   @ Visit my github: https://github.com/AmosZhu
*
*****************************************************************************/

template<class type>
class CDisjointSet
{
public:
    CDisjointSet(void);
    /***************************************************************
    *
    *   @ Pre-store all the data into the set
    *     Construct is directly or call
    *     Add and Initialize seperately.
    *
    ***************************************************************/
    CDisjointSet(type* elemArray,int len);
    CDisjointSet(const CDisjointSet<type>& other);
    CDisjointSet(CDisjointSet<type>&& other);
    ~CDisjointSet(void);

    void Initialize(void);
    bool Add(type& elem);

    int Count(void) const;

    type Find(type& elem);
    type Union(type& e1,type& e2);

private:
    int find(int idx);
    /*
    *   root1 and root2 are parent node!!!
    */
    int setUnion(int root1,int root2);


protected:
    std::vector<int> m_disjSet; //Store the root, default -1;
    std::map<type,int> m_type2idx;  //Type to Index
    std::map<int,type> m_idx2type;  //Index to Type
    int m_count=0;
};


template<class type>
CDisjointSet<type>::CDisjointSet(void)
{
    m_disjSet.clear();
    m_type2idx.clear();
    m_idx2type.clear();
}

template<class type>
CDisjointSet<type>::CDisjointSet(type* elemArray,int len)
{
    int idx;
    if(elemArray==nullptr||len==0)
    {
        return;
    }

    for(idx=0; idx<len; idx++)
    {
        m_type2idx.insert(std::pair<type,int>(elemArray[idx],m_count));
        m_idx2type.insert(std::pair<int,type>(m_count,elemArray[idx]));
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
    std::vector<int>::iterator it=m_disjSet.begin();
    for(; it!=m_disjSet.end(); it++)
    {
        *it=-1;
    }
}

template<class type>
bool CDisjointSet<type>::Add(type& elem)
{
    if(elem==nullptr)
    {
        return false;
    }

    m_disjSet.push_back(-1);
    m_type2idx.insert(std::pair<type,int>(elem,m_count));
    m_idx2type.insert(std::pair<int,type>(m_count,elem));
    m_count++;
    return true;
}

template<class type>
int CDisjointSet<type>::Count(void) const
{
    return m_count;
}

template<class type>
type CDisjointSet<type>::Find(type& elem)
{
    int idx;
    int root;
    type result;

    idx=m_type2idx[elem];
    if(m_disjSet[idx]<=0)
    {
        root=idx;
    }
    else
    {
        root=find(idx);
        /*
        *   Path compression
        */
        m_disjSet[idx]=root;
    }
    result=m_idx2type[root];

    return result;
}

template<class type>
int CDisjointSet<type>::find(int idx)
{
    int root;
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
type CDisjointSet<type>::Union(type& e1,type& e2)
{
    int idx1,idx2;
    int root1,root2;
    int idx;
    type result;
    idx1=m_type2idx[e1];
    idx2=m_type2idx[e2];
    root1=find(idx1);
    root2=find(idx2);
    if(root1==root2)
    {
        result=m_idx2type[root1];
    }
    else
    {
        idx=setUnion(root1,root2);
        result=m_idx2type[idx];
    }
    return result;
}

template<class type>
int CDisjointSet<type>::setUnion(int root1,int root2)
{
    int root;
    if(m_disjSet[root1]<m_disjSet[root2])
    {
        m_disjSet[root1]+=m_disjSet[root2];  //Negative value means how many sons he has, -2 represent it has two son
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
