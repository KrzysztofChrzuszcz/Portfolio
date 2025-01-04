#ifndef GAUGE_H
#define GAUGE_H

#include "abcontrol.h"
#include <QPainter>

class Gauge : public AbstractControl
{
public:
    Gauge(QQuickItem* parent = 0);
    void paint(QPainter* painter);
};

#endif // GAUGE_H