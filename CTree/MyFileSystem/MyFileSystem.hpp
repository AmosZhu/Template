#ifndef _MY_FILESYSTEM_HPP_
#define _MY_FILESYSTEM_HPP_

#include "AmosType.hpp"
#include "CTree.hpp"


#define NAMESIZE 256


/*****************************************************
*                   Struct
*****************************************************/
typedef struct MY_FILENODE_TYPE
{
    char name[NAMESIZE];
    AM_U32 type;
} file_t;


class CFileSystem
{
public:
    CFileSystem(void);
    CFileSystem(const char* path);
    CFileSystem(const CFileSystem& object);
    ~CFileSystem(void);

public:
    Err_t Create(const char* path);
	void Destroy(void);
    void PrintOut(void);

public:
    const CFileSystem& operator=(const CFileSystem& object);


private:
    Err_t dopath(TreeNode_t<file_t>* ptr);
    Err_t createFileSystem(const char* path);


private:
    CTree<file_t>* m_sysTree;
    char m_fullPath[MAXLINE];

};


#endif
