/***********************************
*
*   Consruct a RPNTree
*   Input from RPN expression
*
***********************************/

#include <string.h>
#include <errno.h>
#include "CBinaryTree.hpp"
#include "CStack.hpp"
#include "RPNTree.hpp"

static Err_t PRNTreeProcess(CBinaryTree<rpn_t>** RPNTree,rpn_t* expression);
static void printNode(rpn_t node);

Err_t BuildRPNTree(CBinaryTree<rpn_t>* RPNTree,const char* path)
{
    FILE* fp;
    char buf[MAXLINE];
    rpn_t RPNExpression[MAXLINE];

    if((RPNTree==NULL)||(path==NULL))
        return INVALIDE_PARAMET;

    if((fp=fopen(path,"rb+"))==NULL)
    {
        printf("file open failed, reason: %s\n",strerror(errno));
        return INVALIDE_PARAMET;
    }

    fscanf(fp,"%s",buf);

    RPNTree->SetPrintFunc(printNode);

    if(RPNTranslate(buf,RPNExpression)!=RETURN_SUCCESS)
        return OPERATOR_FAILED;

    return PRNTreeProcess(&RPNTree,RPNExpression);
}

Err_t PRNTreeProcess(CBinaryTree<rpn_t>** RPNTree,rpn_t* expression)
{
    CStack<nodeType<rpn_t>* > box; /*Using a stack box to process the expression*/
    nodeType<rpn_t>* newNode;
    nodeType<rpn_t>* left,*right;
    rpn_t* idx=expression;
    if((RPNTree==NULL)||(expression==NULL))
        return INVALIDE_PARAMET;

    while(idx->type!=TYPE_NULL)
    {

        /*Process number*/
        if(idx->type==TYPE_NUMBER)
        {
            newNode=new nodeType<rpn_t>;
            newNode->elem=*idx;
            newNode->lLink=NULL;
            newNode->rLink=NULL;
            box.PushNoCopy(&newNode);
            idx++;
            continue;
        }

        /*
        *   If it is a operators then pop out 2 element from the box,build a tree;
        */

        if(idx->type==TYPE_OP)
        {
            newNode=new nodeType<rpn_t>;
            newNode->elem=*idx;
            newNode->lLink=NULL;
            newNode->rLink=NULL;
            box.PopNoCopy(&right);
            box.PopNoCopy(&left);

            (*RPNTree)->SetRoot(newNode);
            (*RPNTree)->SetLeftChild(left);
            (*RPNTree)->SetRightChild(right);

            box.PushNoCopy(&newNode);
            idx++;
        }
    }

    return RETURN_SUCCESS;

}

static void printNode(rpn_t node)
{
    if(node.type==TYPE_NUMBER)
        printf("[%d]",node.value);
    else if(node.type==TYPE_OP)
        printf("[%c]",node.value&0xff);
}


