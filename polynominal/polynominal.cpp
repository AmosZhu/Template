/*********************************************************************
*
*   This file use to process polynominal function
*   Implement:@Process polynominal input
*             @Process polynominal add/sub function
*             @Process polynominal multiply function
*
*   @Author: Amos.Zhu
*
*   $$Caution: Support two equation only yet
*
*   %Input: from 1.txt file, equations should have inorder,and in
*           the same order
*
*********************************************************************/

#include "AmosType.hpp"
#include "CList.hpp"
#include "polynominal.hpp"
#include <stdio.h>
#include <string.h>

#define DB_PRINT(fmt,args...) printf("\033[33m[%s]:"fmt"\033[0m\n",__FUNCTION__,##args)


int main(void)
{
    char buf[1024];
    char* equation;
    CList<polynominal_t> list1,list2,list3;
    polynominal_t value;
    FILE* fp;
    int i;
    /*********************
    *   Open file
    *********************/
    fp=fopen("1.txt","rb+");
    fscanf(fp,"%s",buf);
    printf("%s\n",buf);
    /************************
    *   require equation
    ************************/
    equation=strchr(buf,'=');
    equation++;
    polynominalInput(equation,&list1);
    list1.Reverse();
    list1.ResetElemNext();
    printf("no %d\n",list1.CountNo());
    for(i=0; i<list1.CountNo(); i++)
    {
        list1.GetElemNext(&value);
        printf("index %d: coef %d,exponent %d\n",i,value.coeficient,value.exponent);
    }


    fscanf(fp,"%s",buf);
    printf("%s\n",buf);
    /************************
    *   require equation
    ************************/
    equation=strchr(buf,'=');
    equation++;
    polynominalInput(equation,&list2);
    list2.Reverse();
    list2.ResetElemNext();
    printf("no %d\n",list2.CountNo());
    for(i=0; i<list2.CountNo(); i++)
    {
        list2.GetElemNext(&value);
        printf("index %d: coef %d,exponent %d\n",i,value.coeficient,value.exponent);
    }

    /******************************
    *   Polynominal add
    ******************************/
    polynominalAdd(&list1,&list2,&list3);
    printf("no %d\n",list3.CountNo());
    list3.ResetElemNext();
    for(i=0; i<list3.CountNo(); i++)
    {
        list3.GetElemNext(&value);
        printf("index %d: coef %d,exponent %d\n",i,value.coeficient,value.exponent);
    }


    return 1;
}


AM_U32 processDigit(char** fmt)
{
    AM_U32 num=0;
    while(ISDIGIT(**fmt))
    {
        num*=10;
        num+=**fmt-'0';
        (*fmt)++;
    }
    return num;
}

Err_t polynominalInput(char* buf,CList<polynominal_t>* equation)
{
    char* p;
    short flag;
    AM_S32 num;
    polynominal_t value;

    if((buf==NULL)||(equation==NULL))
        return INVALIDE_PARAMET;

    INITVALUE(value);
    CLEARFLAG(flag);
    SETCOE(flag); //Set only once

    p=buf;

    while(*p)
    {
        switch(*p)
        {
            case '+':
            {
                p++;
            }
            continue;
            break;
            case '-':
            {
                SETSIGN(flag);
                p++;
            }
            continue;
            break;
            case 'X':
            case 'x':
            {
                CLEARCOE(flag);
                if(*++p=='^')
                {
                    SETEXP(flag);
                    p++;
                }
                else
                {
                    value.exponent=1;
                    equation->Insert(value);
                    INITVALUE(value);
                    CLEARFLAG(flag);
                }
            }
            continue;
            break;
            default:
                break;
        }

        if(!ISDIGIT(*p))
        {
            DB_PRINT("input error!");
            return OPERATOR_FAILED;
        }

        num=processDigit(&p);
        if(flag&SIGN)
        {
            num=-num;
            CLEARSIGN(flag);
        }


        /****************************************************************
        *
        *   Amos assume that whenever finish processing the exponent,
        *   It should move to next nominal,and clear all flag status!
        *
        ****************************************************************/
        if(flag&EXPONENT)
        {
            value.exponent=num;
            equation->Insert(value);
            INITVALUE(value);
            CLEARFLAG(flag);
        }
        else
        {
            value.coeficient=num;
        }


        /***********************************************
        *
        *   eg: 56+6x^2
        *   56 is a special coeficient withou exponent,
        *   process only once.
        *
        ***********************************************/
        if(flag&COEFONLY)
        {
            if(*p=='X'||*p=='x')
                continue;
            else
            {
                value.exponent=0;
                equation->Insert(value);
                INITVALUE(value);
                CLEARFLAG(flag);
            }
        }

    }
    return RETURN_SUCCESS;
}

Err_t polynominalOutput(CList<polynominal_t>* input,char* output)
{
}

Err_t polynominalAdd(CList<polynominal_t>* input1,CList<polynominal_t>* input2,CList<polynominal_t>* output)
{
    polynominal_t p1,p2,o3;
    if((input1==NULL)||(input2==NULL)||(output==NULL))
        return INVALIDE_PARAMET;

    input1->ResetElemNext();
    input2->ResetElemNext();

    if(input1->GetElemNext(&p1)!=RETURN_SUCCESS)
        goto ProcessList1;

    if(input2->GetElemNext(&p2)!=RETURN_SUCCESS)
        goto ProcessList2;

    for(;;)
    {
        if(p1.exponent<p2.exponent)
        {
            o3.coeficient=p1.coeficient;
            o3.exponent=p1.exponent;
            output->Insert(o3);
            if(input1->GetElemNext(&p1)!=RETURN_SUCCESS)
                goto ProcessList2;

        }
        else if(p1.exponent>p2.exponent)
        {
            o3.coeficient=p2.coeficient;
            o3.exponent=p2.exponent;
            output->Insert(o3);
            if(input2->GetElemNext(&p2)!=RETURN_SUCCESS)
                goto ProcessList1;
        }
        else if(p1.exponent==p2.exponent)
        {
            o3.coeficient=p1.coeficient+p2.coeficient;
            o3.exponent=p1.exponent;
            output->Insert(o3);
            if(input1->GetElemNext(&p1)!=RETURN_SUCCESS)
                goto ProcessList2;

            if(input2->GetElemNext(&p2)!=RETURN_SUCCESS)
                goto ProcessList1;
        }
        else
        {
            DB_PRINT("Error!");
            return OPERATOR_FAILED;
        }
    }

ProcessList1:
    o3.coeficient=p1.coeficient;
    o3.exponent=p1.exponent;
    output->Insert(o3);

    while(input1->GetElemNext(&p1)==RETURN_SUCCESS)
    {
        o3.coeficient=p1.coeficient;
        o3.exponent=p1.exponent;
        output->Insert(o3);
    }
    return RETURN_SUCCESS;
ProcessList2:
    o3.coeficient=p2.coeficient;
    o3.exponent=p2.exponent;
    output->Insert(o3);

    while(input2->GetElemNext(&p2)==RETURN_SUCCESS)
    {
        o3.coeficient=p2.coeficient;
        o3.exponent=p2.exponent;
        output->Insert(o3);
    }
    return RETURN_SUCCESS;

}

