#include "AmosType.hpp"
#include "CDisjointSet.hpp"
#include "CDisjointSet_Test.hpp"
#include <string.h>
#include <string>
#include <iostream>

AM_U32 data[] = {1,2,3,4,5,6};
AM_U32 pairData[5][2]= {{1,2},{1,5},{3,4},{5,2},{1,3}};
AM_U32 relaData[3][2]= {{1,4},{2,3},{5,6}};

std::string strData[]= {"Microsoft","Google","Baidu","Tecent","AmosEve","Intel"};
std::string strPari[4][2]= {{"Microsoft","Baidu"},{"Google","AmosEve"},{"Intel","Google"},{"Tecent","Baidu"}};
std::string strRelaData[3][2]= {{"AmosEve","Intel"},{"Microsoft","Google"},{"Tecent","Microsoft"}};


void CDisjointSet_BasicUT(void)
{
    AM_U32 idx,j;

    CDisjointSet<AM_U32> set1(data,sizeof(data)/sizeof(AM_U32));
    printf("sizeof(pairData)=%d\n",sizeof(pairData)/sizeof(AM_U32));
    for(idx=0; idx<sizeof(pairData)/(sizeof(AM_U32)*2); idx++)
    {
        printf("----------------------%d---------------------\n",idx);
        printf("%d,%d\n",pairData[idx][0],pairData[idx][1]);
        set1.Union(pairData[idx][0],pairData[idx][1]);
        for(j=0; j<sizeof(data)/sizeof(AM_U32); j++)
        {
            printf("%d's root is %d\n",data[j],set1.Find(data[j]));
        }

    }

    for(idx=0; idx<sizeof(relaData)/(sizeof(AM_U32)*2); idx++)
    {
        if(set1.Find(relaData[idx][0])==set1.Find(relaData[idx][1]))
        {
            printf("True\n");
        }
        else
        {
            printf("False\n");
        }
    }

    std::cout<<"The size of set: "<<set1.Size()<<std::endl;
    std::cout<<"The number of different sets: "<<set1.Count()<<std::endl;
}

void CDisjointSet_StringUT(void)
{
    AM_U32 idx,j;

    CDisjointSet<std::string> set1(strData,sizeof(strData)/sizeof(std::string));
    printf("sizeof(pairData)=%d\n",sizeof(strPari)/sizeof(std::string));
    for(idx=0; idx<sizeof(strPari)/(sizeof(std::string)*2); idx++)
    {
        printf("----------------------%d---------------------\n",idx);
        printf("%s,%s\n",strPari[idx][0].c_str(),strPari[idx][1].c_str());
        set1.Union(strPari[idx][0],strPari[idx][1]);
        for(j=0; j<sizeof(strData)/sizeof(std::string); j++)
        {
            printf("%s's root is %s\n",strData[j].c_str(),set1.Find(strData[j]).c_str());
        }

    }

    for(idx=0; idx<sizeof(strRelaData)/(sizeof(std::string)*2); idx++)
    {
        if(set1.Find(strRelaData[idx][0])==set1.Find(strRelaData[idx][1]))
        {
            printf("True\n");
        }
        else
        {
            printf("False\n");
        }
    }

    std::cout<<"The size of set: "<<set1.Size()<<std::endl;
    std::cout<<"The number of different sets: "<<set1.Count()<<std::endl;

}
