#include "api/directory/path.h"
#include "api/string/safe.h"
#include <sys/stat.h>

API_RESULT joinPath(API_CHAR* out, const size_t MAX, const API_CHAR* head, const API_CHAR* tail)
{   
    API_CHAR* cO = out;
    const API_CHAR* cH = head;
    if((*cH) == '\0')
    {
        (*cO) = '.';
        ++cO;
    }

    while((*cH) != '\0')
    {
        (*cO) = (*cH) == NON_NATIVE_DIR_SEP ? NATIVE_DIR_SEP : (*cH);
        if((size_t)(cO - out) > MAX - 1)
        {
            (*cO) = '\0';
            return PATH_E_INVALID_PARAMETER;
        }
        ++cO;
        ++cH;
    }
    const API_CHAR *cT = tail;
    if((*cT) != '\0')
    {
        (*cO) = NATIVE_DIR_SEP;
        ++cO;
    }
    while((*cT) != '\0' && ((*cT) == NATIVE_DIR_SEP || (*cT) == NON_NATIVE_DIR_SEP)) ++cT;
    while((*cT) != '\0')
    {
        (*cO) = (*cT) == NON_NATIVE_DIR_SEP ? NATIVE_DIR_SEP : (*cT);
        if((size_t)(cO - out) > MAX - 1)
        {
            (*cO) = '\0';
            return PATH_E_INVALID_PARAMETER;
        }
        ++cO;
        ++cT;
    }
    (*cO) = '\0';
    return S_OK;
}

BOOL isExist(const API_CHAR* path)
{
    //ref: https://www.learnc.net/c-tutorial/c-file-exists/
    struct stat buffer;
    return stat(path, &buffer) == 0 ? TRUE: FALSE;
}

API_RESULT getLeafOfPath(API_CHAR* out, const size_t MAX, const API_CHAR* path)
{
    const API_CHAR *cP = path;
    API_CHAR *cO = out;

    while((*cP) != '\0')
    {
        if(((*cP) == NATIVE_DIR_SEP) || ((*cP) == NON_NATIVE_DIR_SEP))
        {
            cO = out;
        }
        else
        {
            (*cO) = (*cP);
            if((size_t)(cO - out) > MAX - 1)
            {
                (*cO) = '\0';
                return PATH_E_INVALID_PARAMETER;
            }
            ++cO;
        }
        ++cP;
    }
    
    (*cO) = '\0';
    return S_OK;
}