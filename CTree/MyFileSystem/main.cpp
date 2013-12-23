#include "MyFileSystem.hpp"

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("Usage: <func><path>\n");
        return -1;
    }

    if(CreateFileSystem(argv[1])!=RETURN_SUCCESS)
    {
        printf("Create FileSystem failed!");
        return -1;
    }

    return 0;
}
