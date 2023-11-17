#include "api/string/safe.h"
#ifdef _WIN32
#include <strsafe.h>
#else
#endif

API_RESULT stringCheckLength(const API_CHAR* str, const size_t MAX, size_t* length)
{
    if(str == NULL)
        return STRSAFE_E_INVALID_PARAMETER;

    API_RESULT result = S_OK;

    size_t cLength = 0;
    const API_CHAR* c = str;
    while((*c) != '\0')
    {
        ++cLength;
        if(cLength > MAX)
        {
            result = STRSAFE_E_INVALID_PARAMETER;
            break;
        }
        ++c;
    }

    if(length != NULL) (*length) = cLength;
    return result;
}

API_RESULT stringCopy(API_CHAR* destionation, const size_t MAX, const API_CHAR* source)
{
    if(destionation == NULL || source == NULL)
        return STRSAFE_E_INVALID_PARAMETER;

    API_RESULT result = S_OK;

    API_CHAR* cD = destionation;
    const API_CHAR* cS = source;

    while((*cS) != '\0')
    {
        (*cD) = (*cS);
        
        if((size_t)(cS - source) > MAX)
        {
            (*cD) = '\0';
            result = STRSAFE_E_INVALID_PARAMETER;
            break;
        }
        ++cD;
        ++cS;
    }
    (*cD) = '\0';

    return result;
}

API_RESULT stringCat(API_CHAR* destionation, const size_t MAX, const API_CHAR* cat)
{
    API_RESULT result = S_OK;

    API_CHAR* cD = destionation;
    while((*cD) != '\0') 
    {
        ++cD;
        if((size_t)(cD - destionation) > MAX)
            return STRSAFE_E_INVALID_PARAMETER;
    }
    const API_CHAR* cC = cat;
    while((*cC) != '\0')
    {
        (*cD) = (*cC);
        
        if((size_t)(cC - cat) > MAX)
        {
            (*cD) = '\0';
            result = STRSAFE_E_INVALID_PARAMETER;
            break;
        }
        ++cD;
        ++cC;
    }
    (*cD) = '\0';

    return result;
}

int stringCompare(const API_CHAR* str1, const API_CHAR* str2, const size_t MAX)
{
    const API_CHAR* c1 = str1;
    const API_CHAR* c2 = str2;

    while(((*c1) != '\0') || ((*c2) != '\0'))
    {
        if((size_t)(c1 - str1) > MAX)
            break;
        
        if((*c1) == (*c2))
        {
            ++c1;
            ++c2;
            continue;
        }

        return (int)((*c1) - (*c2));
    }
    return 0;
}