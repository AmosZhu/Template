/*******************************************************
*
*   This file simulate a file system
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
        printf("open  fullpath failed, reason : %s\n",strerror(errno));\
        return OPERATOR_FAILED;\
    }\
    if(S_ISDIR(statbuf.st_mode))\
        node->elem.type=DIRECTORY;\
    else\
        node->elem.type=NORMALFILE;\


#define COPY_NAME(src,dst) \
    AM_U32 dstNameLen=strlen(dst);\
    if(dstNameLen>NAMESIZE) {\
        printf("[%d]:Copy name too big\n",__LINE__);\
        return OPERATOR_FAILED;}\
    strcpy(src,dst);\
    src[dstNameLen]=0;

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
    char* abPath; /*Absolute Path*/

    if(filePath==NULL)
        return INVALIDE_PARAMET;

    /*
    *   Do initialize
    */
    memset(fullPath,0,MAXLINE);
    strcpy(fullPath,filePath);

    abPath=getPath(filePath);

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
    COPY_NAME(rootn->elem.name,abPath);

    fileSystem->SetRoot(rootn);


    /*
    *   Secondly,construct a file system
    */

    dopath(fileSystem,rootn);

    /*
     *  Lastly,print out;
     */

    fileSystem->PreOrderTraversal();
    return RETURN_SUCCESS;

}

void PrintOut(file_t node)
{
    std::cout<<node.name;
    if(node.type==NORMALFILE)
        std::cout<<"*"<<std::endl;
    else if(node.type==DIRECTORY)
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

    /*
    *   Only directory can run here,check again.
    */
    if((dp=opendir(fullPath))==NULL)
        return OPERATOR_FAILED;

    /*
    *   Add first child,and ignore the . and ..
    */

    while((dirp=readdir(dp))!=NULL)
    {
        if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
            continue;

        COPY_NAME(fileName,dirp->d_name);
        CREATE_NEWNODE(child,dirp,statbuf,fullPath);
        fileSystem->AddFirstChild(current,child);
        current=child; /*Make current point to first child*/
        if(S_ISDIR(statbuf.st_mode))
            dopath(fileSystem,current);
        break;
    }

    /*
    *   Do recursive and add sibling;
    */

    while((dirp=readdir(dp))!=NULL)
    {
        if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
            continue;
        COPY_NAME(fileName,dirp->d_name);
        CREATE_NEWNODE(sibling,dirp,statbuf,fullPath);
        fileSystem->AddNextSibling(current,sibling);
        current=sibling; /*Make current point to next sibling*/
        if(S_ISDIR(statbuf.st_mode))
            dopath(fileSystem,current);
    }
    fileName[-1]=0;

    return RETURN_SUCCESS;
}


/********************************************************************
*
*   Get the path's obslute path
*
*********************************************************************/
char* getPath(const char* path)
{
    char* currentPath;
    char* buffer=(char*)malloc(sizeof(char)*BUFSIZE);
    if(path==NULL)
        return NULL;

    currentPath=getcwd(NULL,0);
    chdir(path);
    buffer=getcwd(NULL,0);
    chdir(currentPath);
    return buffer;
}
