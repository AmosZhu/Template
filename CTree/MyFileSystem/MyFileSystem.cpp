/*******************************************************
*
*   This file simulate a file system
*   @Author:Amos.zhu
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
    if(lstat(m_fullPath,&statbuf)==-1){\
        printf("open  fullpath failed, reason : %s\n",strerror(errno));\
        return OPERATOR_FAILED;\
    }\
    if(S_ISDIR(statbuf.st_mode))\
        node->elem.type=DIRECTORY;\
    else\
        node->elem.type=NORMALFILE;\
 

#define COPY_NAME(dst,src) \
    AM_U32 dstNameLen=strlen(src);\
    if(dstNameLen>NAMESIZE) {\
        printf("[%d]:Copy name too big\n",__LINE__);\
        return OPERATOR_FAILED;}\
    strcpy(dst,src);\
    src[dstNameLen]=0;

/******************************************************
*                   Global variable
******************************************************/

/******************************************************
*                 Internal function
******************************************************/
static void printNode(file_t node)
{
    std::cout<<node.name;
    if(node.type==NORMALFILE)
        std::cout<<"*"<<std::endl;
    else if(node.type==DIRECTORY)
        std::cout<<"/"<<std::endl;
}

/********************************************************************
*
*   Get the path's obslute path
*
*********************************************************************/
static char* getPath(const char* path)
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

/********************************************************************
*                           Implemention
********************************************************************/

CFileSystem::CFileSystem(void)
{
    m_sysTree=NULL;
    memset(m_fullPath,0,MAXLINE);
}

CFileSystem::CFileSystem(const char * path)
{
    createFileSystem(path);
}

CFileSystem::CFileSystem(const CFileSystem& object)
{
#if 0
    if(m_sysTree!=NULL)
    {
        delete m_sysTree;
        m_sysTree=NULL;
    }
#endif
    m_sysTree=new CTree<file_t>;
    *m_sysTree=*object.m_sysTree;
}

CFileSystem::~CFileSystem(void)
{
    if((m_sysTree!=NULL)&&(!m_sysTree->IsEmpty()))
    {
        delete m_sysTree;
        m_sysTree=NULL;
    }

    return;
}

const CFileSystem& CFileSystem::operator=(const CFileSystem& object)
{
    if(this==&object)
        return *this;
    if(m_sysTree!=NULL)
    {
        delete m_sysTree;
        m_sysTree=NULL;
    }
    m_sysTree=new CTree<file_t>;
    *m_sysTree=*object.m_sysTree;
    return *this;
}


Err_t CFileSystem::Create(const char * path)
{
    createFileSystem(path);
}

void CFileSystem::Destroy(void)
{
    if((m_sysTree!=NULL)&&(!m_sysTree->IsEmpty()))
    {
        delete m_sysTree;
        m_sysTree=NULL;
    }

    return;
}

void CFileSystem::PrintOut(void)
{
    if(m_sysTree==NULL)
    {
        std::cout<<"Empty file system!"<<std::endl;
        return;
    }
    m_sysTree->PreOrderTraversal();
}

Err_t CFileSystem::createFileSystem(const char* path)
{
    struct stat statbuf;
    TreeNode_t<file_t>* rootn; /*root node*/
    char* abPath; /*Absolute Path*/

    if(path==NULL)
        return INVALIDE_PARAMET;

    /*
    *   Do initialize
    */
    memset(m_fullPath,0,MAXLINE);
    strcpy(m_fullPath,path);

    abPath=getPath(path);

    m_sysTree=new CTree<file_t>(printNode);
    /*
    *   Firstly, Add root
    */

    if(lstat(path,&statbuf)==-1)
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

    if(m_sysTree->SetRoot(rootn)!=RETURN_SUCCESS)
        return OPERATOR_FAILED;


    /*
    *   Secondly,construct a file system
    */

    return dopath(rootn);
}


Err_t CFileSystem::dopath(TreeNode_t<file_t>* ptr)
{
    struct stat statbuf;
    struct dirent* dirp;
    DIR* dp;
    char* fileName;

    TreeNode_t<file_t>* child;
    TreeNode_t<file_t>* sibling;
    TreeNode_t<file_t>* current;

    if((m_sysTree==NULL)||(ptr==NULL))
        return INVALIDE_PARAMET;

    if(ptr->elem.type==NORMALFILE)  /*We do not need to process the normal file*/
        return RETURN_SUCCESS;


    current=ptr;

    fileName=m_fullPath+strlen(m_fullPath);
    *fileName++='/';
    *fileName=0;

    /*
    *   Only directory can run here,check again.
    */
    if((dp=opendir(m_fullPath))==NULL)
        return OPERATOR_FAILED;

    /*
    *   Add first child,and ignore the . and ..
    */

    while((dirp=readdir(dp))!=NULL)
    {
        if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
            continue;

        COPY_NAME(fileName,dirp->d_name);
        CREATE_NEWNODE(child,dirp,statbuf,m_fullPath);
        m_sysTree->AddFirstChild(current,child);
        current=child; /*Make current point to first child*/
        if(S_ISDIR(statbuf.st_mode))
            dopath(current);
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
        CREATE_NEWNODE(sibling,dirp,statbuf,m_fullPath);
        m_sysTree->AddNextSibling(current,sibling);
        current=sibling; /*Make current point to next sibling*/
        if(S_ISDIR(statbuf.st_mode))
            dopath(current);
    }
    fileName[-1]=0;

    return RETURN_SUCCESS;

}




