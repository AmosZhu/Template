#ifndef _MY_FILESYSTEM_HPP_
#define _MY_FILESYSTEM_HPP_

#include "AmosType.hpp"
#include "CTree.hpp"


#define NAMESIZE 64


/*****************************************************
*                   Struct
*****************************************************/
typedef struct MY_FILENODE_TYPE
{
    char name[NAMESIZE];
    AM_U32 type;
} file_t;


Err_t CreateFileSystem(char* filePath);
void PrintOut(TreeNode_t<file_t> node);

#endif
