#include "mainwindow.h"
#include "settingswidget.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(Settings& settings, QWidget *parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow),
    m_Settings(settings)
{
    m_Ui->setupUi(this);
    m_OpenGlWidget = std::shared_ptr<CustomOpenGlWidget>(findChild<CustomOpenGlWidget*>("customOpenGlWidget"));
    if (m_OpenGlWidget == nullptr)
        qDebug() << "ERROR: Issue with GUI form. CustmOpenGLWidget is missing!";
    else
        m_OpenGlWidget.get()->bindWithSettings(settings);

    connect(m_Ui->actionChooseFile, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(m_Ui->actionDraw, SIGNAL(triggered()), this, SLOT(drawLots()));
    connect(m_Ui->displaySettings, SIGNAL(triggered()), this, SLOT(displaySettings()));
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

void MainWindow::openFileBrowser()
{
    // TODO: Add predirectives for Debug/ Release for default path 
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "RelWithDebInfo/resources/data", tr("XML Files (*.xml)"));
    if (!fileName.isEmpty())
        m_Settings.setFilePath(fileName.toStdString());
}

void MainWindow::drawLots()
{
    m_Settings.drawLots();
}

void MainWindow::displaySettings()
{
    SettingsWidget* popup = new SettingsWidget(m_Settings, this);
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowFlags(Qt::Popup); // https://forum.qt.io/topic/115599/qt-popup-window-behaviour/10
    popup->move(this->geometry().topLeft());
    popup->show();
}
