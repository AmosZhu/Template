#include "CPolynomialHeap_Test.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223,100};
AM_U32 data1[] = {56,123,555,55,25,90};

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

    CPolynomialHeap<AM_U32> heap1,heap2;
    CPolynomialHeap<AM_U32>* heap3;

    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(heap1.Add(&data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }

    for(i=0; i<sizeof(data1)/sizeof(AM_U32); i++)
    {
        if(heap2.Add(&data1[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }

    heap1.PrintOut();

    printf("=================>Heap2<==================\n");
    heap2.PrintOut();

    printf("=================>Merge<==================\n");

    heap3=CPolynomialHeap<AM_U32>::Merge(&heap1,&heap2);
    if(heap3!=nullptr)
        heap3->PrintOut();

    printf("=================>Heap4<==================\n");
    CPolynomialHeap<AM_U32> heap4(heap1);
    heap4.PrintOut();

    printf("=================>Heap5,1<==================\n");
    CPolynomialHeap<AM_U32> heap5(std::move(heap1));
    heap5.PrintOut();
    heap1.PrintOut();

    printf("=================>Heap4<==================\n");
    heap4=*heap3;
    heap4.PrintOut();

    printf("=================>Heap1,4<==================\n");

    heap1=std::move(heap4);
    heap1.PrintOut();
    heap4.PrintOut();




}
