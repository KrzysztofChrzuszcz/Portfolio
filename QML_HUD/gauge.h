#ifndef RADIALBAR_H
#define RADIALBAR_H

#include "abcontrol.h"
#include <QPainter>

class Gauge : public AbstractControl
{
public:
    Gauge(QQuickItem* parent = 0);
    void paint(QPainter* painter);
};

#endif // RADIALBAR_H