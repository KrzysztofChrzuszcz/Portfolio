#ifndef LOGGER_H
#define LOGGER_H

#include "global.h"

#include <fstream>
#include <mutex>
#include <memory>
#include <string>

enum class LogLevel
{
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4
};

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void log(LogLevel level, const std::string& eventClass, const std::string& message) = 0;
};

class AbstractSafeLogger : public ILogger
{
public:
    void log(LogLevel level, const std::string& eventClass, const std::string& message) override;

protected:
    virtual void writeLog(const std::string& logEntry) = 0;

private:
    std::mutex m_logMutex;
};

class AbstractDirectOutputLogger : public ILogger
{
public:

    void log(LogLevel level, const std::string& eventClass, const std::string& message) override;

protected:
    virtual void writeLog(LogLevel level, const std::string& logEntry) = 0;

private:
    std::mutex m_logMutex;
};

class FileLogger : public AbstractSafeLogger
{
    friend class LoggerFactory;

protected:
    explicit FileLogger(const std::string& filePath);

public:
    ~FileLogger();

protected:
    void writeLog(const std::string& logEntry) override;

private:
    std::ofstream m_logFile;
};

class ConsoleLogger : public AbstractSafeLogger
{
    friend class LoggerFactory;

protected:
    ConsoleLogger() = default;

    void writeLog(const std::string& logEntry) override;
};
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
class QDebugLogger : public AbstractDirectOutputLogger
{
    friend class LoggerFactory;

protected:
    QDebugLogger() = default;

    void writeLog(LogLevel level, const std::string& logEntry) override;
};
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
class OutputLogger : public AbstractDirectOutputLogger
{
    friend class LoggerFactory;
public:
    enum class Type
    {
        narrow = 0,
        wide = 1
    };

    void setType(Type type);

protected:
    OutputLogger() = default;

    void writeLog(LogLevel level, const std::string& logEntry) override;
    void writeLog(LogLevel level, const std::wstring& logEntry); /// Overloaded OutputLogger::writeLog should be use only with wide output type

private:
    Type m_Type = Type::narrow;
};

class LoggerFactory
{
public:
    static std::shared_ptr<ILogger> createFileLogger(const std::string& filePath);
    static std::shared_ptr<ILogger> createConsoleLogger();
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    static std::shared_ptr<ILogger> createQDebugLogger(); // TODO: makro na odciecie od Qt
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
    static std::shared_ptr<ILogger> createOutputLogger();
};

#endif // LOGGER_H