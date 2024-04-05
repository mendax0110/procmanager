#include "./include/ProcessMonitor.h"

#ifndef __WIN32
#include <windows.h>
#include <tlhelp32.h>
#else
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace std;
using namespace ProcessMonitorInternals;


ProcessMonitor::ProcessMonitor()
{
}

ProcessMonitor::~ProcessMonitor()
{
}

vector<string> ProcessMonitor::GetRunningProcesses()
{
    vector<string> processes;

    #ifndef __WIN32

    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cerr << "Error: CreateToolhelp32Snapshot failed" << endl;
        return processes;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        cerr << "Error: Process32First failed" << endl;
        CloseHandle(hProcessSnap);
        return processes;
    }

    do
    {
        string ws(pe32.szExeFile);
        string processName(ws.begin(), ws.end());
        processes.push_back(processName);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    #else

    DIR *dir;

    struct dirent *ent;

    if ((dir = opendir("/proc")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string pname = ent->d_name;

            if (pname.find_first_not_of("0123456789") == string::npos)
            {
                string procPath = "/proc/" + pname + "/cmdline";

                FILE* file = fopen(procPath.c_str(), "r");

                if (file)
                {
                    char buffer[1024];

                    if (fgets(name, sizeof(name), file) != NULL)
                    {
                        string processName = name;
                        processName.erase(processName.find_last_not_of(" \n") + 1);
                        processName.push_back(processName);
                    }
                    fclose(file);
                }

            }
        }
        closedir(dir);
    }

    #endif

    return processes;   
}

bool ProcessMonitor::IsProcessRunning(const string& processName)
{
    vector<string> processes = GetRunningProcesses();

    for (const string& process : processes)
    {
        if (process == processName)
        {
            return true;
        }
    }
    return false;
}