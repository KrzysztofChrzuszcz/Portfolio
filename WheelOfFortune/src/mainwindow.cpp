#include "mainwindow.h"
#include "settingswidget.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

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
    connect(this, SIGNAL(errorSignal(const QString&)), this, SLOT(displayErrorWindow(const QString&)));
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

std::weak_ptr<CustomOpenGlWidget> MainWindow::getWidget()
{
    return std::weak_ptr<CustomOpenGlWidget>(m_OpenGlWidget);
}

void MainWindow::alarmLoadingDataError(const bitset<4>& flags)
{
    /// Unrecognized input type | Channel value is out of range | Channels amount is incorrect | Amount of entries is not possible to visualize within given settings
    if (flags[0])
        emit errorSignal("Unrecognized input type");
    if (flags[1])
        emit errorSignal("Channel value is out of range");
    if (flags[2])
        emit errorSignal("Channels amount is incorrect");
    if (flags[3])
        emit errorSignal("Amount of entries is not possible to visualize within given settings");
    if (!flags.any())
        emit errorSignal("Unknown problem with selected file");

    // TODO: Problem z ustalonymi katami !! 
}

void MainWindow::displayErrorWindow(const QString& message)
{
    QMessageBox::critical(this, tr("Error Window"), message);
}

void MainWindow::openFileBrowser()
{
    // TODO: Add predirectives for Debug/ Release for default path 
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "RelWithDebInfo/resources/data", tr("XML Files (*.xml)"));
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
