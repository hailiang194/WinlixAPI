#include "api/directory/directory.h"
#include "api/string/safe.h"

#ifdef _WIN32
#include "api/directory/path.h"
#define STAT_FUNC _stat
#else
#include <stdlib.h>
#include <string.h>
#define STAT_FUNC stat
#endif
#ifdef _WIN32
BOOL parseChildForWin32(API_DIR_CHILD* child, const WIN32_FIND_DATA* ffd)
{
    if(child == NULL || ffd == NULL)
        return FALSE;

    stringCopy(child->d_name, API_MAX_PATH, ffd->cFileName);

    return TRUE;
}
#else
BOOL parseChildForLinux(API_DIR_CHILD* child, const struct dirent* dir)
{
    if(child == NULL || dir == NULL)
        return FALSE;
    stringCopy(child->d_name, API_MAX_PATH, dir->d_name);
}
#endif

API_DIR* openDir(const char* path)
{
    API_DIR* dir = (API_DIR*)malloc(sizeof(API_DIR));
    memset(dir, 0x0, sizeof(API_DIR));
#ifdef _WIN32
    dir->ffd = NULL;
#else
#endif
    if(dir == NULL)
        return NULL;
#ifdef _WIN32
    stringCopy(dir->path, API_MAX_PATH, path);
#else
    stringCopy(dir->path, API_MAX_PATH, path);
    dir->dir = opendir(dir->path);
#endif

    return dir;
}

BOOL readDir(API_DIR* dir, API_DIR_CHILD* next)
{
    if(dir == NULL || next == NULL)
        return FALSE;

#ifdef _WIN32
    if(dir->ffd == NULL)
    {
        API_CHAR path[API_MAX_PATH];
        memset(path, API_MAX_PATH, sizeof(API_CHAR));
        joinPath(path, API_MAX_PATH, dir->path, "\\*");

        dir->ffd = (WIN32_FIND_DATA*)malloc(sizeof(WIN32_FIND_DATA));
        if(dir->ffd == NULL)
            return FALSE;
        
        memset(dir->ffd, 0x0, sizeof(WIN32_FIND_DATA));
        dir->hFind = FindFirstFile(path, dir->ffd);
        if (INVALID_HANDLE_VALUE == dir->hFind)
            return FALSE; 
    }
    else if (!FindNextFile(dir->hFind, dir->ffd))
    {
        return FALSE;
    }

    parseChildForWin32(next, dir->ffd);
#else
    if(dir->dir == NULL)
        return FALSE;

    struct dirent* child;
    if((child = readdir(dir->dir)) == NULL)
        return FALSE;
    parseChildForLinux(next, child);
#endif
    API_CHAR fullPath[API_MAX_PATH];
    joinPath(fullPath, API_MAX_PATH, dir->path, next->d_name);
    STAT_FUNC(fullPath, &(next->stat));
    return TRUE;
}

API_RESULT closeDir(API_DIR* dir)
{
    if(dir == NULL)
        return (API_RESULT)-1;
#ifdef _WIN32
    FindClose(dir->hFind);
    if(dir->ffd != NULL)
        free(dir->ffd);
#else
    if(dir->dir != NULL)
        closedir(dir->dir);
#endif
    free(dir);
    return S_OK;
}