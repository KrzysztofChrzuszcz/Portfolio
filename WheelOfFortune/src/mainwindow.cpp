#include "mainwindow.h"
#include "settingswidget.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(SettingsManager& settingsManager, std::shared_ptr<ILogger> logger, QWidget *parent) :
    QMainWindow(parent),
    m_Logger(std::move(logger)),
    m_Ui(new Ui::MainWindow),
    m_SettingsManager(settingsManager)
{
    m_Ui->setupUi(this);
    if (CustomOpenGlWidget* customOpenGlWidget = findChild<CustomOpenGlWidget*>("customOpenGlWidget"))
        m_OpenGlWidget = std::shared_ptr<CustomOpenGlWidget>(customOpenGlWidget);

    if (m_OpenGlWidget == nullptr)
    {
        m_Logger->Log(LogLevel::Error, "MainWindow", "Issue with GUI form. CustmOpenGLWidget is missing!");
    }
    else
        m_OpenGlWidget.get()->bindWithSettings(m_SettingsManager);

    connect(m_Ui->actionChooseFile, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(m_Ui->actionDraw, SIGNAL(triggered()), this, SLOT(drawLots()));
    connect(m_Ui->displaySettings, SIGNAL(triggered()), this, SLOT(displaySettings()));
    connect(this, SIGNAL(errorSignal(const QString&)), this, SLOT(displayErrorWindow(const QString&)));

    m_SettingsTimestamp = m_SettingsManager.getTimestamp();
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
    m_Logger->Log(LogLevel::Warning, "MainWindow", "Selected file has corrupted data.");

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

void MainWindow::onSettingsClosed(QObject* obj)
{
    if (m_SettingsManager.hasChanged(m_SettingsTimestamp))
    {
        m_SettingsManager.dump();
        m_Logger->Log(LogLevel::Info, "MainWindow", "Settings have changed");
    }
}

void MainWindow::openFileBrowser()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "RelWithDebInfo/resources/data", tr("XML Files (*.xml)"));
    m_SettingsManager.setFilePath(fileName.toStdString());
    m_Logger->Log(LogLevel::Info, "MainWindow", "Opened file: " + fileName.toStdString());
}

void MainWindow::drawLots()
{
    m_SettingsManager.setDrawLots();
}

void MainWindow::displaySettings()
{
    m_SettingsTimestamp = m_SettingsManager.getTimestamp();
    SettingsWidget* popup = new SettingsWidget(m_SettingsManager.getSettings(), this);
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowFlags(Qt::Popup); // https://forum.qt.io/topic/115599/qt-popup-window-behaviour/10
    connect(popup, &SettingsWidget::destroyed, this, &MainWindow::onSettingsClosed, Qt::DirectConnection);
    popup->move(this->geometry().topLeft());
    popup->show();
}
