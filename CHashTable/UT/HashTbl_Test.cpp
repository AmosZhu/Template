#include "CHashTable.hpp"
#include "HashTbl_Test.hpp"

#define HASHTBL_SIZE 13

AM_U32 data[]= {11,22,54,32,89,57,40,12,101,220,445,99,532,980,1,4,0,290,122,431,76,310,411,1110,2310,5432,1540};
AM_U32 data_s[] = {56,123,555,55,4,90,22};//Use for search
AM_U32 data_del[] = {13,98,4,999,223,101,220};


static AM_U32 hashFunc(AM_U32* src)
{
    return *src%HASHTBL_SIZE;
}

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


void HashTbl_UT(void)
{
    AM_U32 idx;
    CHashTable<AM_U32> hashTbl(HASHTBL_SIZE);

    AM_U32* elem;
    hashTbl.SetCompareFunc(cmpFunc);
    hashTbl.SetPrintFunc(printNode);
    hashTbl.SetCopyFunc(copyFunc);
    hashTbl.SetHashFunc(hashFunc);

    for(idx=0; idx<(sizeof(data)/sizeof(AM_U32)); idx++)
    {
        hashTbl.Insert(&data[idx]);
    }

    hashTbl.PrintOut();

    printf("----------------------->Search Test<--------------------------\n");
    for(idx=0; idx<(sizeof(data_s)/sizeof(AM_U32)); idx++)
    {
        if((elem=hashTbl.Search(&data_s[idx]))!=NULL)
        {
            printf("[%d] has been found\n",*elem);
        }
        else
        {
            printf("[%d] not found in the hash table!\n",data_s[idx]);
        }
    }

    printf("----------------------->Delete Test<--------------------------\n");
    for(idx=0; idx<(sizeof(data_del)/sizeof(AM_U32)); idx++)
    {
    	printf("===================Delete [%d]===================\n",data_del[idx]);
        if(hashTbl.Delete(&data_del[idx])!=RETURN_SUCCESS)
        {
            printf("[%d] delete failed\n",data_del[idx]);
        }
        else
        {
            hashTbl.PrintOut();
        }
    }

    printf("----------------------->Hash Table 1<--------------------------\n");
    CHashTable<AM_U32> hashTbl1(hashTbl);
    hashTbl1.PrintOut();

    printf("----------------------->Hash Table 2<--------------------------\n");
    CHashTable<AM_U32> hashTbl2;
    hashTbl2=hashTbl1;
    std::cout<<hashTbl2;
}

