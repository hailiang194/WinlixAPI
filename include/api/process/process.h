#ifndef _PROCESS_PROCESS_H_
#define _PROCESS_PROCESS_H_

#include "api/basetype.h"
#ifdef _WIN32
#include <tlhelp32.h>
#else
#include "api/directory/directory.h"
#endif

typedef struct
{
#ifdef _WIN32
    HANDLE snapshot;
    PROCESSENTRY32 pe;
    size_t size;
#else
    API_DIR* procDir;
    API_DIR_CHILD childDir;
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

/*
 * WARNING: Those needs administrator privileges
 */

EXTENDAPI BOOL suspendProcess(const unsigned long pid);
EXTENDAPI BOOL resumeProcess(const unsigned long pid);
EXTENDAPI BOOL terminateProcess(const unsigned long pid);

END_EXTERN

#endif //!_PROCESS_PROCESS_H_