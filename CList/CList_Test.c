#include "CList.hpp"

int data[]= {13,55,43,4,123,98,67,444,777,223};

void ReverseTest(void)
{
    CList<int> list1;
    int i=0;
    for(i=0; i<sizeof(data)/sizeof(int); i++)
        list1.insert(data[i]);

    for(i=0; i<10; i++)
    {
        list1.Reverse();
        std::cout<<list1;
    }
}
