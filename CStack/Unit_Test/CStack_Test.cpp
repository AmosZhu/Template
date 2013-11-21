#include "CStack_Test.hpp"
#include "CStack.hpp"
#include <iostream>

static AM_U32 data[]= {4,5,9,1,10,33,67,88};


void CStackBasicTest(void)
{
    CStack<AM_U32> stack1;
    int i;
    int number;
    AM_U32 temp;


    for(i=0; i<sizeof(data)/sizeof(AM_U32);i++)
    {
        stack1.Push(&data[i]);
    }
    CStack<AM_U32>stack2=stack1;
    CStack<AM_U32>stack3(stack1);
    std::cout<<"stack1 size: "<<stack1.Size()<<std::endl;
    std::cout<<"stack2 size: "<<stack2.Size()<<std::endl;
    std::cout<<"stack3 size: "<<stack3.Size()<<std::endl;

    stack1.Top(&temp);
    std::cout<<"stack1 top: "<<temp<<std::endl;

    std::cout<<"--------STACK 1----------------"<<std::endl;
    number=stack1.CountNo();
    std::cout<<"stack1 count: "<<number<<std::endl;
    for(i=0; i<number;i++)
    {
        stack1.Pop(&temp);
        std::cout<<" "<<temp;
    }
    std::cout<<std::endl;


    std::cout<<"--------STACK 2----------------"<<std::endl;
    number=stack2.CountNo();
    std::cout<<"stack2 count: "<<number<<std::endl;
    for(i=0; i<number;i++)
    {
        stack2.Pop(&temp);
        std::cout<<" "<<temp;
    }
    std::cout<<std::endl;


    std::cout<<"--------STACK 3----------------"<<std::endl;
    number=stack3.CountNo();
    std::cout<<"stack3 count: "<<number<<std::endl;
    for(i=0; i<number;i++)
    {
        stack3.Pop(&temp);
        std::cout<<" "<<temp;
    }
    std::cout<<std::endl;



}
