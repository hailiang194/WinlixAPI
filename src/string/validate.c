#include "api/string/validate.h"

BOOL hasOnlyDigits(const API_CHAR* str, long long int* value)
{
    if(value != NULL) (*value) = 0;
    const API_CHAR* c = str;
    while((*c) != '\0')
    {
        if(((*c) < '0') || ((*c) > '9'))
        {
            return FALSE;
        }
        if(value != NULL)
        {
            (*value) *= 10;
            (*value) += ((*c) - '0');
        }
        ++c;
    }
    return TRUE;
}