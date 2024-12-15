#include "customopenglwidget.h"

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "glwheeloffortune.h"
#include <GL/glut.h>
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
#include "qtwheeloffortune.h"
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

#include <functional>

#include <QCoreApplication>
#include <QOpenGLFunctions>


CustomOpenGlWidget::CustomOpenGlWidget(QWidget* parent):
    QOpenGLWidget(parent),
    m_Painter(this)
{
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    m_WhelOfFortune = std::unique_ptr<WheelOfFortune>(new QtWheelOfFortune(m_Painter));
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
    m_WhelOfFortune = std::unique_ptr<WheelOfFortune>(new GlWheelOfFortune());
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

    QSizePolicy qsp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    setSizePolicy(qsp);
}

CustomOpenGlWidget::~CustomOpenGlWidget()
{
}

void CustomOpenGlWidget::paintGL()
{
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    m_Painter.begin(this);
    if (m_Painter.isActive())
    {
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
        m_WhelOfFortune->paintGL(WheelOfFortune::Size({ this->size().width(), this->size().height() }));
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    }
    m_Painter.end();
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
}

void CustomOpenGlWidget::bindWithSettings(const Settings& settings)
{
    m_WhelOfFortune.get()->isDataReady = std::bind(&Settings::isDataReady, &settings);
}

void CustomOpenGlWidget::initializeGL()
{
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    QOpenGLContext* context = QOpenGLWidget::context();
    QOpenGLFunctions* functions = context->functions();
    functions->glClearColor(0.94f, 0.94f, 0.94f, 1.0f);
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK

#ifdef MINIMUM_USAGE_OF_QT_FRAMEWORK
    int argc = 1;
    char* argv[1] = { (char*)"blind stopper" }; // C style cast as simple example
    glutInit(&argc, argv);
    glClearColor(0.94f, 0.94f, 0.94f, 1.0f);
#endif // MINIMUM_USAGE_OF_QT_FRAMEWORK
}

void CustomOpenGlWidget::resizeGL(int w, int h)
{
    int side = std::min(w, h);
    glViewport(0, 0, (GLint)side, (GLint)side);

    resize(side, side);
    move(
        w > h ? (std::max(w, h) - side) / 2 : 0,
        h > w ? (std::max(w, h) - side) / 2 : 0
        );
}