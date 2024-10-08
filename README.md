# Task Manager

A command-line application written in C/C++ that provides basic system monitoring and task management functionality for Windows. This application retrieves CPU usage, memory usage, and allows management of active processes, such as changing process priority or terminating a process.

## Features

- List all active processes with details such as PID, memory usage, and name
- Monitor system resources (CPU usage, memory usage)
- Kill a process by PID
- Change process priority
- Sort processes by PID, memory usage, or name

## Prerequisites

- **Windows 10 SDK** (for `pdh.h` and `psapi.h` libraries)
- **GCC** (via MinGW-w64) or **Microsoft Visual C++ Compiler**

## Building and Running
1. **Install MinGW-w64** and the **Windows SDK**.
2. **Clone the repository** and navigate to the directory:
   ```bash
   git clone https://github.com/scykl3/Task-Manager.git
   cd Task-Manager
   ```
3. **Compiling**
   ```bash
   cl /FeTaskManager.exe src\main.c src\process_management.c src\resource_monitoring.c src\user_interface.c /link pdh.lib psapi.lib
   ```
   or
   ```bash
   gcc -o TaskManager.exe main.c process_management.c resource_monitoring.c user_interface.c -lpdh -lpsapi
   ```
   Note that you may need to use the `-L` and `-I` flags for the libraries

## Usage
Once you run the application, you can use the following commands:
- `list [pid|mem|name]` - Displays and sorts the list of processes.  
- `kill <PID>` - Terminates the specified process.   
- `priority <PID> <level>` - Changes the process priority (low, normal, high, realtime).
