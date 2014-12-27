#include "SortingAlgorithm_Test.hpp"
#include "CSortingAlgorithm.hpp"


AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223,100,290,1000,999,276};

static cmp_t cmpFunc(AM_U32* arg1,AM_U32* arg2)
{
    if((arg1==NULL)||(arg2==NULL))
        return INVALID;

    if(*arg1==-1||*arg2==-1)
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

static BOOL isDummyFunc(AM_U32 *src)
{
    if(*src==-1)
        return TRUE;

    return FALSE;
}

static void dummyFunc(AM_U32* src)
{
    *src=-1;
}

void InsertionSort_UT(void)
{
    AM_U32 idx;
    CSortingAlgorithm<AM_U32>::SetCopyFunc(copyFunc);
    CSortingAlgorithm<AM_U32>::SetCompareFunc(cmpFunc);

    CSortingAlgorithm<AM_U32>::InsertionSort(data,sizeof(data)/sizeof(AM_U32));

    for(idx=0; idx<sizeof(data)/sizeof(AM_U32); idx++)
    {
        printf("%d ",data[idx]);
    }

    printf("\n");
}

void ShellSort_UT(void)
{
    AM_U32 idx;
    CSortingAlgorithm<AM_U32>::SetCopyFunc(copyFunc);
    CSortingAlgorithm<AM_U32>::SetCompareFunc(cmpFunc);

    CSortingAlgorithm<AM_U32>::ShellSort(data,sizeof(data)/sizeof(AM_U32));

    for(idx=0; idx<sizeof(data)/sizeof(AM_U32); idx++)
    {
        printf("%d ",data[idx]);
    }

    printf("\n");

}

void HeapSort_UT(void)
{
    AM_U32 idx;
    CSortingAlgorithm<AM_U32>::SetCopyFunc(copyFunc);
    CSortingAlgorithm<AM_U32>::SetCompareFunc(cmpFunc);
    CSortingAlgorithm<AM_U32>::SetDummyFunc(dummyFunc,isDummyFunc);

    CSortingAlgorithm<AM_U32>::HeapSort(data,sizeof(data)/sizeof(AM_U32));

    for(idx=0; idx<sizeof(data)/sizeof(AM_U32); idx++)
    {
        printf("%d ",data[idx]);
    }

    printf("\n");
}

void MergeSort_UT(void)
{
    AM_U32 idx;
    CSortingAlgorithm<AM_U32>::SetCopyFunc(copyFunc);
    CSortingAlgorithm<AM_U32>::SetCompareFunc(cmpFunc);

    CSortingAlgorithm<AM_U32>::MergeSort(data,sizeof(data)/sizeof(AM_U32));

    for(idx=0; idx<sizeof(data)/sizeof(AM_U32); idx++)
    {
        printf("%d ",data[idx]);
    }

    printf("\n");

}


