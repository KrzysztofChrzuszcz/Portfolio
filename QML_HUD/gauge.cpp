#include "gauge.h"

Gauge::Gauge(QQuickItem* parent)
    : AbstractControl(parent)
{
}

void Gauge::paint(QPainter* painter)
{
    qreal m_MinValue = 0;
    qreal m_MaxValue = 360;
    qreal spanAngle = -360;
    qreal offset = 5;
    qreal startAngle = -90;

    //------------------------------------------------
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setRenderHints(QPainter::Antialiasing, true);

    painter->setBrush(m_NonActiveColor);
    painter->drawEllipse(boundingRect());
    painter->restore();


    QPen pen = painter->pen();
    painter->setBrush(Qt::NoBrush);
    pen.setCapStyle(Qt::FlatCap);
    painter->save();
    pen.setWidth(10);
    pen.setColor(m_ActiveColor);
    qreal valueAngle = (((360.0 * m_Value) - m_MinValue) / (m_MaxValue - m_MinValue)) * spanAngle;  //Map value to angle range
    painter->setPen(pen);
    QRectF rect = this->boundingRect();
    rect.adjust(offset, offset, -offset, -offset);
    painter->drawArc(rect, startAngle * 16, valueAngle * 16);
    painter->restore();
}
