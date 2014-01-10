#include "AmosType.hpp"
#include "CBinaryTree.hpp"
#include "RPNTree.hpp"
#include "RPNCalculate.hpp"

#define INPUTPATH "expression.txt"


int main(int argc,char* argv[])
{
    CBinaryTree<rpn_t> RPNTree;

    BuildRPNTree(&RPNTree,INPUTPATH);
    RPNTree.PostorderTraversal();
}

