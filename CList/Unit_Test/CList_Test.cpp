#include "CList.hpp"
#include "CList_Test.hpp"
#include "AmosType.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223};
AM_U32 data_s[] = {56,123,555,55,4,90};//Use for search
AM_U32 data_del[] = {13,98,4,999,223};

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


void ReverseTest(void)
{
    CList<AM_U32> list1,list2;
    list1.SetCopyFunc(copyFunc);
    list1.SetCompareFunc(cmpFunc);
    list1.SetPrintFunc(printNode);
    CList<AM_U32> list3(list1);

    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(list1.Insert(&data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }
    list3=list2=list1;
    std::cout<<"list3: "<<list3;
    std::cout<<"list2: "<<list2;
    for(i=0; i<list1.CountNo(); i++)
    {
        list1.Reverse();
        std::cout<<list1;
    }
}

void SearchTest(void)
{
    CList<AM_U32> list1;
    list1.SetCopyFunc(copyFunc);
    list1.SetCompareFunc(cmpFunc);
    list1.SetPrintFunc(printNode);
    int i=0;
    AM_S32 pos;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(list1.Insert(&data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }
    list1.Reverse();
    printf("There are %d elements in the list!\n",list1.CountNo());

    for(i=0; i<sizeof(data_s)/sizeof(AM_U32); i++)
    {
        if((pos=list1.Search(&data_s[i]))!=-1)
        {
            printf("%u find at %d\n",data_s[i],pos);
        }
        else
        {
            printf("%u cannot find!\n",data_s[i]);
        }

    }
}

void DeleteTest(void)
{
    CList<AM_U32> list1,list2,list3;
    list1.SetCopyFunc(copyFunc);
    list1.SetCompareFunc(cmpFunc);
    list1.SetPrintFunc(printNode);
    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(list1.Insert(&data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }
    list3=list2=list1;
    std::cout<<"list3: "<<list3;
    std::cout<<"list2: "<<list2;

    for(i=0; i<sizeof(data_del)/sizeof(AM_U32); i++)
    {
        if(list1.Delete(&data_del[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d delete failed!\n",i,data_del[i]);
    }
    std::cout<<"list1: "<<list1;
    list1.Reverse();
    list1.PrintOut();
    printf("There %d elements left in list1\n",list1.CountNo());
}
