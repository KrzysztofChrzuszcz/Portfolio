#include "fuel.h"

#include <cmath>

const uint SCREEN_REFRESH_FREQUENCY(50); // [Hz]

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

Fuel::Fuel(Engine& engine) :
    m_Engine(engine),
    m_Quit(false)
{
    m_Delay = 1000 / m_Engine.getScreenRefreshFrequency();
}

Fuel::~Fuel()
{
}

void Fuel::updateScreenRefreshing()
{
    m_Engine.updateScreenRefreshingParameters(m_Delay);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CustomWorkingThreadNewWay::CustomWorkingThreadNewWay(Engine& engine) :
    Fuel(engine)
{
}

CustomWorkingThreadNewWay::~CustomWorkingThreadNewWay()
{
    m_Quit = true;
    if (m_Future.valid())
    {
        m_Future.wait();
    }
}

void CustomWorkingThreadNewWay::startEngine()
{
    m_Future = std::async(&CustomWorkingThreadNewWay::run, this);
}

void CustomWorkingThreadNewWay::run()
{
    while (!m_Quit)
    {
        updateScreenRefreshing();
        m_Engine.run();
        std::this_thread::sleep_for(std::chrono::milliseconds(std::lround(m_Delay)));
    }

    // https://stackoverflow.com/questions/22653022/how-to-terminate-a-stdfuture
    // stop_token | stop_source from C++20
    // TODO: Implement in C++ 20 style (Update version in CMake up to Cpp20)
    /*
    std::stop_source stopSource;
    std::stop_token stopToken = stopSource.get_token();

    m_Future = std::async(std::launch::async, [&]() {
        while (!stopToken.stop_requested())
        {
            updateScreenRefreshing();
            m_Engine.run();
            std::this_thread::sleep_for(milliseconds(m_Delay));
        }
    });

    // Stop thread
    stopSource.request_stop();
    */
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CustomWorkingThreadOldWay::CustomWorkingThreadOldWay(Engine& engine) :
    Fuel(engine)
{
}

CustomWorkingThreadOldWay::~CustomWorkingThreadOldWay()
{
    m_Quit = true;
}

void CustomWorkingThreadOldWay::startEngine()
{
    std::thread thread(&CustomWorkingThreadOldWay::run, this);
    thread.detach();
}

void CustomWorkingThreadOldWay::run()
{
    while (!m_Quit)
    {
        updateScreenRefreshing();
        m_Engine.run();
        sleep_for(milliseconds(std::lround(m_Delay)));
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QtWorkingThread::QtWorkingThread(Engine& engine)
    : QThread(nullptr), Fuel(engine)
{
}

QtWorkingThread::QtWorkingThread(QObject* parent, Engine& engine)
    : QThread(parent), Fuel(engine)
{
}

QtWorkingThread::~QtWorkingThread()
{
    m_Quit = true;
    quit();
    wait();
}

void QtWorkingThread::startEngine()
{
    if (!isRunning())
        start();
}

void QtWorkingThread::run()
{
    while (!m_Quit)
    {
        updateScreenRefreshing();
        m_Engine.run();
        Sleep(m_Delay);
    }
}

