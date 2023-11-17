#ifndef _STRING_SAFE_H_
#define _STRING_SAFE_H_

#include "api/basetype.h"

#ifdef _WIN32

#else
#define STRSAFE_E_INSUFFICIENT_BUFFER           ((API_RESULT)0x8007007AL)  // 0x7A = 122L = ERROR_INSUFFICIENT_BUFFER
#define STRSAFE_E_INVALID_PARAMETER             ((API_RESULT)0x80070057L)  // 0x57 =  87L = ERROR_INVALID_PARAMETER
#define STRSAFE_E_END_OF_FILE                   ((API_RESULT)0x80070026L)  // 0x26 =  38L = ERROR_HANDLE_EOF
#endif

START_EXTERN

EXTENDAPI API_RESULT stringCheckLength(const API_CHAR* str, const size_t MAX, size_t* length);
EXTENDAPI API_RESULT stringCopy(API_CHAR* destionation, const size_t MAX, const API_CHAR* source);
EXTENDAPI API_RESULT stringCat(API_CHAR* destionation, const size_t MAX, const API_CHAR* cat);
EXTENDAPI int stringCompare(const API_CHAR* str1, const API_CHAR* str2, const size_t MAX);

END_EXTERN

#endif //!_STRING_SAFE_H_