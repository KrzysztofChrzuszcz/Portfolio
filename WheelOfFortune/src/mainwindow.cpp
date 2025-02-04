#include "mainwindow.h"
#include "settingswidget.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(Settings& settings, std::shared_ptr<ILogger> logger, QWidget *parent) :
    QMainWindow(parent),
    m_Logger(std::move(logger)),
    m_Ui(new Ui::MainWindow),
    m_Settings(settings)
{
    m_Ui->setupUi(this);
    m_OpenGlWidget = std::shared_ptr<CustomOpenGlWidget>(findChild<CustomOpenGlWidget*>("customOpenGlWidget"));
    if (m_OpenGlWidget == nullptr)
    {
        m_Logger->log(LogLevel::Error, "MainWindow", "Issue with GUI form. CustmOpenGLWidget is missing!");
    }
    else
        m_OpenGlWidget.get()->bindWithSettings(settings);

    connect(m_Ui->actionChooseFile, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(m_Ui->actionDraw, SIGNAL(triggered()), this, SLOT(drawLots()));
    connect(m_Ui->displaySettings, SIGNAL(triggered()), this, SLOT(displaySettings()));
    connect(this, SIGNAL(errorSignal(const QString&)), this, SLOT(displayErrorWindow(const QString&)));

    m_SettingsTimestamp = m_Settings.getTimestamp();
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
    m_Logger->log(LogLevel::Warning, "MainWindow", "Selected file has corrupted data.");

    if (flags[0])
        emit errorSignal(g_WrongInputMsg.c_str());
    if (flags[1])
        emit errorSignal(g_ChannelOutOfRangeMsg.c_str());
    if (flags[2])
        emit errorSignal(g_WrongChannelAmountMsg.c_str());
    if (flags[3])
        emit errorSignal(g_WrongPieSettingsMsg.c_str());
    if (!flags.any())
        emit errorSignal("Unknown problem with selected file");
}

void MainWindow::displayErrorWindow(const QString& message)
{
    QMessageBox::critical(this, tr("Error Window"), message);
}

void MainWindow::logSettingsChange(QObject* obj)
{
    if (m_Settings.hasChanged(m_SettingsTimestamp))
        m_Logger->log(LogLevel::Info, "MainWindow", "Settings have changed");
}

void MainWindow::openFileBrowser()
{
    // TODO: Add predirectives for Debug/ Release for default path 
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "RelWithDebInfo/resources/data", tr("XML Files (*.xml)"));
    m_Settings.setFilePath(fileName.toStdString());
    m_Logger->log(LogLevel::Info, "MainWindow", "Opened file: " + fileName.toStdString());
}

void MainWindow::drawLots()
{
    m_Settings.setDrawLots();
}

void MainWindow::displaySettings()
{
    m_SettingsTimestamp = m_Settings.getTimestamp();
    SettingsWidget* popup = new SettingsWidget(m_Settings, this);
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowFlags(Qt::Popup); // https://forum.qt.io/topic/115599/qt-popup-window-behaviour/10
    connect(popup, &SettingsWidget::destroyed, this, &MainWindow::logSettingsChange, Qt::DirectConnection);
    popup->move(this->geometry().topLeft());
    popup->show();
}
