#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <iostream>
#include <string>

namespace ProcessManagerInternals
{
    class ProcessManager
    {
    public:
        ProcessManager();
        ~ProcessManager();
        static bool StartProcess(const std::string& processPath);
        static bool EndProcess(const std::string& processName);

    private:

    protected:

    };
}

#endif // PROCESS_MANAGER_H