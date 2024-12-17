#define _CRTDBG_MAP_ALLOC

#include "mainwindow.h"
#include "global.h"
#include "customopenglwidget.h"
#include "fuel.h"
#include "engine.h"
#include "dataloader.h"

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
	Settings settings;
	MainWindow window(settings);

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
	TinyDataLoader dataLoader;
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	QtDataLoader dataLoader;
#endif // !MINIMUM_USAGE_OF_QT_FRAMEWORK

	Engine engine(window, dataLoader); // TODO: Scalic z WorkingThread NOTE: NIE! musi byc podmianka na watek z SLTa.

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
