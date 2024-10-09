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
    inline Settings&                        GetSettings() { return m_Settings; } /// Getter defined in the header to inline it in order to reduce the overhead associated with function calls, such as saving and restoring registers, stack management
    std::weak_ptr<CustomOpenGlWidget>       GetWidget() { return std::weak_ptr<CustomOpenGlWidget>(m_OpenGlWidget); }

public slots:
    void                                    openFileBrowser();
    void                                    play();
    //    void                              openSettings();

private:
    Ui::MainWindow*                         m_Ui;
    Settings                                m_Settings;
    std::shared_ptr<CustomOpenGlWidget>     m_OpenGlWidget;
    // TODO: dodac wersje jezykowe, BEZ wyboru. Kompilacja wersji Polskiej i Angielskiej. np. poprzez wybor innego mainwindow.ui

};

#endif // MAINWINDOW_H
