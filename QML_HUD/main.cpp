#include "gauge.h"
#include "vbar.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> 
#include <QObject>
#include <QDebug>
#include <iostream>
using namespace std;

// Klasa backendu
class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void printSth()
    {
        cout << "Screen mirror toggle" << endl;
    }
};

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<VerticalBar>("CustomControls", 1, 0, "VerticalBar");
    qmlRegisterType<Gauge>("CustomControls", 1, 0, "Gauge");

    QQmlApplicationEngine engine;

    // Rejestracja instancji klasy Backend
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    // TODO: Konfiguracja

    // Ładowanie pliku QML
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#include "main.moc"