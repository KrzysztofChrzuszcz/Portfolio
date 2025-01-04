#include "vbar.h"

VerticalBar::VerticalBar(QQuickItem* parent)
    : AbstractControl(parent)
{
}

void VerticalBar::paint(QPainter* painter)
{
    // TODO: poprawic gorny zakres aby zmiesciec cyferblat
    //  Na bazie font higth zrobic adjust do boundingRect tak aby w obszarze kontrolki miescily sie wszystkie dodatki (liczby cyferblatu) 
    
    // TODO: PROPERTY
    // TODO: angle instead factor <0;45> : default = 15
    // ! TODO: bool m_MirrorView

    // TODO: paintTrack()
    static const QPointF backgroundPoints[4] = {
        QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0, m_Size),
        QPointF(2.0 * m_Size / 6.0, m_Size)
        };
    painter->save();
    painter->setBrush(m_NonActiveColor);
    painter->setPen(Qt::lightGray);
    painter->drawConvexPolygon(backgroundPoints, 4);
    painter->restore();

    // TODO:  paintIncicator()
    // TODO: gradient lub paski (apha =0)
    qreal wingOffset = m_Size * 0.2 * m_Value;
    const QPointF progressBarPoints[4] = {
        QPointF(2.0 * m_Size / 6.0 - wingOffset, m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0 + wingOffset, m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0, m_Size),
        QPointF(2.0 * m_Size / 6.0, m_Size)
        };
    painter->save();
    painter->setBrush(m_ActiveColor);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(progressBarPoints, 4);
    painter->restore();

    // TODO: paintDial()
    qreal spacing = 5;
    painter->save();
    painter->setPen(QPen(Qt::black));

    QPointF pointA = QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2 - spacing, 0);
    QPointF pointB = QPointF(2.0 * m_Size / 6.0 - spacing, m_Size);

    QPointF pointC = QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2 + spacing, 0);
    QPointF pointD = QPointF(4.0 * m_Size / 6.0 + spacing, m_Size);

    painter->drawLine(pointA, pointB);
    painter->drawLine(pointC, pointD);

    for (int i = 0; i <= 100; i++)
    {
        qreal wingOffset = m_Size * 0.2 * i / 100;

        qreal lineLength = 0; 
        if (i == 0 || i == 100)
            lineLength = 12; /// Longest markers for first and last of them
        else if (i % 10 == 0)
            lineLength = 10; /// Labeled markers
        else
            lineLength = 6; /// Basic markers

        QPointF pointA = QPointF(4.0 * m_Size / 6.0 + wingOffset + spacing, m_Size - m_Size * i / 100);
        QPointF pointB = QPointF(4.0 * m_Size / 6.0 + wingOffset + spacing + lineLength, m_Size - m_Size * i / 100);

        QPointF pointC = QPointF(2.0 * m_Size / 6.0 - wingOffset - spacing, m_Size - m_Size * i / 100);
        QPointF pointD = QPointF(2.0 * m_Size / 6.0 - wingOffset - spacing - lineLength, m_Size - m_Size * i / 100);

        if (i % 5 == 0 || i == 0)
        {
            painter->drawLine(pointA, pointB);
            painter->drawLine(pointC, pointD);

            if (i % 10 == 0)
            {
                QString label = QString::number(i);
                QFont font = painter->font();
                QFontMetrics fm(font);
                int textWidth = fm.width(label);

                QPointF labelPointL = QPointF(2.0 * m_Size / 6.0 - wingOffset - spacing - lineLength - spacing - textWidth, m_Size - m_Size * i / 100);
                QPointF labelPointR = QPointF(4.0 * m_Size / 6.0 + wingOffset + spacing + lineLength + spacing, m_Size - m_Size * i / 100);

                painter->save();
                painter->translate(labelPointL);
                painter->drawText(QPointF(0, 0), label);
                painter->restore();

                painter->save();
                painter->translate(labelPointR);
                painter->drawText(QPointF(0, 0), label);
                painter->restore();
            }
        }
    }
    painter->restore();
    // + linie prawie pionowe (rownolegle do brzegow)
}
