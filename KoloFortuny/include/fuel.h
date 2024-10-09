﻿#ifndef FUEL_H
#define FUEL_H

#include "engine.h"

#include <future>
#include <chrono>

#include <QThread.h>
#include <QMutex.h>
#include <QWaitCondition.h>

// In case of this particular group of classes, due to name convention incoherence, I have decided to keep them together in one place.

/// Interface for thread running engine.
// The class name is an easter egg(joke), just for fun.
class Fuel
{
public:
						Fuel(Engine& engine);
	virtual				~Fuel() = 0; /// Derived classes has to setup m_Quit to true;

	virtual void		startEngine() = 0;

public:
	std::atomic<bool>	m_Quit;

protected:
	Engine&				m_Engine;
	int					m_Delay;
};


class CustomWorkingThreadNewWay : public Fuel
{
public:
						CustomWorkingThreadNewWay(Engine& engine);
						~CustomWorkingThreadNewWay();

	void				startEngine() override;

protected:
	void				run();

private:
	std::future<void>	m_Future;
};


class CustomWorkingThreadOldWay : public Fuel
{
public:
						CustomWorkingThreadOldWay(Engine& engine);
						~CustomWorkingThreadOldWay();

	void				startEngine() override;

protected:
	void				runEngine();
};


class QtWorkingThread : public QThread, public Fuel
{
	Q_OBJECT

public:
						QtWorkingThread(Engine& engine);
						QtWorkingThread(QObject* parent, Engine& engine);
	virtual				~QtWorkingThread();

	void				startEngine() override;

protected:
	void				run() override;

//signals:
//	void				error(int socketError, const QString& message); // TODO: dokonczyc
};

#endif //FUEL_H