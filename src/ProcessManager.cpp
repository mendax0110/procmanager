#include "./include/ProcessManager.h"

#ifdef __WIN32
#include <windows.h>
#include <TlHelp32.h>
#endif

#ifdef __LINUX
#include <unistd.h>
#include <sys/types.h>
#include <singal.h>
#endif

#ifdef __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <signal.h>
#endif


using namespace std;
using namespace ProcessManagerInternals;


ProcessManager::ProcessManager()
{
}

ProcessManager::~ProcessManager()
{
}

bool ProcessManager::StartProcess(const string& processPath)
{
    #ifdef __WIN32
    wstring wstr(processPath.begin(), processPath.end());

    if (ShellExecuteW(NULL, L"open", wstr.c_str(), NULL, NULL, SW_SHOWNORMAL) > (HINSTANCE)32)
    {
        return true;
    }

    std::cerr << "Error: ShellExecute failed" << endl;
    return false;
    #endif

    #ifdef __LINUX
    pid_t pid = fork();

    if (pid == 0)
    {
        excel(processPath.c_str(), processPath.c_str(), nullptr);
        cerr << "Error: Failed to start process" << endl;
        return false;
    }
    else if (pid > 0)
    {
        return true;
    }
    else
    {
        cerr << "Error: Failed to fork process" << endl;
        return false;
    }
    #endif

    #ifdef __APPLE__
    pid_t pid = fork();

    if (pid == 0)
    {
        char* args[] = {const_cast<char*>(processPath.c_str()), nullptr};
        execvp(processPath.c_str(), args);

        cerr << "Error: Failed to start process" << endl;
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            return true;
        }
        else
        {
            cerr << "Error: Child process terminated abnormally" << endl;
            return false;
        }
    }
    else
    {
        cerr << "Error: Failed to fork process" << endl;
        return false;
    }
    #endif
}

bool ProcessManager::EndProcess(const string& processName)
{
    #ifdef __WIN32
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe))
    {
        do
        {   
            wchar_t wProcessName[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, pe.szExeFile, -1, wProcessName, MAX_PATH);

            if (_wcsicmp(wProcessName, wstring(processName.begin(), processName.end()).c_str()) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);

                if (hProcess != nullptr)
                {
                    if (TerminateProcess(hProcess, 0))
                    {
                        cout << "Process " << processName << " terminated" << endl;
                        CloseHandle(hProcess);
                        CloseHandle(hSnap);
                        return true;
                    }
                    else
                    {
                        std::cerr << "Error: Failed to terminate process" << endl;
                        CloseHandle(hProcess);
                        CloseHandle(hSnap);
                        return false;
                    }
                }
                else
                {
                    std::cerr << "Error: Failed to open process" << endl;
                    return false;
                }
            }
        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
    std::cerr << "Error: Process " << processName << " not found" << endl;
    return false;
    #endif

    #ifdef __LINUX
    pid_t pid = 0;
    FILE* pipe = popen(("pidof " + processName).c_str(), "r");

    if (pipe != nullptr)
    {
        if (fscanf(pipe, "%d", &pid) == EOF)
        {
            cerr << "Error: Failed to get process ID" << endl;
            pclose(pipe);
            return false;
        }
        pclose(pipe);

        if (kill(pid, SIGTERM) == 0)
        {
            cout << "Process " << processName << " terminated" << endl;
            return true;
        }
        else
        {
            cerr << "Error: Failed to terminate process" << endl;
            return false;
        }
    }
    else
    {
        cerr << "Error: Failed to get process ID" << endl;
        return false;
    }
    #endif

    #ifdef __APPLE__
    pid_t pid = 0;
    FILE* pipe = popen(("pidof " + processName).c_str(), "r");

    if (pipe != nullptr)
    {
        if (fscanf(pipe, "%d", &pid) == EOF)
        {
            cerr << "Error: Failed to get process ID" << endl;
            pclose(pipe);
            return false;
        }
        pclose(pipe);

        if (kill(pid, SIGTERM) == 0)
        {
            cout << "Process " << processName << " terminated" << endl;
            return true;
        }
        else
        {
            cerr << "Error: Failed to terminate process" << endl;
            return false;
        }
    }
    else
    {
        cerr << "Error: Failed to get process ID" << endl;
        return false;
    }
    #endif
}

