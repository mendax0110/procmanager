#include "./include/TaskScheduler.h"

#ifdef _WIN32
#include <windows.h>
#include <taskschd.h>
#include <variant>
#include <comutil.h>
#include <comdef.h>
#pragma comment(lib, "taskschd.lib")
#endif

#ifdef __LINUX
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#ifdef __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <signal.h>
#endif

#include <iostream>


using namespace std;
using namespace TaskSchedulerInternals;

TaskSchedule::TaskSchedule()
{
}

TaskSchedule::~TaskSchedule()
{
}

bool TaskSchedule::ScheduleTask(const string& taskPath, const string& taskName, const string& taskTime)
{
    #ifdef _WIN32
    ITaskService* pService = NULL;
    ITaskFolder* pRootFolder = NULL;
    IRegisteredTask* pTask = NULL;
    HRESULT hr = S_OK;

    hr = CoInitialize(NULL);

    if (FAILED(hr))
    {
        cerr << "Error: CoInitialize failed" << endl;
        return false;
    }

    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);

    if (FAILED(hr))
    {
        cerr << "Error: CoCreateInstance failed" << endl;
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());

    if (FAILED(hr))
    {
        cerr << "Error: Connect failed" << endl;
        pService->Release();
        return false;
    }

    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

    if (FAILED(hr))
    {
        cerr << "Error: GetFolder failed" << endl;
        pService->Release();
        return false;
    }

    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(taskName.c_str()),
        NULL,
        //LONG(taskPath.c_str()),
        static_cast<long>(taskPath.size() + 1),
        variant_t(L""),
        variant_t(L""),
        TASK_LOGON_TYPE::TASK_LOGON_INTERACTIVE_TOKEN,
        variant_t(L""),
        /*_bstr_t(L""),
        _bstr_t(L""),
        _bstr_t(L""),
        _bstr_t(L""),*/
        &pTask
    );

    if (FAILED(hr))
    {
        cerr << "Error: RegisterTaskDefinition failed" << endl;
        pRootFolder->Release();
        pService->Release();
        return false;
    }

    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return true;
    #endif

    #ifdef __LINUX
    pid_t pid = fork();

    if (pid == 0)
    {
        excel(taskPath.c_str(), taskPath.c_str(), nullptr);
        cerr << "Error: Failed to start task" << endl;
        return false;
    }
    else if (pid > 0)
    {
        return true;
    }
    #endif

    #ifdef __APPLE__
    pid_t pid = fork();

    if (pid == 0)
    {
        char* args[] = {const_cast<char*>(taskPath.c_str()), nullptr};
        execvp(taskPath.c_str(), args);

        cerr << "Error: Failed to start task" << endl;
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
            cerr << "Error: Child task terminated abnormally" << endl;
            return false;
        }
    }

    cerr << "Error: Failed to fork task" << endl;

    return false;
    #endif
}

bool TaskSchedule::UnscheduleTask(const string& taskName)
{
    #ifdef _WIN32
    ITaskService* pService = NULL;
    ITaskFolder* pRootFolder = NULL;
    HRESULT hr = S_OK;

    hr = CoInitialize(NULL);

    if (FAILED(hr))
    {
        cerr << "Error: CoInitialize failed" << endl;
        return false;
    }

    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);

    if (FAILED(hr))
    {
        cerr << "Error: CoCreateInstance failed" << endl;
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());

    if (FAILED(hr))
    {
        cerr << "Error: Connect failed" << endl;
        pService->Release();
        return false;
    }

    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

    if (FAILED(hr))
    {
        cerr << "Error: GetFolder failed" << endl;
        pService->Release();
        return false;
    }

    hr = pRootFolder->DeleteTask(_bstr_t(taskName.c_str()), 0);

    if (FAILED(hr))
    {
        cerr << "Error: DeleteTask failed" << endl;
        pRootFolder->Release();
        pService->Release();
        return false;
    }

    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return true;
    #endif

    #ifdef __LINUX
    pid_t pid = fork();

    if (pid == 0)
    {
        kill(pid, SIGKILL);
        cerr << "Error: Failed to unschedule task" << endl;
        return false;
    }
    else if (pid > 0)
    {
        return true;
    }
    #endif

    #ifdef __APPLE__
    pid_t pid = fork();

    if (pid == 0)
    {
        kill(pid, SIGKILL);
        cerr << "Error: Failed to unschedule task" << endl;
        return false;
    }
    else if (pid > 0)
    {
        return true;
    }

    cerr << "Error: Failed to fork task" << endl;

    return false;
    #endif
}

