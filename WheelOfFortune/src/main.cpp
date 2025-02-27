#define _CRTDBG_MAP_ALLOC

#include "global.h"
#include "mainwindow.h"
#include "fuel.h"
#include "engine.h"
#include "dataloader.h"
#include "logger.h"
#include "settingsmanager.h"

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "tinydataloader.h"
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "qtdataloader.h"
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

#if defined(DEBUG) && defined(MINIMUM_USAGE_OF_QT_FRAMEWORK)
	std::shared_ptr<ILogger> logger = LoggerFactory::createOutputLogger();
#endif // DEBUG && MINIMUM_USAGE_OF_QT_FRAMEWORK
#if defined(DEBUG) && !defined(MINIMUM_USAGE_OF_QT_FRAMEWORK)
	std::shared_ptr<ILogger> logger = LoggerFactory::createQDebugLogger();
#endif //  DEBUG && !MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef DEBUG
	std::shared_ptr<ILogger> logger = LoggerFactory::createFileLogger("log.txt");
#endif // !DEBUG

	SettingsManager settingsManager(logger);
	settingsManager.load();
	MainWindow window(settingsManager, logger);

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	TinyDataLoader dataLoader(logger);
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	QtDataLoader dataLoader(logger);
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

	Engine engine(window, dataLoader);

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	std::unique_ptr<Fuel> threadSmrtPtr(new CustomWorkingThreadNewWay(engine)); // New way
	// TODO: create a task to stop in new way std::async. Maybe stop_token from C++20
	// std::unique_ptr<Fuel> threadSmrtPtr(new CustomWorkingThreadOldWay(engine)); // Old fashion way
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
