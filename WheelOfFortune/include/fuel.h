#ifndef FUEL_H
#define FUEL_H

#include "engine.h"

#include <future>
#include <chrono>

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

// In case of this particular group of classes, due to name convention incoherence, I have decided to keep them together in one place.


// The class name is an easter egg(joke), just for fun.
/**
 * \brief Interface for thread running engine.
 */
class Fuel
{
public:
                        Fuel(Engine& engine);
    virtual             ~Fuel(); /// Derived classes has to set m_Quit to true;

    virtual void        startEngine() = 0;
    void                updateScreenRefreshing();

protected:
    std::atomic<bool>   m_Quit;
    Engine&             m_Engine;
    double              m_Delay;
};


class CustomWorkingThreadNewWay : public Fuel
{
public:
                        CustomWorkingThreadNewWay(Engine& engine);
                        ~CustomWorkingThreadNewWay();

    void                startEngine() override;

protected:
    void                run();

private:
    std::future<void>   m_Future;
};


class CustomWorkingThreadOldWay : public Fuel
{
public:
                        CustomWorkingThreadOldWay(Engine& engine);
                        ~CustomWorkingThreadOldWay();

    void                startEngine() override;

protected:
    void                runEngine();
};


class QtWorkingThread : public QThread, public Fuel
{
    Q_OBJECT

public:
                        QtWorkingThread(Engine& engine);
                        QtWorkingThread(QObject* parent, Engine& engine);
    virtual             ~QtWorkingThread();

    void                startEngine() override;

protected:
    void                run() override;
};

#endif //FUEL_H