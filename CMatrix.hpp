/*************************************************************************************
*   Author: Amos.Zhu
*
*   1.Amos used it to implement his cryptography algorithm
*   2.This is a Matrix template(Howerver, we always use with the type of integer)
*
*
*   2013-7-2
*
*
*************************************************************************************/


#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#define MAXSIZE 100

#include <iostream>
#include <iomanip>


#define IsInvalid(Matrix) {\
    if(Matrix.m_data==NULL\
        ||Matrix.m_row==0\
        ||Matrix.m_column==0)\
        return RET_INVALIDE;}

typedef enum ERROR_TYPE
{
    RET_SUCCESS,
    RET_FAILED,
    RET_INVALIDE,
} Err_t;

typedef enum DEBUG_LEVEL
{
    DBG_NONE=0,
    DBG_INFO,
    DBG_WARNING,
    DBG_ERROR,
} DEBUG_T;


template<typename type>
class CMatrix
{
public:
    CMatrix(void);
    CMatrix(int m,int n);
    CMatrix(const CMatrix<type>& otherMatrix);
    ~CMatrix(void);

    Err_t GetDataFrom(type* table);

public: //Matrix operations
#if 0 //Wait to implement tommorow
    bool IsSingular(void);
    type DetValue(void);
    Err_t SetValue(int m,int n);
#endif /* Modify by Amos.zhu */
    Err_t Elimination(const CMatrix<type>& Matrix_A,CMatrix<type>& Matrix_L,CMatrix<type>& Matrix_U);


