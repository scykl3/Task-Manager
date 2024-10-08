#include "process_management.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>

int GetProcessList(ProcessInfo** processList, int* processCount) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    int count = 0;
    ProcessInfo* list = NULL;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }

    do {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        SIZE_T memUsage = 0;

        if (hProcess) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                memUsage = pmc.WorkingSetSize;
            }
            CloseHandle(hProcess);
        }

        list = realloc(list, sizeof(ProcessInfo) * (count + 1));
        list[count].pid = pe32.th32ProcessID;
        strncpy(list[count].processName, pe32.szExeFile, MAX_PATH);
        list[count].memoryUsage = memUsage;
        count++;

    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    *processList = list;
    *processCount = count;

    return 1;
}

void FreeProcessList(ProcessInfo* processList, int processCount) {
    free(processList);
}

int TerminateProcessByPID(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!hProcess) {
        return 0;
    }
    int result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}

int ChangeProcessPriority(DWORD pid, DWORD priorityClass) {
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
    if (!hProcess) {
        return 0;
    }
    int result = SetPriorityClass(hProcess, priorityClass);
    CloseHandle(hProcess);
    return result;
}

int CompareByPID(const void* a, const void* b) {
    ProcessInfo* p1 = (ProcessInfo*)a;
    ProcessInfo* p2 = (ProcessInfo*)b;
    return (int)(p1->pid - p2->pid);
}

int CompareByMemory(const void* a, const void* b) {
    ProcessInfo* p1 = (ProcessInfo*)a;
    ProcessInfo* p2 = (ProcessInfo*)b;
    if (p1->memoryUsage > p2->memoryUsage) return -1;
    if (p1->memoryUsage < p2->memoryUsage) return 1;
    return 0;
}

int CompareByName(const void* a, const void* b) {
    ProcessInfo* p1 = (ProcessInfo*)a;
    ProcessInfo* p2 = (ProcessInfo*)b;
    return strcmp(p1->processName, p2->processName);
}
