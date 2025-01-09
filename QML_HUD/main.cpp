#include "utilities.h"
#include "gauge.h"
#include "vbar.h"
#include "hboard.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> 
#include <QObject>
#include <QDebug>

// TODO: Umiescic w QMLach wyswietlacz wartosci i jednostki
// TODO: Mirror
// TODO: Poprawic rysowanie niektorych kontrolerow
// TODO: Uwspolnic zmienne jak odleglosc od krawedzi
// TODO: Zrefaktoryzowac wzory na rysowanie: kat w vBar, gauge ..., hBoard ...
// TODO: uzyc shiftow i scale z QMLa

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<ThemeProvider>("Themes", 1, 0, "ThemeProvider");
    qmlRegisterType<HorizontalBoard>("CustomControls", 1, 0, "HorizontalBoard");
    qmlRegisterType<VerticalBar>("CustomControls", 1, 0, "VerticalBar");
    qmlRegisterType<Gauge>("CustomControls", 1, 0, "Gauge");

    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

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