    CMatrix<type>& operator=(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator+(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator-(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator*(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator+=(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator-=(const CMatrix<type>& otherMatrix);
    CMatrix<type>& operator*=(const CMatrix<type>& otherMatrix);

public:
    int NoOfColumns(void);
    int NoOfRows(void);
    void Destroy(void);
    Err_t ShowMatrix(void);
    static void SetDbgLVL(DEBUG_T level);
    //inline Err_t IsIvalid(const CMatrix<type>& Matrix);

private:
    type* m_data;
    int m_column;
    int m_row;
    static DEBUG_T DBG_LVL;

private://Function
    void copy(CMatrix<type>& Dst,const CMatrix<type>& Src);
    bool check_pivot(int m,int n,CMatrix<type>& Matrix_A);
    void rowExchange(int to,int from,CMatrix<type>& matrix);
    void columnExchange(int to,int from,CMatrix<type>& matrix);


public: //friend function
    template<typename type> friend std::ostream& operator<<(std::ostream& output,const CMatrix<type>& matrix);



};


template<typename type>
DEBUG_T CMatrix<type>::DBG_LVL=DBG_NONE;

template<typename type>
CMatrix<type>::CMatrix(void)
{
    m_column=0;
    m_row=0;
    m_data=NULL;
}

template<typename type>
CMatrix<type>::CMatrix(int m,int n)
{
    if(m==0||n==0)
        std::cout<<"Invalide columns or row!"<<std::endl;

    m_row=m;
    m_column=n;

    m_data=new type[m_column*m_row];
    memset(m_data,0,(sizeof(type)*m_column*m_row));
    return;
}

template<typename type>
CMatrix<type>::CMatrix(const CMatrix<type>& otherMatrix)
{
    if(this==&otherMatrix)
        return;

    if(otherMatrix.m_column==0\
       ||otherMatrix.m_row==0\
       ||otherMatrix.m_data==NULL)
    {
        m_data=NULL;
        m_column=0;
        m_row=0;
        return;
    }

    copy(*this,otherMatrix);
    return;
}

template<typename type>
CMatrix<type>::~CMatrix(void)
{
    Destroy();
}

#if 0
template<typename type>
inline Err_t CMatrix<type>::IsIvalid(const CMatrix<type>& Matrix)
{
    if(Matrix.m_data==NULL\
       ||Matrix.m_row==0\
       ||Matrix.m_column==0)
        return RET_INVALIDE;
}
#endif /* Modify by Amos.zhu */


template<typename type>
Err_t CMatrix<type>::GetDataFrom(type* table)
{
    if(table==NULL)
        return RET_FAILED;

    memcpy(m_data,table,sizeof(type)*m_column*m_row);

    return RET_SUCCESS;
}



template<typename type>
CMatrix<type>& CMatrix<type>::operator=(const CMatrix<type>& otherMatrix)
{
    if(this==&otherMatrix)
        return *this;

    if(otherMatrix.m_column==0\
       ||otherMatrix.m_row==0\
       ||otherMatrix.m_data==NULL)
    {
        m_data=NULL;
        m_column=0;
        m_row=0;
        return *this;
    }

    this->Destroy();
    copy(*this,otherMatrix);

    return *this;
}

template<typename type>
CMatrix<type>& CMatrix<type>::operator+(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        return *this;
    }

    if(otherMatrix.m_column!=this->m_column\
       ||otherMatrix.m_row!=this->m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
        {
            std::cout<<"Matrices are not same size, cannot do add operation!"<<std::endl;
        }
        return *this;
    }

    CMatrix<type>* temp_Matrix=new CMatrix<type>(m_column,m_row);

    for(int i=0; i<m_column*m_row; i++)
        temp_Matrix->m_data[i]=this->m_data[i]+otherMatrix.m_data[i];

    return *temp_Matrix;
}

template<typename type>
CMatrix<type>& CMatrix<type>::operator-(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        return *this;
    }

    if(otherMatrix.m_column!=this->m_column\
       ||otherMatrix.m_row!=this->m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
        {
            std::cout<<"Matrices are not same size, cannot do subtract operation!"<<std::endl;
            return *this;
        }
    }

    CMatrix<type>* temp_Matrix=new CMatrix<type>(m_column,m_row);

    for(int i=0; i<m_column*m_row; i++)
        temp_Matrix->m_data[i]=this->m_data[i]-otherMatrix.m_data[i];

    return *temp_Matrix;
}

template<typename type>
CMatrix<type>& CMatrix<type>::operator*(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"Cannot multiply an empty matrix"<<std::endl;
        return *this;
    }

    if(this->m_column!=otherMatrix.m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"left matrix's m_column not equal to right matrix's row"<<std::endl;
        return *this;
    }

    CMatrix<type>* temp_Matrix=new CMatrix<type>(this->m_row,otherMatrix.m_column);
    int swap_data=0;
    for(int i=0; i<temp_Matrix->m_row; i++)
    {
        for(int j=0; j<temp_Matrix->m_column; j++)
        {
            for(int k=0; k<this->m_column; k++)
            {
                swap_data+=this->m_data[i*this->m_column+k]*otherMatrix.m_data[k*otherMatrix.m_column+j];
                if(DBG_LVL>=DBG_INFO)
                {
                    std::cout<<"i="<<i<<",j="<<j<<",k="<<k<<std::endl;
                    std::cout<<"swap_data="<<this->m_data[i*this->m_column+k]<<"*"<<otherMatrix.m_data[k*otherMatrix.m_column+j]<<std::endl;
                }
            }
            temp_Matrix->m_data[i*temp_Matrix->m_column+j]=swap_data;
            swap_data=0;
        }
    }

    return *temp_Matrix;
}


template<typename type>
CMatrix<type>& CMatrix<type>::operator+=(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        return *this;
    }

    if(otherMatrix.m_column!=this->m_column\
       ||otherMatrix.m_row!=this->m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
        {
            std::cout<<"Matrices are not same size, cannot do add operation!"<<std::endl;
        }
        return *this;
    }

    for(int i=0; i<m_column*m_row; i++)
        this->m_data[i]+=otherMatrix.m_data[i];

    return *this;
}

template<typename type>
CMatrix<type>& CMatrix<type>::operator-=(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        return *this;
    }

    if(otherMatrix.m_column!=this->m_column\
       ||otherMatrix.m_row!=this->m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
        {
            std::cout<<"Matrices are not same size, cannot do subtract operation!"<<std::endl;
            return *this;
        }
    }


    for(int i=0; i<m_column*m_row; i++)
        this->m_data[i]-=otherMatrix.m_data[i];

    return *this;
}

