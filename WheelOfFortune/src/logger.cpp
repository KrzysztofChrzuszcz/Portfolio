#include "logger.h"
#include <cstdio>
#include <iostream>
#include <ctime>
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include <QDebug>
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

inline std::string logLevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Critical:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

std::string currentDateTime()
{
    auto now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void AbstractSafeLogger::log(LogLevel level, const std::string& eventClass, const std::string& message)
{
   
    std::string logEntry = std::string("{") + currentDateTime() + "} [" + logLevelToString(level) + "] (" + eventClass + "): " + message;

    std::lock_guard<std::mutex> lock(m_logMutex);
    writeLog(logEntry);
}

void AbstractDirectOutputLogger::log(LogLevel level, const std::string& eventClass, const std::string& message)
{
    std::string logEntry = std::string("{") + currentDateTime() + "} [" + logLevelToString(level) + "] (" + eventClass + "): " + message;

    std::lock_guard<std::mutex> lock(m_logMutex);
    writeLog(level, logEntry);
}

FileLogger::FileLogger(const std::string& filePath)
{
    m_logFile.open(filePath, std::ios::app);
    if (!m_logFile.is_open())
    {
        throw std::runtime_error("Unable to open log file");
    }
}

FileLogger::~FileLogger()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void FileLogger::writeLog(const std::string& logEntry)
{
    if (m_logFile.is_open())
    {
        m_logFile << logEntry << std::endl;
    }
}

void ConsoleLogger::writeLog(const std::string& logEntry)
{
    std::printf("%s\n", logEntry.c_str());
}
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
void QDebugLogger::writeLog(LogLevel level, const std::string& logEntry)
{
    switch (level)
    {
        case LogLevel::Error:
        case LogLevel::Critical:
            qCritical() << logEntry.c_str();
            break;
        case LogLevel::Info:
            qInfo() << logEntry.c_str();
            break;
        case LogLevel::Warning:
            qWarning() << logEntry.c_str();
            break;
        case LogLevel::Debug:
        default:
            qDebug() << logEntry.c_str();
    }
}
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
void OutputLogger::setType(Type type)
{
    m_Type = type;
}

void OutputLogger::writeLog(LogLevel level, const std::string& logEntry)
{
    if (m_Type == Type::narrow)
    {
        switch (level)
        {
            case LogLevel::Error:
            case LogLevel::Critical:
                std::cerr << logEntry << std::endl;
                break;
            case LogLevel::Info:
            case LogLevel::Warning:
                std::clog << logEntry << std::endl;
                break;
            case LogLevel::Debug:
            default:
                std::cout << logEntry << std::endl;
        }

        return;
    }

    if (m_Type == Type::wide)
        writeLog(level, std::wstring(logEntry.begin(), logEntry.end()));
}

void OutputLogger::writeLog(LogLevel level, const std::wstring& logEntry)
{
    if (m_Type == Type::narrow)
        return; /// Overloaded OutputLogger::writeLog should be use only with wide output type

    if (m_Type == Type::wide)
        switch (level)
        {
            case LogLevel::Error:
            case LogLevel::Critical:
                std::wcerr << logEntry << std::endl;
                break;
            case LogLevel::Info:
            case LogLevel::Warning:
                std::wclog << logEntry << std::endl;
                break;
            case LogLevel::Debug:
            default:
                std::wcout << logEntry << std::endl;
        }
}

/// Protected concrete loggers constructors prevent for using preferred make_shared as creation method in factory.
std::shared_ptr<ILogger> LoggerFactory::createFileLogger(const std::string& filePath)
{
    return std::shared_ptr<ILogger>(new FileLogger(filePath));
}

std::shared_ptr<ILogger> LoggerFactory::createConsoleLogger()
{
    return std::shared_ptr<ILogger>(new ConsoleLogger());
}
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
std::shared_ptr<ILogger> LoggerFactory::createQDebugLogger()
{
    return std::shared_ptr<ILogger>(new QDebugLogger());
}
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
std::shared_ptr<ILogger> LoggerFactory::createOutputLogger()
{
    return std::shared_ptr<ILogger>(new OutputLogger());
}
