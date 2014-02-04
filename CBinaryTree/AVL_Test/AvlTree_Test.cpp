#include "AmosType.hpp"
#include "CBSAvlTree.hpp"
#include "AvlTree_Test.hpp"

//AM_U32 data[]= {12,11,10,9,8,7,6,5,4,3,2,1}; //left single rotation only
//AM_U32 data[]={1,2,3,4,5,6,7,8,9,10,11,12}; //right single rotation only
//AM_U32 data[]={11,22,54,32,89,57,40,12,101,220,445,532,980,1,4,0}; //left & right single rotation
//AM_U32 data[]={100,40,80}; // cause right double rotation
//AM_U32 data[]={100,120,110}; // casue left double rotation
AM_U32 data[]={8,4,20,16,25};
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


void AVLTree_UT(void)
{
    CBSAvlTree<AM_U32> tree1;
    AM_U32 num=55;
    nodeType<AM_U32>* pos=NULL;
    tree1.SetPrintFunc(printNode);
    tree1.SetCmpFunc(cmpFunc);
    AM_U32 idx;
    AM_U32 length=sizeof(data)/sizeof(AM_U32);
    for(idx=0; idx<length; idx++)
    {
        tree1.Insert(&data[idx]);
    }

    printf("------------> CBSAvlTree Info <---------------\n");
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree nodes is %d\n",tree1.TreeNodeCount());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
    pos=tree1.FindMax();
    printf("Max number in the tree is %d\n",pos->elem);
    pos=tree1.FindMin();
    printf("Min number in the tree is %d\n",pos->elem);
    pos=tree1.Find(&num);
    if(pos==NULL)
        printf("%d is not in the tree\n",num);
    else
        printf("%d is in the tree, pos->elem=%d\n",pos->elem);
    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();

    num=4;
    if(tree1.Delete(&num)!=RETURN_SUCCESS)
    {
        printf("Delete failed!\n");
    }
    else
    {
        printf("===============After delete====================\n");
        printf("------------> CBSAvlTree Info <---------------\n");
        printf("Tree depth is %d\n",tree1.TreeDepth());
        printf("Tree nodes is %d\n",tree1.TreeNodeCount());
        printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
        pos=tree1.FindMax();
        printf("Max number in the tree is %d\n",pos->elem);
        pos=tree1.FindMin();
        printf("Min number in the tree is %d\n",pos->elem);
        pos=tree1.Find(&num);
        if(pos==NULL)
            printf("%d is not in the tree\n",num);
        else
            printf("%d is in the tree, pos->elem=%d\n",pos->elem);
        tree1.InorderTraversal();
        tree1.PreorderTraversal();
        tree1.PostorderTraversal();
    }
}

void CBSearchTree_Compare(void)
{
    CBSearchTree<AM_U32> tree1;
    AM_U32 num=55;
    nodeType<AM_U32>* pos=NULL;
    tree1.SetPrintFunc(printNode);
    tree1.SetCmpFunc(cmpFunc);
    AM_U32 idx;
    AM_U32 length=sizeof(data)/sizeof(AM_U32);
    for(idx=0; idx<length; idx++)
        tree1.Insert(&data[idx]);

    printf("------------> CBSearchTree Info <---------------\n");
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree nodes is %d\n",tree1.TreeNodeCount());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
    pos=tree1.FindMax();
    printf("Max number in the tree is %d\n",pos->elem);
    pos=tree1.FindMin();
    printf("Min number in the tree is %d\n",pos->elem);
    pos=tree1.Find(&num);
    if(pos==NULL)
        printf("%d is not in the tree\n",num);
    else
        printf("%d is in the tree, pos->elem=%d\n",pos->elem);
    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();

    num=7;
    tree1.Delete(&num);
    tree1.InorderTraversal();

}