template<typename type>
CMatrix<type>& CMatrix<type>::operator*=(const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_column==0||this->m_column==0\
       ||otherMatrix.m_row==0||this->m_row==0\
       ||otherMatrix.m_data==NULL||this->m_data==NULL)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"Cannot multiply an empty matrix"<<std::endl;
        return *this;
    }

    if(this->m_column!=otherMatrix.m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"left matrix's column not equal to right matrix's row"<<std::endl;
        return *this;
    }

    CMatrix<type>* temp_Matrix=new CMatrix<type>(this->m_row,otherMatrix.m_column);
    int swap_data=0;
    for(int i=0; i<temp_Matrix->m_row; i++)
    {
        for(int j=0; j<temp_Matrix->m_column; j++)
        {
            for(int k=0; k<this->m_column; k++)
            {
                swap_data+=this->m_data[i*this->m_column+k]*otherMatrix.m_data[k*otherMatrix.m_column+j];
                if(DBG_LVL>=DBG_INFO)
                {
                    std::cout<<"i="<<i<<",j="<<j<<",k="<<k<<std::endl;
                    std::cout<<"swap_data="<<this->m_data[i*this->m_column+k]<<"*"<<otherMatrix.m_data[k*otherMatrix.m_column+j]<<std::endl;
                }
            }
            temp_Matrix->m_data[i*temp_Matrix->m_column+j]=swap_data;
            swap_data=0;
        }
    }
    copy(*this,*static_cast<const CMatrix<type>* >(temp_Matrix));
    return *this;
}



template<typename type>
int CMatrix<type>::NoOfColumns(void)
{
    return m_column;
}

template<typename type>
int CMatrix<type>::NoOfRows(void)
{
    return m_row;
}

template<typename type>
void CMatrix<type>::Destroy(void)
{
    if((m_data==NULL)||(m_column<=0)||(m_row<=0))
    {
        if(DBG_LVL>=DBG_WARNING)
            std::cout<<"No data destroy!"<<std::endl;
        return;
    }

    delete[] m_data;
    m_column=0;
    m_row=0;

    if(DBG_LVL>=DBG_WARNING)
        std::cout<<"Destroy end!"<<std::endl;

}

template<typename type>
Err_t CMatrix<type>::ShowMatrix(void)
{
    if(m_data==NULL||m_column==0||m_row==0)
        return RET_INVALIDE;

    std::cout<<std::endl;
    std::cout<<"Matrix size = "<<m_row<<"*"<<m_column<<std::endl;

    for(int i=0; i<m_row; i++)
    {
        for(int j=0; j<m_column; j++)
        {
            std::cout<<std::setw(10)<<setiosflags(std::ios::left)<<m_data[i*m_column+j]<<" ";
        }
        std::cout<<std::endl;
    }

    return RET_SUCCESS;
}

template<typename type>
void CMatrix<type>::copy(CMatrix<type>& Dst,const CMatrix<type>& Src)
{
    Dst.m_column=Src.m_column;
    Dst.m_row=Src.m_row;
    Dst.m_data=new type[m_column*m_row];
    memcpy(Dst.m_data,Src.m_data,sizeof(type)*Dst.m_column*Dst.m_row);
}


template<typename type>
void CMatrix<type>::SetDbgLVL(DEBUG_T level)
{
    DBG_LVL=level;
}

template<typename type>
void CMatrix<type>::rowExchange(int to,int from,CMatrix<type>& matrix)
{
    if(to>matrix.m_row||from>matrix.m_row)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"Caution its size,row="<<m_row<<std::endl;
        return;
    }
    int len=sizeof(type)*matrix.m_column;
    type* temp=new type[matrix.m_column];
    memcpy(temp,&matrix.m_data[to*m_column],len);
    memcpy(&matrix.m_data[to*m_column],&matrix.m_data[from*m_column],len);
    memcpy(&matrix.m_data[from*m_column],temp,len);
    if(DBG_LVL>=DBG_ERROR)
    {
        std::cout<<"change "<<from<<" and "<<to<<std::endl;
        matrix.ShowMatrix();
    }
    return;

}

template<typename type>
void CMatrix<type>::columnExchange(int to,int from,CMatrix<type>& matrix)
{
    if(to-->matrix.m_column||from-->matrix.m_column)
    {
        if(DBG_LVL>=DBG_ERROR)
            std::cout<<"Caution its size,column="<<m_column<<std::endl;
        return;
    }

    type temp;
    for(int i=0; i<matrix.m_row; i++)
    {
        temp=matrix.m_data[i*m_column+to];
        matrix.m_data[i*m_column+to]=matrix.m_data[i*m_column+from];
        matrix.m_data[i*m_column+from]=temp;
    }
}

template<typename type>
bool CMatrix<type>::check_pivot(int m,int n,CMatrix<type>& Matrix_A)
{
    if(Matrix_A.m_data[m*m_column+n]!=0)
        return true;
    else
        return false;
}

