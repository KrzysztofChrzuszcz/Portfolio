#define _CRTDBG_MAP_ALLOC

#include "mainwindow.h"
#include "global.h"
#include "customopenglwidget.h"
#include "fuel.h"
#include "engine.h"
#include "dataloader.h"
#include "logger.h"

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "tinydataloader.h"
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "qtdataloader.h"
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

#include <stdlib.h>
#include <crtdbg.h>
#include <vector>

#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

#ifdef DEBUG
#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	std::shared_ptr<ILogger> fileLogger = LoggerFactory::createOutputLogger();
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	std::shared_ptr<ILogger> fileLogger = LoggerFactory::createQDebugLogger();
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK
#endif // DEBUG
#ifndef DEBUG
	std::shared_ptr<ILogger> fileLogger = LoggerFactory::createFileLogger("log.txt");
#endif // !DEBUG
	Settings settings;
	MainWindow window(settings, fileLogger); // fileLogger

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	TinyDataLoader dataLoader(fileLogger); // fileLogger
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	QtDataLoader dataLoader(fileLogger); // fileLogger
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

	Engine engine(window, dataLoader);

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	// The new one doesn't stop yet.
	// std::unique_ptr<Fuel> threadSmrtPtr(new CustomWorkingThreadNewWay(engine)); // New way
	// TODO: create a task to stop in new way std::async. Maybe stop_token from C++20
	 std::unique_ptr<Fuel> threadSmrtPtr(new CustomWorkingThreadOldWay(engine)); // Old fashion way
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	 std::unique_ptr<Fuel> threadSmrtPtr(new QtWorkingThread(engine));
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

	threadSmrtPtr->startEngine();
	QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
	qsp.setHeightForWidth(true);
	window.setSizePolicy(qsp);
	window.show();
	return app.exec();
}
