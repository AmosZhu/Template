#include "CPolynomialHeap_Test.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223,100};

static void printNode(AM_U32* node)
{
    printf("[%d]\n",*node);
}

static cmp_t cmpFunc(AM_U32* arg1,AM_U32* arg2)
{
    if((arg1==NULL)||(arg2==NULL))
        return INVALID;

    if(*arg1==*arg2)
        return EQUAL;
    else if(*arg1>*arg2)
        return LARGER;
    else if(*arg1<*arg2)
        return SMALLER;
}

static void copyFunc(AM_U32* dst,AM_U32* src)
{
    memcpy(dst,src,sizeof(AM_U32));
}


void BasicTest(void)
{
    CPolynomialHeap<AM_U32>::SetPrintFunc(printNode);
    CPolynomialHeap<AM_U32>::SetCompareFunc(cmpFunc);
    CPolynomialHeap<AM_U32>::SetCopyFunc(copyFunc);

    CPolynomialHeap<AM_U32> heap1;


    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(heap1.Add(&data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }

    heap1.PrintOut();

}
