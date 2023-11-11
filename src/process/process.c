#include "api/process/process.h"
#include "api/string/safe.h"

ProcessSnapshot* takeProcessSnapshot(void)
{
    ProcessSnapshot *snapshot = (ProcessSnapshot*)malloc(sizeof(ProcessSnapshot));
    if(snapshot == NULL)
        return NULL;

    memset(snapshot, 0x0, sizeof(ProcessSnapshot));
    snapshot->size = 0;
#ifdef _WIN32
    snapshot->snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == snapshot->snapshot)
    {
        free(snapshot);
        return NULL;
    }
    (snapshot->pe).dwSize = sizeof(PROCESSENTRY32);
#else
#endif
    return snapshot;
}

BOOL readProcessSnapshot(ProcessSnapshot* snapshoot, ProcessAPI* process)
{
    if(snapshoot == NULL || process == NULL)
        return FALSE;

    BOOL hResult;

#ifdef _WIN32
    if(snapshoot->size == 0)
    {
        hResult = Process32First(snapshoot->snapshot, &(snapshoot->pe));
    }
    else
    {
        hResult = Process32Next(snapshoot->snapshot, &(snapshoot->pe));
    }

    if(!hResult)
        return FALSE;
    
    ++(snapshoot->size);
    process->pid = snapshoot->pe.th32ProcessID;
    process->ppid = snapshoot->pe.th32ParentProcessID;
    process->nThreads = snapshoot->pe.cntThreads;
    stringCopy(process->exeFile, API_MAX_PATH, snapshoot->pe.szExeFile);
#else
#endif
    return hResult;
}

API_RESULT closeProcessSnapshot(ProcessSnapshot *snapshot)
{
    if(snapshot == NULL)
        return (API_RESULT)-1;

#ifdef _WIN32
    if(snapshot->snapshot == NULL)
        return (API_RESULT)-1;

    CloseHandle(snapshot->snapshot);
#else
#endif
    free(snapshot);
    return S_OK;
}