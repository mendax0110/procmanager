#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <iostream>
#include <string>
#include <vector>

namespace TaskSchedulerInternals
{
    class TaskSchedule
    {
    public:
        TaskSchedule();
        ~TaskSchedule();
        static bool ScheduleTask(const std::string& taskPath, const std::string& taskName, const std::string& taskTime);
        static bool UnscheduleTask(const std::string& taskName);
        static std::vector<std::string> GetScheduledTasks();

    private:

    protected:

    };
}

#endif // TASK_SCHEDULER_H