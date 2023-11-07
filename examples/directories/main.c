#include <stdio.h>
#include <stdlib.h>
#include "api/directory/directory.h"
#include "api/directory/path.h"

int main(int argc, API_CHAR** argv)
{
    if(argc != 2)
    {
        API_PRINTF(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
        return (-1);
    }
    size_t length = 0;
    stringCheckLength(argv[1], API_MAX_PATH, &length);
    if(length > (API_MAX_PATH - 3))
    {
        API_PRINTF(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }
    API_PRINTF(TEXT("\nTarget directory is %s\n\n"), argv[1]);
    API_DIR* dir = openDir(argv[1]);
    API_DIR_CHILD child;
    while(readDir(dir, &child))
    {
        API_CHAR fullPath[API_MAX_PATH];
        joinPath(fullPath, API_MAX_PATH, dir->path, child.d_name);
        API_PRINTF(TEXT("\n\t%s"), fullPath);
    }
    API_PRINTF(TEXT("\n"));
    closeDir(dir);
    return 0;
}