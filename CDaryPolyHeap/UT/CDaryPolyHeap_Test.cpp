#include "CDaryPolyHeap_Test.hpp"


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
    AM_U32 val=37;
    TreeNode_t<AM_U32> node;
    CDaryPolyHeap<AM_U32> heap1,heap3,heap4;
    CDaryPolyHeap<AM_U32>* heap5,*heap6,*heap7;
    heap1.SetPrintFunc(printNode);
    heap1.SetCopyFunc(copyFunc);
    heap1.SetCompareFunc(cmpFunc);

    heap1.SetRoot(&val);
    //heap1.PreOrderTraversal();

    CDaryPolyHeap<AM_U32> heap2(heap1);
    val=58;
    heap2.SetRoot(&val);
    //heap2.PreOrderTraversal();

    heap5=heap1.Merge(&heap2);
    //heap1.PreOrderTraversal();
    heap4=heap3=heap2;
    //heap4.PreOrderTraversal();
    //heap3.PreOrderTraversal();

    val=12;
    heap3.SetRoot(&val);
    val=100;
    heap4.SetRoot(&val);

    heap6=heap3.Merge(&heap4);

    heap6->PreOrderTraversal();

    if(heap5!=nullptr)
    {
        heap5->PreOrderTraversal();
    }

    heap7=heap5->Merge(heap6);
    heap7->PreOrderTraversal();

    printf("\n");
}
