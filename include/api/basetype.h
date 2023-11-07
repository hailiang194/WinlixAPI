#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "User32.lib")
#else
#include <stdio.h>
#include <limits.h>
#include <linux/limits.h>
#endif

#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define EXTENDAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define EXTENDAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef EXTENDAPI
    #define EXTENDAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#ifdef __cplusplus
#define START_EXTERN extern "C" {
#define END_EXTERN }
#else
#define START_EXTERN
#define END_EXTERN
#endif

#ifdef _WIN32
#define API_MAX_PATH MAX_PATH
#define API_PRINTF _tprintf
typedef HRESULT API_RESULT;
typedef TCHAR API_CHAR;
#else
#define BOOL int
#define TRUE !0
#define FALSE 0
#define API_MAX_PATH PATH_MAX 
#define API_PRINTF printf
#define TEXT(CONTENT) CONTENT

#define SUCCEEDED(hr)   (((API_RESULT)(hr)) >= 0)
#define FAILED(hr)      (((API_RESULT)(hr)) < 0)

#define S_OK            ((API_RESULT)0L)

typedef long API_RESULT;
typedef char API_CHAR;
#endif


#endif //!BASE_TYPE_H