vector<string> TaskSchedule::GetScheduledTasks()
{
    vector<string> tasks;

    #ifdef _WIN32
    ITaskService* pService = NULL;
    ITaskFolder* pRootFolder = NULL;
    IRegisteredTaskCollection* pTaskCollection = NULL;
    HRESULT hr = S_OK;

    hr = CoInitialize(NULL);

    if (FAILED(hr))
    {
        cerr << "Error: CoInitialize failed" << endl;
        return tasks;
    }

    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);

    if (FAILED(hr))
    {
        cerr << "Error: CoCreateInstance failed" << endl;
        return tasks;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());

    if (FAILED(hr))
    {
        cerr << "Error: Connect failed" << endl;
        pService->Release();
        return tasks;
    }

    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

    if (FAILED(hr))
    {
        cerr << "Error: GetFolder failed" << endl;
        pService->Release();
        return tasks;
    }

    hr = pRootFolder->GetTasks(0, &pTaskCollection);

    if (FAILED(hr))
    {
        cerr << "Error: GetTasks failed" << endl;
        pRootFolder->Release();
        pService->Release();
        return tasks;
    }

    LONG numTasks = 0;
    hr = pTaskCollection->get_Count(&numTasks);

    if (FAILED(hr))
    {
        cerr << "Error: get_Count failed" << endl;
        pTaskCollection->Release();
        pRootFolder->Release();
        pService->Release();
        return tasks;
    }

    for (LONG i = 0; i < numTasks; i++)
    {
        IRegisteredTask* pTask = NULL;
        hr = pTaskCollection->get_Item(_variant_t(i + 1), &pTask);

        if (FAILED(hr))
        {
            cerr << "Error: get_Item failed" << endl;
            pTaskCollection->Release();
            pRootFolder->Release();
            pService->Release();
            return tasks;
        }

        BSTR taskName = NULL;
        hr = pTask->get_Name(&taskName);

        if (FAILED(hr))
        {
            cerr << "Error: get_Name failed" << endl;
            pTask->Release();
            pTaskCollection->Release();
            pRootFolder->Release();
            pService->Release();
            return tasks;
        }

        tasks.push_back(reinterpret_cast<const char*>(taskName));
        SysFreeString(taskName);
        pTask->Release();
    }

    pTaskCollection->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return tasks;

    #endif

    #ifdef __LINUX
    DIR* dir;

    if ((dir = opendir("/proc")) == NULL)
    {
        cerr << "Error: opendir failed" << endl;
        return tasks;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_DIR)
        {
            string name = ent->d_name;

            if (all_of(name.begin(), name.end(), ::isdigit))
            {
                tasks.push_back(name);
            }
        }
    }
    closedir(dir);
    #endif

    #ifdef __APPLE__
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
    size_t len;
    struct kinfo_proc* procs;
    int status;

    status = sysctl(mib, 4, NULL, &len, NULL, 0);
    if (status == -1)
    {
        perror("sysctl");
        return tasks;
    }

    procs = (struct kinfo_proc*)malloc(len);
    status = sysctl(mib, 4, procs, &len, NULL, 0);
    if (status == -1)
    {
        perror("sysctl");
        free(procs);
        return tasks;
    }

    int nprocs = len / sizeof(struct kinfo_proc);
    for (int i = 0; i < nprocs; i++)
    {
        tasks.push_back(to_string(procs[i].kp_proc.p_pid));
    }
        

    free(procs);
    return tasks;
    #endif
}