#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "process_management.h"
#include "resource_monitoring.h"
#include "user_interface.h"

int main() {
    if (!InitializeResourceMonitoring()) {
        fprintf(stderr, "Failed to initialize resource monitoring.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        system("cls");

        DisplaySystemResources();

        DisplayProcessList();

        if (!ProcessUserInput()) {
            break;
        }

        Sleep(1000); 
    }

    CleanupResourceMonitoring();

    return EXIT_SUCCESS;
}
