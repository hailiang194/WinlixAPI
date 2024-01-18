#include <stdio.h>
#include <stdlib.h>
#include "api/ipc/shared-memory.h"
#include "api/string/safe.h"
#ifdef _WIN32
#else
#include <unistd.h>
#endif

int main(void)
{
    API_PRINTF(TEXT("CREATING SHARED MEMORY...\n"));
    SharedMemory_t memory = createSharedMemory((key_t)1308, 1024);

    if(!isOpenSharedMemory(&memory))
    {
        API_PRINTF(TEXT("Failed to create memory\n"));
        closeSharedMemory(&memory);
        return -1;
    }

    API_PRINTF(TEXT("Create memory successfully\n"));

    stringCopy((API_CHAR*)memory.data, 100, TEXT("HELLO WORLD"));

#ifdef _WIN32
    Sleep(10000);
#else
    sleep(10);
#endif

    closeSharedMemory(&memory);
    return 0;
}