#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <iostream>
#include <string>
#include <vector>

namespace ProcessMonitorInternals
{
    class ProcessMonitor
    {
    public:
        ProcessMonitor();
        ~ProcessMonitor();
        static std::vector<std::string> GetRunningProcesses();
        static bool IsProcessRunning(const std::string &processName);

    private:


    protected:
    };
}

#endif // PROCESS_MONITOR_H