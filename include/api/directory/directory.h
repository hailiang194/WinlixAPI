#ifndef _DIRECTORY_DIRECTORY_H_
#define _DIRECTORY_DIRECTORY_H_

#include "api/basetype.h"
#ifdef _WIN32
#else
#include <dirent.h>
#endif

typedef struct
{
    char d_name[API_MAX_PATH + 1];
} API_DIR_CHILD;

typedef struct 
{
    char path[API_MAX_PATH + 1];
#ifdef _WIN32
    WIN32_FIND_DATA* ffd;
    HANDLE hFind;
#else
    DIR* dir;
#endif
} API_DIR;

START_EXTERN

EXTENDAPI API_DIR* openDir(const char* path);
EXTENDAPI BOOL readDir(API_DIR* dir, API_DIR_CHILD* next);
EXTENDAPI API_RESULT closeDir(API_DIR* dir);

END_EXTERN

#endif //!_DIRECTORY_DIRECTORY_H_