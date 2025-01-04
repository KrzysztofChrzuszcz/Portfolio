#include "gauge.h"
#include <QtMath> 
#include <QFontMetrics>

Gauge::Gauge(QQuickItem* parent)
    : AbstractControl(parent)
{
}

void Gauge::paint(QPainter* painter)
{
    // ! TODO: bool m_MirrorView
    // TODO: PROPERTY
    qreal m_MinValue = 0;
    qreal m_MaxValue = 360;
    qreal spanAngle = -360; // Zakladajac, ze pelny obrot to 360 stopni
    qreal startAngle = -90; // Ustawienie poczatku na dole (0 stopni)

    qreal offset = 10; // Przesuniecie, by nie rysowac na brzegach
    qreal markOffsetA = 30; // Przesuniecie krawedzi tarczy
    qreal markOffsetB = 25; // Przesuniecie cyferblatu
    qreal progressWidth = 10; // Szerokosc paska postepu

    // TODO: paintTrack()
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setBrush(m_NonActiveColor);
    QRectF trackRect = boundingRect().adjusted(markOffsetA, markOffsetA, -markOffsetA, -markOffsetA);
    painter->drawEllipse(trackRect);
    painter->restore();


    // TODO:  paintIncicator()
    // TODO: gradient lub paski (apha =0)
    qreal valueAngle = (((360.0 * m_Value) - m_MinValue) / (m_MaxValue - m_MinValue)) * spanAngle;

    QPen pen = painter->pen();
    painter->setBrush(Qt::NoBrush);
    pen.setCapStyle(Qt::FlatCap);
    painter->save();
    pen.setWidth(10);
    pen.setColor(m_ActiveColor);
    painter->setPen(pen);
    QRectF valueRect = trackRect;
    valueRect.adjust(progressWidth / 2, progressWidth / 2, -progressWidth / 2, -progressWidth / 2);
    painter->drawArc(valueRect, startAngle * 16, valueAngle * 16);
    painter->restore();


    // TODO: paintDial()
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::black);


    QRectF rect = this->boundingRect();
    rect.adjust(offset, offset, -offset, -offset);
    QRectF outerRect = this->boundingRect();

    int numMarks = m_MaxValue;

    for (int i = 0; i <= numMarks; ++i)
    {
        qreal angle = -1 * (startAngle + (i * (spanAngle / numMarks)));
        QPointF pointA = outerRect.center() + QPointF(std::cos(qDegreesToRadians(angle)) * (outerRect.width() / 2 - markOffsetB), std::sin(qDegreesToRadians(angle)) * (outerRect.height() / 2 - markOffsetB));
        qreal lineLength = (i % 15 == 0) ? 10 : 6;
        QPointF pointB = pointA + QPointF(std::cos(qDegreesToRadians(angle)) * lineLength, std::sin(qDegreesToRadians(angle)) * lineLength);
        qreal labelRadiance = 13;
        QPointF pointC = pointA + QPointF(std::cos(qDegreesToRadians(angle)) * labelRadiance, std::sin(qDegreesToRadians(angle)) * labelRadiance);

        // TODO: inline paintMeasureMarks()
        if (i % 5 == 0)
            painter->drawLine(pointA, pointB); // TODO: fix BUG! No line for 0 in runtime

        if (i == numMarks)
            continue; /// fix for a problem with double labels

        // TODO: inline paintMeasureLabels()
        if (i % 15 == 0) {
            QFont font = painter->font();
            //int pointSize = font.pointSize(); // TODO: use it for scalability
            font.setPointSize(7);

            QFontMetrics fm(font);
            QString label = QString::number(i);
            int textWidth = fm.width(label);
            //int textHeight = fm.height(); // TODO: OR use it for scalability

            painter->setFont(font);
            painter->setPen(QPen(Qt::black));

            painter->save();
            if (i < numMarks / 2)
            {
                int textOffset = textWidth;
                pointC = pointA + QPointF(std::cos(qDegreesToRadians(angle)) * (labelRadiance + textOffset), std::sin(qDegreesToRadians(angle)) * (labelRadiance + textOffset));
            }

            painter->translate(pointC);

            if (i < numMarks / 2)
                painter->rotate(180);

            painter->rotate(angle);

            painter->drawText(QPointF(0, 3.5), QString::number(i));
            painter->restore();
        }
    }

    painter->restore();
}
