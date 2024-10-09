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
        m_OpenGlWidget.get()->bindWithSettings(m_Settings); // TODO: wskaznik na funkcje lub bind jak boosta

    connect(m_Ui->actionWybierz_warianty, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(m_Ui->actionLosuj, SIGNAL(triggered()), this, SLOT(play()));
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

void MainWindow::openFileBrowser()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "RelWithDebInfo/resources/data", tr("XML Files (*.xml)")); //TODO: Ustawic makro na tryb debugu/release i zrobic sciezke dla release
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

