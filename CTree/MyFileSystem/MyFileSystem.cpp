/*******************************************************
*
*   This file implement a virtual file system
*
*******************************************************/


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include "MyFileSystem.hpp"

/*****************************************************
*                   Define
*****************************************************/

#define DIRECTORY 1
#define NORMALFILE 2


/*****************************************************
*                   Macro
*****************************************************/
#define CREATE_NEWNODE(node,dirp,statbuf,path) \
    node =new TreeNode_t<file_t>;\
    node->firstChild=NULL;\
    node->nextSibling=NULL;\
    memcpy(node->elem.name,dirp->d_name,NAMESIZE);\
    if(lstat(fullPath,&statbuf)==-1){\
        printf("open root path failed, reason : %s\n",strerror(errno));\
        return OPERATOR_FAILED;\
    }\
    if(S_ISDIR(statbuf.st_mode))\
        node->elem.type=DIRECTORY;\
    else\
        node->elem.type=NORMALFILE;\

/******************************************************
*                   Global variable
******************************************************/
char fullPath[MAXLINE];

/******************************************************
*                 Internal function
******************************************************/
static Err_t dopath(CTree<file_t>* fileSystem,TreeNode_t<file_t>* ptr);


Err_t CreateFileSystem(char* filePath)
{
    CTree<file_t>* fileSystem;
    struct stat statbuf;
    TreeNode_t<file_t>* rootn; /*root node*/

    if(filePath==NULL)
        return INVALIDE_PARAMET;

    /*
    *   Do initialize
    */
    memset(fullPath,0,MAXLINE);
    strcpy(fullPath,filePath);

    fileSystem=new CTree<file_t>(PrintOut);
    /*
    *   Firstly, Add root
    */

    if(lstat(filePath,&statbuf)==-1)
    {
        printf("open root path failed, reason : %s\n",strerror(errno));
        return OPERATOR_FAILED;
    }

    rootn=new TreeNode_t<file_t>;
    rootn->firstChild=NULL;
    rootn->nextSibling=NULL;
    if(S_ISDIR(statbuf.st_mode))
        rootn->elem.type=DIRECTORY;
    else
        rootn->elem.type=NORMALFILE;
    memcpy(rootn->elem.name,filePath,NAMESIZE);

    fileSystem->SetRoot(rootn);


    /*
    *   Secondly,construct a file system
    */

    dopath(fileSystem,rootn);

    return RETURN_SUCCESS;

}

void PrintOut(TreeNode_t<file_t> node)
{
    std::cout<<node.elem.name;
    if(node.elem.type==NORMALFILE)
        std::cout<<"*"<<std::endl;
    else if(node.elem.type==DIRECTORY)
        std::cout<<"/"<<std::endl;
}

Err_t dopath(CTree<file_t>* fileSystem,TreeNode_t<file_t>* ptr)
{
    struct stat statbuf;
    struct dirent* dirp;
    DIR* dp;
    char* fileName;

    TreeNode_t<file_t>* child;
    TreeNode_t<file_t>* sibling;
    TreeNode_t<file_t>* current;

    if((fileSystem==NULL)||(ptr==NULL))
        return INVALIDE_PARAMET;

    if(ptr->elem.type==NORMALFILE)  /*We do not need to process the normal file*/
        return RETURN_SUCCESS;

    current=ptr;

    fileName=fullPath+strlen(fullPath);
    *fileName++='/';
    *fileName=0;

    if((dp=opendir(fullPath))==NULL)
        return OPERATOR_FAILED;

    /*
    *   Add first child;
    */
    if((dirp=readdir(dp))!=NULL)
    {
        strcpy(fileName,dirp->d_name);
        CREATE_NEWNODE(child,dirp,statbuf,fullPath);
        fileName[-1]=0;
        fileSystem->AddFirstChild(current,child);
        current=child; /*Make current point to first child*/
    }

    /*
    *   Do recursive and add sibling;
    */
    while((dirp=readdir(dp))!=NULL)
    {
        strcpy(fileName,dirp->d_name);
        CREATE_NEWNODE(sibling,dirp,statbuf,fullPath);
        fileSystem->AddNextSibling(current,sibling);
        current=sibling; /*Make current point to next sibling*/
        if(S_ISDIR(statbuf.st_mode))
            dopath(fileSystem,current);
        fileName[-1]=0;
    }
}

