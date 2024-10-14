#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "settings.h"

#include <memory>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit                                MainWindow(QWidget *parent = nullptr);
                                            ~MainWindow();

    inline Settings&                        GetSettings() { return m_Settings; } //!< Getter defined in the header to inline it in order to reduce the overhead associated with function calls, such as saving and restoring registers, stack management
    std::weak_ptr<CustomOpenGlWidget>       GetWidget() { return std::weak_ptr<CustomOpenGlWidget>(m_OpenGlWidget); } //!< OpenGl widget getter

public slots:
    void                                    openFileBrowser();  //!< Open a file browser, saves path to selected file and initiate loading the input file
    void                                    play();             //!< Start a fortune draw
    //    void                              openSettings();     //!< Open GUI allowing to configure some settings

private:
    Ui::MainWindow*                         m_Ui;
    Settings                                m_Settings;
    std::shared_ptr<CustomOpenGlWidget>     m_OpenGlWidget;
    // TODO: Add precompiled language versions
    // dodac wersje jezykowe, BEZ wyboru. Kompilacja wersji Polskiej i Angielskiej. np. poprzez wybor innego mainwindow.ui

};

#endif // MAINWINDOW_H