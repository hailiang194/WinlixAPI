#include <stdio.h>
#include <stdlib.h>
#include "api/string/safe.h"
#include "api/process/process.h"

enum MODE {
    TERMINATE,
    SUSPEND,
    RESUME,
    NOT_FOUND
};

int main(int argc, API_CHAR** argv)
{
    if(argc != 3)
    {
        API_PRINTF(TEXT("\nUsage: %s <process name> <TERM,SUS,RES>\n"), argv[0]);
        return (-1);
    }

    enum MODE mode = NOT_FOUND;
    if(stringCompare(argv[2], "TERM", 10) == 0)
    {
        API_PRINTF(TEXT("Terminate process\n"));
        mode = TERMINATE;
    }
    else if(stringCompare(argv[2], "SUS", 10) == 0)
    {
        API_PRINTF(TEXT("Suspend process\n"));
        mode = SUSPEND;
    }
    else if(stringCompare(argv[2], "RES", 10) == 0)
    {
        API_PRINTF(TEXT("Resume process\n"));
        mode = RESUME;
    }
    else
    {
        API_PRINTF(TEXT("Mode not found\n"));
        return (-1);
    }
    API_PRINTF(TEXT("FINDING %s\n"), argv[1]);
    ProcessSnapshot* snapshot = takeProcessSnapshot();
    ProcessAPI process;
    API_PRINTF(TEXT("%10s|%100s|%10s|%10s|\n"), "PID", "EXE", "PPID", "THREADS");

    while(readProcessSnapshot(snapshot, &process))
    {
        if(stringCompare(process.exeFile, argv[1], API_MAX_PATH) == 0)
        {
            API_PRINTF(TEXT("%10d|%100s|%10d|%10d|\n"), process.pid, process.exeFile, process.ppid, process.nThreads);
            switch(mode)
            {
                case TERMINATE:
                    terminateProcess(process.pid);  
                    break;
                case SUSPEND:
                    suspendProcess(process.pid);
                    break;
                case RESUME:
                    resumeProcess(process.pid);
                    break;
            }

        }
            
    }
    closeProcessSnapshot(snapshot);
    return 0;
}