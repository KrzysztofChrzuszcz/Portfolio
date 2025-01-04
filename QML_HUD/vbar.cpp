#include "vbar.h"

VerticalBar::VerticalBar(QQuickItem* parent)
    : AbstractControl(parent)
{
}

void VerticalBar::paint(QPainter* painter)
{
    // TODO: angle instead factor <0;45> : default = 15
    static const QPointF backgroundPoints[4] = {
        QPointF(m_Size / 4.0 - m_Size * 0.2, 0),
        QPointF(3.0 * m_Size / 4.0 + m_Size * 0.2, 0),
        QPointF(3.0 * m_Size / 4.0, m_Size),
        QPointF(m_Size / 4.0, m_Size)
        };
    painter->save();
    painter->setBrush(m_NonActiveColor);
    painter->setPen(Qt::lightGray);
    painter->drawConvexPolygon(backgroundPoints, 4);
    painter->restore();

   // qreal factor = m_Value; // TODO: sprawdzic czy factor tu pasuje jako mnoznik/ulamek
    qreal wingOffset = m_Size * 0.2 * m_Value;
    const QPointF progressBarPoints[4] = {
        QPointF(m_Size / 4.0 - wingOffset, m_Size - m_Size * m_Value),
        QPointF(3.0 * m_Size / 4.0 + wingOffset, m_Size - m_Size * m_Value),
        QPointF(3.0 * m_Size / 4.0, m_Size),
        QPointF(m_Size / 4.0, m_Size)
        };
    painter->save();
    painter->setBrush(m_ActiveColor);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(progressBarPoints, 4);
    painter->restore();
}
