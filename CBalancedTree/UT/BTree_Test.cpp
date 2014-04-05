#include "BTree_Test.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AM_U32 data[]= {11,22,54,32,89,57,40,12,101,220,445,99,532,980,1,4,0,290,122,431,76,310,411,1110,2310,5432,1540};
AM_U32 deleteData[]= {99,101,532,980,220,445};

static void printNode(AM_U32* node)
{
    printf("[%d]",*node);
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

static void clearFunc(AM_U32* src)
{
    memset(src,0,sizeof(AM_U32));
}

void BTree_UT()
{
    CBalancedTree<AM_U32> btree;
    AM_U32 i;

    btree.SetCopyFunc(copyFunc);
    btree.SetCompareFunc(cmpFunc);
    btree.SetClearFunc(clearFunc);
    btree.SetPrintFunc(printNode);
    for(i=0; i<(sizeof(data)/sizeof(AM_U32)); i++)
    {
        btree.Insert(&data[i]);
    }

    btree.PrintTree();

    for(i=0; i<(sizeof(deleteData)/sizeof(AM_U32)); i++)
    {
        btree.Delete(&deleteData[i]);
        btree.PrintTree();
    }

    printf("============================================\n");
    printf("BTree depth is %d\n",btree.TreeDepth());
}

