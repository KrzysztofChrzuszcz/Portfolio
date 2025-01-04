#ifndef VERTICALBAR_H
#define VERTICALBAR_H

#include "abcontrol.h"
#include <QPainter>

class VerticalBar : public AbstractControl
{
public:
    VerticalBar(QQuickItem* parent = 0);
    void paint(QPainter* painter);
};

#endif // VERTICALBAR_H