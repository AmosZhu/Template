#include "AmosType.hpp"
#include "CQueue.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223};
AM_U32 data_s[] = {56,123,555,55,4,90};//Use for search


void CQueueInOutTest(void)
{
    CQueue<AM_U32> q1,q2,q3;
    AM_U32 i=0;
    AM_U32 out;
    AM_U32 count;
    AM_U32 pos;
    std::cout<<"------------>Enqueue now<------------------"<<std::endl;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(q1.Enqueue(data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }
    std::cout<<q1;

    std::cout<<"------------>Dequeue now<------------------"<<std::endl;

    q3=q2=q1;
    count=q1.CountNo();
    for(i=0; i<count; i++)
    {
        q1.Dequeue(&out);
        std::cout<<out<<" ";
    }
    std::cout<<std::endl;
    printf("There %d left \n",q1.CountNo());
    std::cout<<"q3: "<<q3;
    std::cout<<"q2: "<<q2;
    std::cout<<"q1: "<<q1;

    for(i=0; i<sizeof(data_s)/sizeof(AM_U32); i++)
    {
        if((pos=q3.Search(data_s[i]))!=-1)
        {
            printf("%u found at %d\n",data_s[i],pos);
        }
        else
        {
            printf("%u Cannot find in the queue!\n");
        }
    }

}
