#include "vbar.h"

VerticalBar::VerticalBar(QQuickItem* parent)
    : AbstractControl(parent)
{
    // Defaults
    m_TrackToDialSpacing = 5;
    m_Angle = 15; // TODO1: use it  // TODO2: sideAngle <0:30>
    m_DialLineWidth = 1;
    m_UpsideDown = false;
}

void VerticalBar::setAngle(qreal angle)
{
    if (m_Angle == angle)
        return;
    m_Angle = angle;
    emit angleChanged();
    update();
}

void VerticalBar::setDialLineWidth(int width)
{
    if (m_DialLineWidth == width)
        return;
    m_DialLineWidth = width;
    emit dialLineWidthChanged();
    update();
}

void VerticalBar::setUpsideDown(bool isUpsideDown)
{
    if (m_UpsideDown == isUpsideDown)
        return;
    m_UpsideDown = isUpsideDown;
    emit upsideDownChanged();
    update();
}

void VerticalBar::paintBackground(QPainter* painter)
{
    static const QPointF backgroundPoints[4] = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.4, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * 0.4, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, m_Size),
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, m_Size)
    };
    painter->save();
    QColor TransparentBackgoundColor = m_ActiveColor;
    TransparentBackgoundColor.setAlphaF(0.8);
    QLinearGradient gradient(QPointF(0, m_Size/2), QPointF(m_Size, m_Size/2));
    gradient.setColorAt(0, TransparentBackgoundColor);
    gradient.setColorAt(0.2, QColor(0,0,0,0));
    gradient.setColorAt(0.8, QColor(0, 0, 0, 0));
    gradient.setColorAt(1, TransparentBackgoundColor);
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawConvexPolygon(backgroundPoints, 4);
    painter->restore();
}

void VerticalBar::paintTrack(QPainter* painter)
{
    static const QPointF trackPoints[4] = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, 0),
       QPointF(4.0 * m_Size / 6.0, m_Size),
       QPointF(2.0 * m_Size / 6.0, m_Size)
    };
    painter->save();
    painter->setBrush(m_NonActiveColor);
    painter->setPen(m_NonActiveColor);
    painter->drawConvexPolygon(trackPoints, 4);
    painter->restore();
}

void VerticalBar::paintIndicator(QPainter* painter)
{
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
}

void VerticalBar::paintDial(QPainter* painter)
{
    painter->setPen(QPen(QBrush(m_DialColor), m_DialLineWidth));

    for (int i = 0; i <= m_MaxValue; i++)
    {
        qreal wingOffset = m_Size * 0.2 * i / m_MaxValue;

        qreal lineLength = 0;
        if (i == 0 || i == m_MaxValue)
            lineLength = 12; /// Longest markers for first and last of them
        else if (i % 10 == 0)
            lineLength = 10; /// Labeled markers
        else
            lineLength = 6; /// Basic markers

        QPointF pointA = QPointF(4.0 * m_Size / 6.0 + wingOffset + m_TrackToDialSpacing, m_Size - m_Size * i / m_MaxValue);
        QPointF pointB = QPointF(4.0 * m_Size / 6.0 + wingOffset + m_TrackToDialSpacing + lineLength, m_Size - m_Size * i / m_MaxValue);

        QPointF pointC = QPointF(2.0 * m_Size / 6.0 - wingOffset - m_TrackToDialSpacing, m_Size - m_Size * i / m_MaxValue);
        QPointF pointD = QPointF(2.0 * m_Size / 6.0 - wingOffset - m_TrackToDialSpacing - lineLength, m_Size - m_Size * i / m_MaxValue);

        if (i % 5 == 0 || i == 0)
        {
            painter->save();
            if (i == 0 || i == m_MaxValue)
                painter->setPen(QPen(QBrush(m_DialColor), m_DialLineWidth + 1));

            painter->drawLine(pointA, pointB);
            painter->drawLine(pointC, pointD);
            painter->restore();

            if (i % 10 == 0)
            {
                QString label = QString::number(i);
                QFont font = painter->font();
                QFontMetrics fm(font);
                int textWidth = fm.width(label);
                // TODO: Rethink those formulas, maybe simplify them
                QPointF labelPointL = QPointF(2.0 * m_Size / 6.0 - wingOffset - m_TrackToDialSpacing - lineLength - m_TrackToDialSpacing - textWidth, m_Size - m_Size * i / m_MaxValue);
                QPointF labelPointR = QPointF(4.0 * m_Size / 6.0 + wingOffset + m_TrackToDialSpacing + lineLength + m_TrackToDialSpacing, m_Size - m_Size * i / m_MaxValue);

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
    painter->save();
    painter->setPen(QPen(QBrush(m_DialColor), m_DialLineWidth + 1));

    QPointF pointA = QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2 - m_TrackToDialSpacing, 0);
    QPointF pointB = QPointF(2.0 * m_Size / 6.0 - m_TrackToDialSpacing, m_Size);

    QPointF pointC = QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2 + m_TrackToDialSpacing, 0);
    QPointF pointD = QPointF(4.0 * m_Size / 6.0 + m_TrackToDialSpacing, m_Size);

    painter->drawLine(pointA, pointB);
    painter->drawLine(pointC, pointD);

    painter->restore();
}
