#include "CList.hpp"
#include "CList_Test.hpp"
#include "AmosType.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223};
AM_U32 data_s[] = {56,123,555,55,4,90};//Use for search

void ReverseTest(void)
{
    CList<AM_U32> list1;
    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(list1.Insert(data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }

    for(i=0; i<10; i++)
    {
        list1.Reverse();
        std::cout<<list1;
    }
}

void SearchTest(void)
{
    CList<AM_U32> list1;
    int i=0;
    AM_S32 pos;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(list1.Insert(data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }
    list1.Reverse();
    printf("There are %d elements in the list!\n",list1.CountNo());

    for(i=0; i<sizeof(data_s)/sizeof(AM_U32); i++)
    {
        if((pos=list1.Search(data_s[i]))!=-1)
        {
            printf("%u find at %d\n",data_s[i],pos);
        }
        else
        {
            printf("%u cannot find!\n",data_s[i]);
        }

    }
}
