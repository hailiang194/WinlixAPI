#ifndef _IPC_SHARED_MEMORY_H_
#define _IPC_SHARED_MEMORY_H_

#ifdef _WIN32
typedef int key_t;
#else
#include <sys/shm.h>
#endif

#include "api/basetype.h"

typedef struct 
{
    void* data;
    size_t size;
    key_t key;
#ifdef _WIN32
    HANDLE mapFile;
#else
#endif
} SharedMemory_t;

START_EXTERN

EXTENDAPI SharedMemory_t createSharedMemory(key_t key, size_t size);
EXTENDAPI SharedMemory_t openSharedMemory(key_t key, size_t size);
EXTENDAPI BOOL isOpenSharedMemory(const SharedMemory_t* sharedMemory);
EXTENDAPI void closeSharedMemory(const SharedMemory_t* sharedMemory);

END_EXTERN

#endif //!_IPC_SHARED_MEMORY_H_