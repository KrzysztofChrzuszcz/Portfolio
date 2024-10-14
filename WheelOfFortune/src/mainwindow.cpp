#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);
    m_OpenGlWidget = std::shared_ptr<CustomOpenGlWidget>(findChild<CustomOpenGlWidget*>("customOpenGlWidget"));
    if (m_OpenGlWidget == nullptr)
        qDebug() << "ERROR: Issue with GUI form. CustmOpenGLWidget is missing!";
    else
        m_OpenGlWidget.get()->bindWithSettings(m_Settings);

    connect(m_Ui->actionChooseFile, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(m_Ui->actionDraw, SIGNAL(triggered()), this, SLOT(play()));
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
    {
        m_Settings.m_FilePath = fileName.toStdString();
        m_Settings.m_DataSelected = true;
        m_Settings.m_DataProcessed = false;
    }
}

void MainWindow::play()
{
    m_Settings.m_DrawLots = true;
}

