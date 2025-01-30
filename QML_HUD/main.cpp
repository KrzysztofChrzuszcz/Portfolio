#include "utilities.h"
#include "gauge.h"
#include "vbar.h"
#include "hboard.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> 
#include <QObject>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QVariantMap>
#include <QWindow>

// X TODO: Umiescic w QMLach wyswietlacz wartosci i jednostki
// TODO: Poprawic rysowanie niektorych kontrolerow
// TODO: Uwspolnic zmienne jak odleglosc od krawedzi
// +/-X TODO: Zrefaktoryzowac wzory na rysowanie: kat w vBar, gauge ..., hBoard ...

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Register custom types
    qmlRegisterType<ThemeProvider>("Themes", 1, 0, "ThemeProvider");
    qmlRegisterType<Gauge>("CustomControls", 1, 0, "Gauge");
    qmlRegisterType<HorizontalBoard>("CustomControls", 1, 0, "HorizontalBoard");
    qRegisterMetaType<HorizontalBoard::DialType>("HorizontalBoard::DialType");
    qmlRegisterType<VerticalBar>("CustomControls", 1, 0, "VerticalBar");

    // Backend
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    // Path to the local QML file
    QString mainQmlFile = QStringLiteral("%1/../../main.qml").arg(QCoreApplication::applicationDirPath()); // TODO: fix path before release

    // File change monitoring
    QFileSystemWatcher fileWatcher;
    fileWatcher.addPath(mainQmlFile);

    // Variables for storing window position and size
    QRect windowGeometry;

    // Function to reload QML
    auto reloadQml = [&engine, &backend, &mainQmlFile, &windowGeometry]() {
        qDebug() << "Reloading QML file: " << mainQmlFile;

        // Save current window position and size
        if (!engine.rootObjects().isEmpty())
        {
            QObject* rootObject = engine.rootObjects().first();
            if (rootObject)
            {
                QWindow* window = qobject_cast<QWindow*>(rootObject);
                if (window)
                {
                    windowGeometry = window->geometry(); // Save window geometry
                    qDebug() << "Window geometry saved:" << windowGeometry;
                }
            }
        }

        // Clear QML component cache
        engine.clearComponentCache();

        // Check if a rootObject already exists
        if (!engine.rootObjects().isEmpty())
        {
            qDebug() << "Existing rootObjects found, deleting old rootObject.";
            // Get the first rootObject (main window)
            QObject* rootObject = engine.rootObjects().first();
            // Delete rootObject
            rootObject->deleteLater();
        }

        // Reload QML
        engine.load(QUrl::fromLocalFile(mainQmlFile));
        };

    // Connect `fileChanged` signal to QML reload function
    QObject::connect(&fileWatcher, &QFileSystemWatcher::fileChanged, [&fileWatcher, reloadQml](const QString& path) {
        qDebug() << "File changed: " << path;
        fileWatcher.addPath(path);
        reloadQml();
        });

    // Connect `objectCreated` signal to restore window geometry
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&windowGeometry](QObject* object, const QUrl& url) {
        if (object)
        {
            QWindow* window = qobject_cast<QWindow*>(object);
            if (window)
            {
                // Check if windowGeometry is valid (non-zero width and height)
                if (windowGeometry.width() > 0 && windowGeometry.height() > 0) {
                    // Restore window position and size
                    window->setGeometry(windowGeometry);
                    qDebug() << "Window geometry restored to:" << windowGeometry;
                }
                else
                {
                    qDebug() << "Skipping geometry restoration: invalid window geometry.";
                }
            }
        }
        });

    // Load QML for the first time
    engine.load(QUrl::fromLocalFile(mainQmlFile));

    // Check if the main QML object was loaded
    if (engine.rootObjects().isEmpty())
    {
        qCritical() << "Failed to load initial QML file.";
        return -1;
    }

    return app.exec();
}

#include "main.moc"