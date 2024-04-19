#include "./include/Logger.h"

using namespace std;
using namespace LoggerInternals;

Logger::Logger(const string& logFilePath) : logFile(logFilePath, ios::app)
{
    if (!logFile.is_open())
    {
        cerr << "Error: Cannot open log file: " << logFilePath << endl;
    }
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void Logger::log(LogLevel level, const string& message)
{
    lock_guard<mutex> lock(mutexLogger);

    string currentTime = getCurrentTime();
    string levelStr;

    switch (level)
    {
    case LogLevel::INFO:
        levelStr = "[INFO]";
        break;
    case LogLevel::WARNING:
        levelStr = "[WARNING]";
        break;
    case LogLevel::ERROR:
        levelStr = "[ERROR]";
        break;
    case LogLevel::DEBUG:
        levelStr = "[DEBUG]";
        break;
    case LogLevel::TRACE:
        levelStr = "[TRACE]";
        break;
    }

    logFile << currentTime << " " << levelStr << " " << message << endl;
}

string Logger::getCurrentTime()
{
    auto now = chrono::system_clock::now();
    time_t nowTime = chrono::system_clock::to_time_t(now);
    string currentTime = ctime(&nowTime);
    currentTime.pop_back();

    return currentTime;
}
