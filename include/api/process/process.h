#ifndef _PROCESS_PROCESS_H_
#define _PROCESS_PROCESS_H_

#include "api/basetype.h"
#ifdef _WIN32
#include <tlhelp32.h>
#else
#endif

typedef struct
{
#ifdef _WIN32
    HANDLE snapshot;
    PROCESSENTRY32 pe;
    size_t size;
#else
#endif
} ProcessSnapshot;

typedef struct
{
    unsigned long pid;
    unsigned long nThreads;
    unsigned long ppid;
    API_CHAR exeFile[API_MAX_PATH];
} ProcessAPI;

START_EXTERN

EXTENDAPI ProcessSnapshot* takeProcessSnapshot(void);
EXTENDAPI BOOL readProcessSnapshot(ProcessSnapshot* snapshoot, ProcessAPI* process);
EXTENDAPI API_RESULT closeProcessSnapshot(ProcessSnapshot *snapshot);

END_EXTERN

#endif //!_PROCESS_PROCESS_H_