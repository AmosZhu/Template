#include "CLeftHeap.hpp"
#include "LeftHeap_UT.hpp"
#include <string.h>

AM_S32 data[] = {13,55,43,4,123,98,67,444,777,223};
AM_S32 data1[] = {90,22,45,76,33,101};

static void printNode(AM_S32* node)
{
    printf("[%d]",*node);
}

static cmp_t cmpFunc(AM_S32* arg1,AM_S32* arg2)
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

static void copyFunc(AM_S32* dst,AM_S32* src)
{
    memcpy(dst,src,sizeof(AM_S32));
}

void LeftHeap_UT()
{
    AM_U32 idx,count;
    AM_S32 minData;
    CLeftHeap<AM_S32> heap,heap1,heap2;
    heap.SetCompareFunc(cmpFunc);
    heap.SetCopyFunc(copyFunc);
    heap.SetPrintFunc(printNode);

    heap1.SetCompareFunc(cmpFunc);
    heap1.SetCopyFunc(copyFunc);
    heap1.SetPrintFunc(printNode);

    for(idx=0; idx<(sizeof(data)/sizeof(AM_S32)); idx++)
    {
        heap.Insert(&data[idx]);
    }

    for(idx=0; idx<(sizeof(data1)/sizeof(AM_S32)); idx++)
    {
        heap1.Insert(&data1[idx]);
    }

    printf("===================>> Heap <<=========================\n");
    heap.PrintOut();
    printf("===================>> Heap1 <<=========================\n");
    heap1.PrintOut();

    heap2=heap;
    CLeftHeap<AM_S32> heap3(heap1);

    heap.Merge(&heap1);
    printf("===================>> Heap Merge With Heap1 <<=========================\n");
    heap.PrintOut();

    printf("===================>> After Merge Heap1 <<=========================\n");
    heap1.PrintOut();

    printf("===================>> Heap2 <<=========================\n");
    heap2.PrintOut();
    printf("===================>> Heap3 <<=========================\n");
    heap3.PrintOut();

    printf("===================>> Size of the heaps <<=========================\n");
    printf("heap : %d\n",heap.ElemCount());
    printf("heap1 : %d\n",heap1.ElemCount());
    printf("heap2 : %d\n",heap2.ElemCount());
    printf("heap3 : %d\n",heap3.ElemCount());

    count=heap.ElemCount();

    for(idx=count; idx>0; idx--)
    {
        heap.DeleteMin(&minData);
        printf("===================>> %d time delete <<=========================\n",count-idx);
        printf("minData = %d\n",minData);
        heap.PrintOut();
    }

}
