#include "CBinaryHeap.hpp"
#include "Heap_Test.hpp"
#include <string.h>

AM_S32 data[] = {13,55,43,4,123,98,67,444,777,223};

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

static BOOL isDummyFunc(AM_S32 *src)
{
    if(*src==-1)
        return TRUE;

    return FALSE;
}

static void dummyFunc(AM_S32* src)
{
    *src=-1;
}

void BinaryHeap_UT()
{
    AM_U32 idx;
    AM_S32 minData;
    CBinaryHeap<AM_S32> heap(32);
    heap.SetCompareFunc(cmpFunc);
    heap.SetCopyFunc(copyFunc);
    heap.SetPrintFunc(printNode);
    heap.SetDummyFunc(dummyFunc,isDummyFunc);

    heap.Initialize();
    for(idx=0; idx<(sizeof(data)/sizeof(AM_S32)); idx++)
    {
        heap.Insert(&data[idx]);
    }

    heap.PrintOut();

    CBinaryHeap<AM_S32> heap1(heap);
    CBinaryHeap<AM_S32> heap2;
    std::cout<<heap1;

    heap2=heap;

    for(idx=0; idx<5; idx++)
    {
        heap2.DeleteMin(&minData);
        std::cout<<idx<<". Delete Min: "<<minData<<std::endl;
    }

    std::cout<<"heap2 has "<<heap2.ElemCount()<<" elements!"<<std::endl;

    std::cout<<heap2;

    heap.Destroy();
    heap.PrintOut();
    heap1.Destroy();
    heap1.PrintOut();
}
