#include <stdio.h>
#include <stdlib.h>
#include "api/ipc/shared-memory.h"

int main(void)
{
    API_PRINTF(TEXT("READ SHARED MEMORY...\n"));
    SharedMemory_t memory = openSharedMemory((key_t)1308, 1024);

    if(!isOpenSharedMemory(&memory))
    {
        API_PRINTF(TEXT("Failed to read memory\n"));
        closeSharedMemory(&memory);
        return -1;
    }

    API_PRINTF(TEXT("Read memory successfully\n"));
    API_PRINTF(TEXT("%s\n"), (API_CHAR*)memory.data);
    
    closeSharedMemory(&memory);
    return 0;
}