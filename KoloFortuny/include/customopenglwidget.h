#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include "global.h"
#include "settings.h"
#include "wheeloffortune.h"

#include <memory>

#include <QOpenGLWidget>
#include <QGLWidget>

class CustomOpenGlWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
                                    CustomOpenGlWidget(QWidget* parent = 0);
                                    ~CustomOpenGlWidget();

    inline WheelOfFortune*          getGui() { return &*m_WhelOfFortune; }
    // inline WheelOfFortune*       getGui() { return m_WhelOfFortune.get(); }
    void                            paintGL() override;
    void                            bindWithSettings(const Settings& conf);


protected:
    void                            initializeGL() override;
    void                            resizeGL(int w, int h) override;

private:
    QPainter                        m_Painter;
    std::unique_ptr<WheelOfFortune> m_WhelOfFortune;
};

#endif //CUSTOMOPENGLWIDGET_H