#include "api/process/process.h"
#include "api/string/safe.h"
#ifdef _WIN32
#else
#include <string.h>
#include <stdlib.h>
#include "api/string/validate.h"
#include "api/directory/path.h"
#include <signal.h>
#endif

ProcessSnapshot* takeProcessSnapshot(void)
{
    ProcessSnapshot *snapshot = (ProcessSnapshot*)malloc(sizeof(ProcessSnapshot));
    if(snapshot == NULL)
        return NULL;

    memset(snapshot, 0x0, sizeof(ProcessSnapshot));
#ifdef _WIN32
    snapshot->size = 0;
    snapshot->snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == snapshot->snapshot)
    {
        free(snapshot);
        return NULL;
    }
    (snapshot->pe).dwSize = sizeof(PROCESSENTRY32);
#else
    snapshot->procDir = openDir("/proc");
#endif
    return snapshot;
}

BOOL readProcessSnapshot(ProcessSnapshot* snapshoot, ProcessAPI* process)
{
    if(snapshoot == NULL || process == NULL)
        return FALSE;

    BOOL hResult = FALSE;

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
    while(readDir(snapshoot->procDir, &(snapshoot->childDir)))
    {
        long long int procId = 0;
        if(!hasOnlyDigits(snapshoot->childDir.d_name, &procId))
            continue;
        //get proc id
        API_CHAR procPath[API_MAX_PATH];
        joinPath(procPath, API_MAX_PATH, snapshoot->procDir->path, snapshoot->childDir.d_name);
        
        
        //get proc name, pid, ppid
        API_CHAR statPath[API_MAX_PATH];
        joinPath(statPath, API_MAX_PATH, procPath, "stat");
        if(!isExist(statPath))
            continue;

        long long int tempNum;
        API_CHAR tempStr[API_MAX_PATH];

        FILE* statFile = fopen(statPath, "r");
        if(statFile == NULL)
            continue;
        
        fscanf(statFile, "%lu", &(process->pid));
        fscanf(statFile, " (%s) ", process->exeFile);
        size_t exeSize = 0;
        stringCheckLength(process->exeFile, API_MAX_PATH, &exeSize);
        process->exeFile[exeSize - 1] = '\0';
        API_CHAR state;
        fscanf(statFile, " %c", &state);
        fscanf(statFile ,"%lu", &(process->ppid));
        
        fclose(statFile);
        
        //get num threads
        process->nThreads = 0;
        API_CHAR taskFolder[API_MAX_PATH];
        joinPath(taskFolder, API_MAX_PATH, procPath, "task");
        API_DIR* taskDir = openDir(taskFolder);
        
        if(taskDir == NULL)
            continue;

        API_DIR_CHILD taskChild;
        while(readDir(taskDir, &taskChild))
        {
            ++(process->nThreads);
        }

        (process->nThreads) -= 2; //not counting ./ and ../
        closeDir(taskDir);

        //because filename of the executable is 16 character limit and some of them can't access the exe
        API_CHAR exePath[API_MAX_PATH];
        joinPath(exePath, API_MAX_PATH, procPath, "exe");
        if(isExist(exePath))
        {
            API_CHAR procNamePath[API_MAX_PATH];
            if(realpath(exePath, procNamePath) == NULL)
                continue;
            API_CHAR exeFile[API_MAX_PATH];
            getLeafOfPath(exeFile, API_MAX_PATH, procNamePath);
            stringCopy(process->exeFile, API_MAX_PATH, exeFile);
        }
        
        return TRUE;
    }
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
    closeDir(snapshot->procDir);
#endif
    free(snapshot);
    return S_OK;
}

BOOL suspendProcess(const unsigned long pid)
{
#ifdef _WIN32
    //ref: https://stackoverflow.com/questions/11010165/how-to-suspend-resume-a-process-in-windows
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry; 
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == pid)
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);
            
            SuspendThread(hThread);
            
            CloseHandle(hThread);
        }
    } while (Thread32Next(hThreadSnapshot, &threadEntry));

    CloseHandle(hThreadSnapshot);
#else
    kill(pid, SIGSTOP);
#endif
    return TRUE;
}

BOOL resumeProcess(const unsigned long pid)
{
#ifdef _WIN32
    //ref: https://stackoverflow.com/questions/11010165/how-to-suspend-resume-a-process-in-windows
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry; 
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == pid)
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);
            
            ResumeThread(hThread);
            CloseHandle(hThread);
        }
    } while (Thread32Next(hThreadSnapshot, &threadEntry));

    CloseHandle(hThreadSnapshot);
#else
    kill(pid, SIGCONT);
#endif
    return TRUE;
}

BOOL terminateProcess(const unsigned long pid)
{
#ifdef _WIN32
    //ref: https://stackoverflow.com/questions/11010165/how-to-suspend-resume-a-process-in-windows
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry; 
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == pid)
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);
            
            TerminateThread(hThread, EXIT_SUCCESS);
            CloseHandle(hThread);
        }
    } while (Thread32Next(hThreadSnapshot, &threadEntry));

    CloseHandle(hThreadSnapshot);
#else
    kill(pid, SIGINT);
#endif
    return TRUE;
}