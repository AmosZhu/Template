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
    CList<polynominal_t> list1;
    polynominal_t value;
    FILE* fp;
    int i;
    /*********************
    *   Open file
    *********************/
    fp=fopen("1.txt","rb+");
    fscanf(fp,"%s",buf);
    printf("buf=%s\n",buf);
    /************************
    *   require equation
    ************************/
    equation=strchr(buf,'=');
    equation++;
    polynominalProcess(equation,&list1);
    list1.ResetElemNext();
    printf("no %d\n",list1.CountNo());
    for(i=0; i<list1.CountNo(); i++)
    {
        list1.GetElemNext(&value);
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

void polynominalProcess(char* buf,CList<polynominal_t>* equation)
{
    char* p;
    short flag;
    AM_S32 num;
    polynominal_t value;
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
            return ;
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
}

