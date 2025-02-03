#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "settings.h"
#include "customopenglwidget.h"
#include "logger.h"

#include <memory>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit                                MainWindow(Settings& settings, std::shared_ptr<ILogger> logger, QWidget *parent = nullptr);
                                            ~MainWindow();

    inline Settings&                        getSettings() { return m_Settings; }            //!< Getter defined in the header to inline it in order to reduce the overhead associated with function calls, such as saving and restoring registers, stack management
    std::weak_ptr<CustomOpenGlWidget>       getWidget();                                    //!< OpenGl widget getter
    void                                    alarmLoadingDataError(const bitset<4>& flags);  //!< Emits error signal for data loading known issues

signals:
    void                                    errorSignal(const QString& message);            //!< Signal that initiates warning error window

public slots:
    void                                    logSettingsChange(QObject* obj);                // TODO
private slots:
    void                                    openFileBrowser();                              //!< Open a file browser, saves path to selected file and initiate loading the input file
    void                                    drawLots();                                     //!< Start a fortune draw
    void                                    displaySettings();                              //!< Displays settings control widget
    void                                    displayErrorWindow(const QString& message);     //!< Displays Warning Window inform about error with given message

private:
    Ui::MainWindow*                         m_Ui;
    Settings&                               m_Settings;
    std::time_t                             m_SettingsTimestamp;
    std::shared_ptr<CustomOpenGlWidget>     m_OpenGlWidget;
    std::shared_ptr<ILogger>                m_Logger;
    // TODO: Add precompiled language versions
    // dodac wersje jezykowe, BEZ wyboru. Kompilacja wersji Polskiej i Angielskiej. np. poprzez wybor innego mainwindow.ui

};

#endif // MAINWINDOW_H
