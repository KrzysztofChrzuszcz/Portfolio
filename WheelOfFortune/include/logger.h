#ifndef LOGGER_H
#define LOGGER_H

#include "global.h"

#include <fstream>
#include <mutex>
#include <memory>
#include <string>
#include <queue>

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
    virtual                     ~ILogger() = default;
    virtual void                Log(LogLevel level, const std::string& eventClass, const std::string& message) = 0;
};

class AbstractSafeLogger : public ILogger
{
public:
                                AbstractSafeLogger();
    virtual                     ~AbstractSafeLogger();

    void                        Log(LogLevel level, const std::string& eventClass, const std::string& message) override;

protected:
    void                        ProcessQueue();
    virtual void                WriteLog(const std::string& logEntry) = 0;

private:
    std::mutex                  m_Mutex;
    std::queue<std::string>     m_Queue;
    std::condition_variable     m_CV;
    bool                        m_StopLogging = false;
    std::thread                 m_WorkerThread;
};

class AbstractDirectOutputLogger : public ILogger
{
public:

    void                        Log(LogLevel level, const std::string& eventClass, const std::string& message) override;

protected:
    virtual void                WriteLog(LogLevel level, const std::string& logEntry) = 0;

private:
    std::mutex                  m_LogMutex; // TODO: Rozwazyc wspolny poziom posredni dla powyzszych klas
};

class FileLogger : public AbstractSafeLogger
{
    friend class LoggerFactory;
    // TODO: m_Config : wybor sposobu zapisu, reakcja na przepelnienie pliku, moze rozszerzenie
protected:
    explicit                    FileLogger(const std::string& filePath);

public:
                                ~FileLogger();

protected:
    void                        WriteLog(const std::string& logEntry) override;

private:
    std::ofstream               m_File;
};

class ConsoleLogger : public AbstractSafeLogger
{
    friend class LoggerFactory;

protected:
                                ConsoleLogger() = default;

    void                        WriteLog(const std::string& logEntry) override;
};
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
class QDebugLogger : public AbstractDirectOutputLogger
{
    friend class LoggerFactory;

protected:
                                QDebugLogger() = default;

    void                        WriteLog(LogLevel level, const std::string& logEntry) override;
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

    void                        SetType(Type type);

protected:
                                OutputLogger() = default;

    void                        WriteLog(LogLevel level, const std::string& logEntry) override;
    void                        WriteLog(LogLevel level, const std::wstring& logEntry); /// Overloaded OutputLogger::writeLog should be use only with wide output type

private:
    Type                        m_Type = Type::narrow;
};

class LoggerFactory
{
public:
    static std::shared_ptr<ILogger> createFileLogger(const std::string& filePath);
    static std::shared_ptr<ILogger> createConsoleLogger();
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    static std::shared_ptr<ILogger> createQDebugLogger();
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
    static std::shared_ptr<ILogger> createOutputLogger();
};

#endif // LOGGER_H