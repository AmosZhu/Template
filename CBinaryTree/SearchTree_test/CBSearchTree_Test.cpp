#include "AmosType.hpp"
#include "CBSearchTree.hpp"
#include "CBSearchTree_Test.hpp"

AM_U32 data[]={11,22,54,32,89,57,40,12,101,220,445,532,980,1,4,0};
//AM_U32 data[]={12,11,10,9,8,7,6,5,4,3,2,1};

static void printNode(AM_U32 node)
{
    printf("[%d]",node);
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

void CBS_Test(void)
{
    CBSearchTree<AM_U32> tree1;
    AM_U32 idx,length;
    nodeType<AM_U32>* p;
	AM_U32 aaa=9999;


    tree1.SetPrintFunc(printNode);
    tree1.SetCmpFunc(cmpFunc);
    length=sizeof(data)/sizeof(AM_U32);
    for(idx=0;idx<length;idx++)
        tree1.Insert(&data[idx]);

    printf("-------->Tree depth is %d <-----------\n",tree1.TreeDepth());
    printf("-------->Tree nodes is %d <-----------\n",tree1.TreeNodeCount());
    printf("-------->Tree leaves is %d <-----------\n",tree1.TreeLeavesCount());
    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();
    tree1.LevelorderTraversal();

    p=tree1.FindMax();
    printf("max=%d\n",p->elem);
    p=tree1.FindMin();
    printf("min=%d\n",p->elem);

    printf("After delete %d\n",p->elem);
    tree1.Delete(&p->elem);
    tree1.InorderTraversal();

	p=tree1.Find(&data[10]);
	printf("data[10]=%d,find is %d\n",data[10],p->elem);
	p=tree1.Find(&aaa);

    tree1.DestroyTree();
}

