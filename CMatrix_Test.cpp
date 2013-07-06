#include "CMatrix_Test.hpp"


/***************        data here   *****************************/
double data1[16]= {1,1,1,1, 2,2,2,2, 43,56,78,9, 4,4,4,4};
double data2[16]= { 7.7,8.8,9.9,0.1, 1.2,3.3,4.4,5.5, 1,2,3,4, 2,4,5,6,};
int data3[4]= {1,1,2,2};
int data4[9]= {4,2,1,5,6,2,7,3,9};
int data5[8]= {1,1,1,1,2,2,2,4};

int MATRIX_Elimination_Test(void)
{
    CMatrix<double> matrix_A(4,4);
    matrix_A.GetDataFrom(data2);
    CMatrix<double> matrix_U;
    CMatrix<double> matrix_L;
    matrix_A.Elimination(matrix_L,matrix_U);
    matrix_U.ShowMatrix();

    return 0;
}

int MATRIX_Mutiple_Test(void)
{
    CMatrix<double> matrix_A(4,4);
    matrix_A=static_cast<double>(2)*matrix_A;
    matrix_A.ShowMatrix();

    matrix_A.GetDataFrom(data1);
    matrix_A=static_cast<double>(3)*matrix_A;
    matrix_A.ShowMatrix();
    return 0;
}

int MATRIX_Mutiple_Test1(void)
{
    CMatrix<int> matrix1(2,2);
    matrix1.GetDataFrom(data3);

    CMatrix<int> matrix2(2,4);
    matrix2.GetDataFrom(data5);

    CMatrix<int> matrix3=matrix1*matrix2;
    std::cout<<matrix3;
    return 0;
}

int MATRIX_Cofactor_Test(void)
{
    //CMatrix<double>::SetDbgLVL(DBG_ERROR);
    CMatrix<double> matrix_A(4,4,data1);
    matrix_A.ShowMatrix();
	CMatrix<double> matrix_co;
	matrix_A.Cofactor(matrix_co,1,2);
	matrix_co.ShowMatrix();

	return 0;
}


