#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <mutex>


namespace LoggerInternals
{
    enum class LogLevel
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG,
        TRACE
    };
    
    class Logger
    {
    public:
        Logger(const std::string& logFilePath);
        ~Logger();

        void log(LogLevel level, const std::string& message);

    private:
        std::ofstream logFile;
        std::string getCurrentTime();

    protected:
        std::mutex mutexLogger;

    };
}

#endif // LOGGER_H