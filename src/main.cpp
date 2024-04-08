#include "./include/ProcessManager.h"
#include "./include/ProcessMonitor.h"
#include "./include/TaskScheduler.h"
#include "./include/CmakeCreator.h"
#include "./include/Analyzer.h"

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace ProcessManagerInternals;
using namespace ProcessMonitorInternals;
using namespace TaskSchedulerInternals;
using namespace CmakeCreatorInternals;
using namespace AnalyzerInternals;


void checkProcess(const string& processName);
void startProcess(const string& processPath);
void endProcess(const string& processName);
void listRunningProcesses();
void scheduleTask(const string& taskName, const string& taskPath, const string& taskTime);
void unscheduleTask(const string& taskName);
void listScheduledTasks();
void createCmakeFile(const string& projectName, const string& projectPath);
void createBuildDirectoryAndBuild(const string& projectPath);
void detectCmakeAndCompiler();
void createGitIgnore(const string& projectPath);
void createReadMe(const string& projectPath);
void binaryAnalysis(const string& binaryPath);
void help();


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <option> [arguments...]" << endl;
        return 1;
    }

    string helpFlag = "--help";

    if (argv[1] == helpFlag)
    {
        help();
        return 0;
    }

    int option = atoi(argv[1]);

    switch (option)
    {
        case 1:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 1 <processName>" << endl;
                return 1;
            }
            checkProcess(argv[2]);
            break;
        case 2:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 2 <processPath>" << endl;
                return 1;
            }
            startProcess(argv[2]);
            break;
        case 3:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 3 <processName>" << endl;
                return 1;
            }
            endProcess(argv[2]);
            break;
        case 4:
            listRunningProcesses();
            break;
        case 5:
            if (argc < 5)
            {
                cout << "Usage: " << argv[0] << " 5 <taskName> <taskPath> <taskTime>" << endl;
                return 1;
            }
            scheduleTask(argv[2], argv[3], argv[4]);
            break;
        case 6:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 6 <taskName>" << endl;
                return 1;
            }
            unscheduleTask(argv[2]);
            break;
        case 7:
            listScheduledTasks();
            break;
        case 8:
            if (argc < 4)
            {
                cout << "Usage: " << argv[0] << " 8 <projectName> <projectPath>" << endl;
                return 1;
            }
            createCmakeFile(argv[2], argv[3]);
            break;
        case 9:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 9 <projectPath>" << endl;
                return 1;
            }
            createBuildDirectoryAndBuild(argv[2]);
            break;
        case 10:
            detectCmakeAndCompiler();
            break;
        case 11:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 11 <projectPath>" << endl;
                return 1;
            }
            createGitIgnore(argv[2]);
            break;
        case 12:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 12 <projectPath>" << endl;
                return 1;
            }
            createReadMe(argv[2]);
            break;
        case 13:
            if (argc < 3)
            {
                cout << "Usage: " << argv[0] << " 13 <binaryPath>" << endl;
                return 1;
            }
            binaryAnalysis(argv[2]);
            break;
        case 14:
            cout << "Exiting...";
            std::exit(0);
            break;
        default:
            cout << "Invalid option" << endl;
            return 1;
    }

    return 0;
}

void checkProcess(const string& processName)
{
    if (ProcessMonitor::IsProcessRunning(processName))
        cout << "Process is running" << endl;
    else
        cout << "Process is not running" << endl;
}

void startProcess(const string& processPath)
{
    if (ProcessManager::StartProcess(processPath))
        cout << "Process started successfully" << endl;
    else
        cout << "Failed to start process" << endl;
}

void endProcess(const string& processName)
{
    if (ProcessManager::EndProcess(processName))
        cout << "Process ended successfully" << endl;
    else
        cout << "Failed to end process" << endl;
}

void listRunningProcesses()
{
    vector<string> processes = ProcessMonitor::GetRunningProcesses();
    for (const string& process : processes)
        cout << process << endl;
}

void scheduleTask(const string& taskName, const string& taskPath, const string& taskTime)
{
    if (TaskScheduler::ScheduleTask(taskName, taskPath, taskTime))
        cout << "Task scheduled successfully" << endl;
    else
        cout << "Failed to schedule task" << endl;
}

void unscheduleTask(const string& taskName)
{
    if (TaskScheduler::UnscheduleTask(taskName))
        cout << "Task unscheduled successfully" << endl;
    else
        cout << "Failed to unschedule task" << endl;
}

void listScheduledTasks()
{
    vector<string> tasks = TaskScheduler::GetScheduledTasks();
    for (const string& task : tasks)
        cout << task << endl;
}

void createCmakeFile(const string& projectName, const string& projectPath)
{
    if (CmakeCreator::CreateCmakeFile(projectName, projectPath))
        cout << "Cmake file created successfully" << endl;
    else
        cout << "Failed to create Cmake file" << endl;
}

void createBuildDirectoryAndBuild(const string& projectPath)
{
    if (CmakeCreator::CreateBuildDirAndBuild(projectPath))
        cout << "Build directory created and project built successfully" << endl;
    else
        cout << "Failed to create build directory and build project" << endl;
}

void detectCmakeAndCompiler()
{
    CmakeCreator cmakeCreator;
    cout << "Cmake and compiler detected: " << cmakeCreator.DetectCmakeAndCompiler() << endl;
}

void createGitIgnore(const string& projectPath)
{
    if (CmakeCreator::CreateGitIgnore(projectPath))
        cout << ".gitignore file created successfully" << endl;
    else
        cout << "Failed to create .gitignore file" << endl;
}

void createReadMe(const string& projectPath)
{
    if (CmakeCreator::CreateReadMe(projectPath))
        cout << "README.md file created successfully" << endl;
    else
        cout << "Failed to create README.md file" << endl;
}

void binaryAnalysis(const string& binaryPath)
{
    if (Analyzer::AnalyzeCreatedBinary(binaryPath))
        cout << "Binary analyzed successfully" << endl;
    else
        cout << "Failed to analyze binary" << endl;
}

void help()
{
    cout << "Usage: " << endl;
    cout << "1 <processName> - Check if process is running" << endl;
    cout << "2 <processPath> - Start process" << endl;
    cout << "3 <processName> - End process" << endl;
    cout << "4 - List running processes" << endl;
    cout << "5 <taskName> <taskPath> <taskTime> - Schedule task" << endl;
    cout << "6 <taskName> - Unschedule task" << endl;
    cout << "7 - List scheduled tasks" << endl;
    cout << "8 <projectName> <projectPath> - Create Cmake file" << endl;
    cout << "9 <projectPath> - Create build directory and build project" << endl;
    cout << "10 - Detect Cmake and compiler" << endl;
    cout << "11 <projectPath> - Create .gitignore file" << endl;
    cout << "12 <projectPath> - Create README.md file" << endl;
    cout << "13 <binaryPath> - Analyze binary" << endl;
    cout << "14 - Exit" << endl;
}