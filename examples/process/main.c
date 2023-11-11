#include <stdio.h>
#include <stdlib.h>
#include "api/basetype.h"
#include "api/process/process.h"

int main(void)
{
    ProcessSnapshot* snapshot = takeProcessSnapshot();
    ProcessAPI process;
    API_PRINTF(TEXT("%10s|%100s|%10s|%10s|\n"), "PID", "EXE", "PPID", "THREADS");
    while(readProcessSnapshot(snapshot, &process))
    {
        API_PRINTF(TEXT("%10d|%100s|%10d|%10d|\n"), process.pid, process.exeFile, process.ppid, process.nThreads);
    }
    closeProcessSnapshot(snapshot);
    return 0;
}