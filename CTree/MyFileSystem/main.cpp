#include "MyFileSystem.hpp"

int main(int argc,char* argv[])
{
    CFileSystem fileSystem;
    if(argc!=2)
    {
        printf("Usage: <func><path>\n");
        return -1;
    }

#if 0
    if(fileSystem.Create(argv[1])!=RETURN_SUCCESS)
    {
        printf("Create FileSystem failed!");
        return -1;
    }
#endif /* Modify by Amos.zhu */
    CFileSystem fileSystem1(argv[1]);
	fileSystem1.Destroy();
	fileSystem1.PrintOut();
#if 0
    CFileSystem fileSystem2(fileSystem1);
    fileSystem2.PrintOut();
    fileSystem=fileSystem1;
    fileSystem.PrintOut();
    fileSystem1=fileSystem2;
    //fileSystem2.PrintOut();
    fileSystem1.PrintOut();
#endif 
    return 0;
}
