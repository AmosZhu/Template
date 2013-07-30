#include "AmosType.hpp"
#include "CQueue.hpp"

AM_U32 data[] = {13,55,43,4,123,98,67,444,777,223};

void CQueueInOutTest(void)
{
    CQueue<AM_U32> q1;
    int i=0;
    for(i=0; i<sizeof(data)/sizeof(AM_U32); i++)
    {
        if(q1.Enqueue(data[i])!=RETURN_SUCCESS)
            printf("data[%d]=%d insert failed!\n",i,data[i]);
    }

    q1.PrintOut();
}