/*************************************************************************************
*       The function of elimnation
*
*   Sample Matrix:
*
*   1.
*   ===================
*   |3  2   3   4   5|| <-pivot_Row
*   |4  5   6   7   8|| <-temp_Row*3/4
*   |9  10  2   4   5||
*   ===================
*
*   2.
*   ======================
*   |3    2    3    4   5||
*   |3  15/4 18/4 21/4  6||   then temp_row minus pivot_row
*   |9    10   2    4   5||
*   ======================
*
*   3.
*   ====================
*   |3   2   3   4   5||
*   |0  7/4 6/4 5/4  1||     after elimination,do same for the 3rd row;
*   |9  10   2   4   5||
*   ===================
*
*   If we want to do elmination, temp_row must mutiple 3 then
*   divde 4 to ensure temp_row can do elimination from pivot_row.
*   Some times the type is float, that mean: even my temp_row has been subtracted
*   by pivot row,but it cannot ensure temp_row's column is 0,so assign 0 intentionly.
**************************************************************************************/


template<typename type>
Err_t CMatrix<type>::Elimination(const CMatrix<type>& Matrix_A,CMatrix<type>& Matrix_L,CMatrix<type>& Matrix_U)
{
    IsInvalid(Matrix_A);

    CMatrix<type>* U=new CMatrix<type>;
    CMatrix<type>* P=new CMatrix<type>;

    type* temp_row; //Use to store the temp row data

    copy(*U,Matrix_A);

    int m=0,n=0;
    int pivot_row=-1;
    type divident;
    type multiple;
    memset(&divident,0,sizeof(type));
    memset(&multiple,0,sizeof(type));
    for(n=0; n<m_column; n++)
    {
        pivot_row++;
        if(DBG_LVL>=DBG_WARNING)
        {
            std::cout<<"pivot_row="<<pivot_row<<std::endl;
        }

        /*****************************************************
        *   Check pivot,if no valid pivot,do row exchange
        *****************************************************/
        for(m=n; m<m_row; m++)
        {
            if(DBG_LVL>=DBG_WARNING)
            {
                std::cout<<"m="<<m<<", n="<<n<<", pivot_row="<<pivot_row<<std::endl;
            }
            if(!check_pivot(m,n,*U))
                continue;
            if(m!=pivot_row)
            {
                rowExchange(pivot_row,m,*U);
            }
            break; //Find the first valid pivot,do row exchange then break loop
        }
        /******************************************************
        *   Eliminate step here
        ******************************************************/
        if(!check_pivot(pivot_row,n,*U))
            continue;
        divident=U->m_data[pivot_row*U->m_column+n];

        for(m=n+1; m<m_row; m++)
        {
            if(!check_pivot(m,n,*U))
                continue;

            temp_row=new type[U->m_column-n];
            multiple=U->m_data[m*U->m_column+n];
            memcpy(temp_row,&U->m_data[pivot_row*U->m_column+n],sizeof(type)*(U->m_column-n));
            if(DBG_LVL>=DBG_WARNING)
            {
                for(int count=0; count<U->m_column-n; count++)
                    std::cout<<temp_row[count]<<" ";
                std::cout<<std::endl;
            }
            for(int i=0; i<(U->m_column-n); i++)
            {
                (U->m_data[m*U->m_column+n+i])-=(temp_row[i]*multiple/divident);
            }
            U->m_data[m*U->m_column+n]=0;
            delete[] temp_row;
            if(DBG_LVL>=DBG_WARNING)
            {
                U->ShowMatrix();
            }

        }
    }

    copy(Matrix_U,*U);

    delete U;
    delete P;
    return RET_SUCCESS;
}

template<typename type>
std::ostream& operator<<(std::ostream& output,const CMatrix<type>& otherMatrix)
{
    if(otherMatrix.m_data==NULL||otherMatrix.m_column==0||otherMatrix.m_row==0)
        return output;

    output<<std::endl;
    output<<"Matrix size = "<<otherMatrix.m_row<<"*"<<otherMatrix.m_column<<std::endl;
    for(int i=0; i<otherMatrix.m_row; i++)
    {
        for(int j=0; j<otherMatrix.m_column; j++)
        {
            output<<std::setw(10)<<setiosflags(std::ios::left)<<otherMatrix.m_data[i*otherMatrix.m_column+j]<<" ";
        }
        output<<std::endl;
    }

    return output;
}


#endif
