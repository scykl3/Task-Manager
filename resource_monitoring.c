#include "resource_monitoring.h"
#include <pdh.h>
#include <pdhmsg.h>
#include <stdio.h>

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

int InitializeResourceMonitoring() {
    PDH_STATUS status = PdhOpenQuery(NULL, 0, &cpuQuery);
    if (status != ERROR_SUCCESS) return 0;

    status = PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
    if (status != ERROR_SUCCESS) return 0;

    status = PdhCollectQueryData(cpuQuery);
    if (status != ERROR_SUCCESS) return 0;

    return 1;
}

void DisplaySystemResources() {
    PDH_FMT_COUNTERVALUE counterVal;
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    double cpuUsage = counterVal.doubleValue;

    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    double memUsagePercent = memInfo.dwMemoryLoad;

    printf("System Resources:\n");
    printf("CPU Usage: %.2f%%\n", cpuUsage);
    printf("Memory Usage: %.2f%% (%llu MB / %llu MB)\n\n",
        memUsagePercent,
        physMemUsed / (1024 * 1024),
        totalPhysMem / (1024 * 1024));
}

void CleanupResourceMonitoring() {
    PdhCloseQuery(cpuQuery);
}
