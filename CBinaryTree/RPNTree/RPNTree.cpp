/***********************************
*
*   Consruct a RPNTree
*   Input from RPN expression
*
***********************************/

#include <string.h>
#include <errno.h>
#include "CBinaryTree.hpp"
#include "RPNCalculate.hpp"

Err_t BuildRPNTree(const char* path)
{
    FILE* fp;
    char buf[MAXLINE];
    rpn_t RPNExpression[MAXLINE];
    rpn_t* idx;
    CBinaryTree<rpn_t>* RPNTree;

    if(path==NULL)
        return INVALIDE_PARAMET;

    if((fp=fopen(path,"rb+"))==NULL)
    {
        printf("file open failed, reason: %s\n",strerror(errno));
        return INVALIDE_PARAMET;
    }

    fscanf(fp,"%s",buf);

    if(RPNTranslate(buf,RPNExpression)!=RETURN_SUCCESS)
        return OPERATOR_FAILED;

    for(idx=RPNExpression; idx->type!=TYPE_NULL; idx++)
    {
        if(idx->type==TYPE_NUMBER)
            printf("[%d]",idx->value);
        else if(idx->type==TYPE_OP)
            printf("[%c]",idx->value&0xff);
        else
            break;
    }
    printf("\n");


    RPNTree=new CBinaryTree<rpn_t>;

    return RETURN_SUCCESS;
}
