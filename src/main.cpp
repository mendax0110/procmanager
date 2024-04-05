#include "./include/ProcessManager.h"
#include "./include/ProcessMonitor.h"
#include <thread>

using namespace std;
using namespace ProcessManagerInternals;
using namespace ProcessMonitorInternals;


int main()
{
    int choice;
    string processName;
    string processPath;

    string menu =   "-------- Process Manager --------\n"
                    "1. Check if a process is running\n"
                    "2. Start a process\n"
                    "3. End a process\n"
                    "4. List all running processes\n"
                    "5. Exit\n";
                    "---------------------------------\n"
                    "Enter your choice:";

    do
    {
        cout << menu;
        cin >> choice;

        switch (choice)
        {
            case 1:
                cout << "Enter the name of the process you want to check:";
                cin >> processName;

                if (ProcessMonitor::IsProcessRunning(processName))
                {
                    cout << "Process is running" << endl;
                }
                else
                {
                    cout << "Process is not running" << endl;
                }
                break;
            case 2:
                cout << "Enter the path of the process you want to start:";
                cin >> processPath;

                if (ProcessManager::StartProcess(processPath))
                {
                    cout << "Process started successfully" << endl;
                }
                else
                {
                    cout << "Failed to start process" << endl;
                }
                break;
            case 3:
                cout << "Enter the name of the process you want to end:";
                cin >> processName;

                if (ProcessManager::EndProcess(processName))
                {
                    cout << "Process ended successfully" << endl;
                }
                else
                {
                    cout << "Failed to end process" << endl;
                }
                break;
            case 4:
                {
					vector<string> processes = ProcessMonitor::GetRunningProcesses();
                    for (const string& process : processes)
                    {
						cout << process << endl;
					}
				}
				break;
            case 5:
                cout << "Exiting...";
                this_thread::sleep_for(chrono::seconds(3));
                exit(1);
                break;
            default:
                cout << "Invalid choice" << endl;
                break;
        }

    } while(choice != 5);

    return 0;
}
