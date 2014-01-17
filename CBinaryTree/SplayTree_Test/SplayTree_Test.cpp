#include "AmosType.hpp"
#include "SplayTree_Test.hpp"
#include "CBSSplayTree.hpp"


#define DATALEN 32
#define FOUNDTIME 5

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

static void OrderedDataProduce(AM_U32* data,AM_U32 len)
{
    int i;
    if((data==NULL)||(len==0))
        return;

    for(i=0; i<len; i++)
        data[i]=i+1;
    return;
}

static void ReverseOrderDataProduce(AM_U32* data,AM_U32 len)
{
    int i;
    if((data==NULL)||(len==0))
        return;

    for(i=0; i<len; i++)
        data[i]=len-i;
    return;
}

void SPLAYTREE_UT(void)
{
    AM_U32 data[DATALEN];
    CBSSplayTree<AM_U32> tree1;
    AM_U32 num=55;
    nodeType<AM_U32>* pos=NULL;
    AM_U32 idx;
    srandom(time(NULL));

    //OrderedDataProduce(data,DATALEN);
    ReverseOrderDataProduce(data,DATALEN);
    tree1.SetPrintFunc(printNode);
    tree1.SetCmpFunc(cmpFunc);
    for(idx=0; idx<DATALEN; idx++)
        tree1.Insert(&data[idx]);

    printf("------------> CBSAvlTree Info <---------------\n");
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree nodes is %d\n",tree1.TreeNodeCount());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
    pos=tree1.FindMax();
    printf("Max number in the tree is %d\n",pos->elem);
    pos=tree1.FindMin();
    printf("Min number in the tree is %d\n",pos->elem);

    printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("+               Find %d times                  +\n",FOUNDTIME);
    printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");

    for(idx=1; idx<=FOUNDTIME; idx++)
    {
        num=random()%32;
		//num=idx;
        printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("+           The %d time find                   +\n",idx);
        printf("++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");
        printf("------------> Before find <---------------\n");
        printf("Tree depth is %d\n",tree1.TreeDepth());
        printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
        tree1.InorderTraversal();
        tree1.PreorderTraversal();
        tree1.PostorderTraversal();

        pos=tree1.Find(&num);
        if(pos==NULL)
            printf("%d is not in the tree\n",num);
        else
            printf("%d is in the tree, pos->elem=%d\n",num,pos->elem);

        printf("\n\n");

        printf("------------> After find <---------------\n");
        printf("Tree depth is %d\n",tree1.TreeDepth());
        printf("Tree leaves is %d\n",tree1.TreeLeavesCount());

        tree1.InorderTraversal();
        tree1.PreorderTraversal();
        tree1.PostorderTraversal();
        printf("\n\n");
    }

    num=-1;
    printf("++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("+           The %d time find                   +\n",idx);
    printf("++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");
    printf("------------> Before find <---------------\n");
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());
    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();

    pos=tree1.Find(&num);
    if(pos==NULL)
        printf("%d is not in the tree\n",num);
    else
        printf("%d is in the tree, pos->elem=%d\n",num,pos->elem);

    printf("\n\n");

    printf("------------> After find <---------------\n");
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());

    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();
    printf("\n\n");

    num=6;
    tree1.Delete(&num);
    printf("Tree depth is %d\n",tree1.TreeDepth());
    printf("Tree leaves is %d\n",tree1.TreeLeavesCount());

    tree1.InorderTraversal();
    tree1.PreorderTraversal();
    tree1.PostorderTraversal();

}
