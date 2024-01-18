#include <stdio.h>
#include <stdlib.h>
#include "api/ipc/shared-memory.h"
#include "api/string/safe.h"

#ifdef _WIN32
#define SHARED_MEMORY_NAME_LENGTH 20
#define INVALID_DATA NULL
#else
#define INVALID_DATA ((void*)-1)
#endif

SharedMemory_t createSharedMemory(key_t key, size_t size)
{
    SharedMemory_t memory = { 0 };
    memory.key = key;
    memory.size = 0;
    memory.data = NULL;

#ifdef _WIN32
    API_CHAR name[SHARED_MEMORY_NAME_LENGTH];
    stringCopy(name, SHARED_MEMORY_NAME_LENGTH, "Global\\");
    API_CHAR number[15];
    API_SPRINTF(number, TEXT("%d"), key);
    stringCat(name, SHARED_MEMORY_NAME_LENGTH, number);

    memory.mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
    if(memory.mapFile == NULL)
    {
        return memory;
    }

    memory.data = MapViewOfFile(memory.mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if(memory.data == NULL)
    {
        closeSharedMemory(&memory);
        return memory;
    }
#else
    int shmid=shmget(key, size, 0666 | IPC_CREAT);
    if(shmid == -1)
    {
        return memory;
    }
    memory.data = shmat(shmid, NULL, 0);
    if(memory.data == INVALID_DATA)
    {
        closeSharedMemory(&memory);
        return memory;
    }
    
#endif
    
    memory.size = size;
    return memory;
}


SharedMemory_t openSharedMemory(key_t key, size_t size)
{
    SharedMemory_t memory = { 0 };
    memory.key = key;
    memory.size = 0;
    memory.data = NULL;

#ifdef _WIN32
    API_CHAR name[SHARED_MEMORY_NAME_LENGTH];
    stringCopy(name, SHARED_MEMORY_NAME_LENGTH, "Global\\");
    API_CHAR number[15];
    API_SPRINTF(number, TEXT("%d"), key);
    stringCat(name, SHARED_MEMORY_NAME_LENGTH, number);

    memory.mapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);
    if(memory.mapFile == NULL)
    {
        return memory;
    }

    memory.data = MapViewOfFile(memory.mapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if(memory.data == NULL)
    {
        closeSharedMemory(&memory);
        return memory;
    }

#else
    int shmid=shmget(key, size, 0666);
    if(shmid == -1)
    {
        return memory;
    }
    memory.data = shmat(shmid, NULL, 0);
    if(memory.data == INVALID_DATA)
    {
        closeSharedMemory(&memory);
        return memory;
    }
#endif
    memory.size = size;
    return memory;
}

BOOL isOpenSharedMemory(const SharedMemory_t* sharedMemory)
{
    return
        (sharedMemory != NULL) &&
#ifdef _WIN32
        (sharedMemory->mapFile != NULL) &&
#else
#endif
        (sharedMemory->data != INVALID_DATA) && (sharedMemory->size > 0);
}
void closeSharedMemory(const SharedMemory_t* sharedMemory)
{
    if(sharedMemory == NULL)
        return;

#ifdef _WIN32
    if(sharedMemory->data != NULL)
        UnmapViewOfFile(sharedMemory->data);

    if(sharedMemory->mapFile != NULL)
        CloseHandle(sharedMemory->mapFile);
#else
#endif
}