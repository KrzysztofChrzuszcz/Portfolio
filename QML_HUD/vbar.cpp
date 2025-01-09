#include "vbar.h"
#include <tuple>

VerticalBar::VerticalBar(QQuickItem* parent)
    : AbstractControl(parent)
{
    // Defaults
    m_TrackToDialSpacing = 5;
    m_Angle = 15; // TODO1: use it  // TODO2: sideAngle <0:30>
    m_DialLineWidth = 1;
    m_DialFontSize = 7;
    m_UpsideDown = false;
}

void VerticalBar::setAngle(qreal angle)
{
    if (m_Angle == angle)
        return;
    m_Angle = angle;
    emit angleChanged();
    // TODO: racalculate  trackPoints etc.
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

QPointF calculateNormal(const QPointF& p1, const QPointF& p2)
{
    QPointF edge = p2 - p1;
    QPointF normal(-edge.y(), edge.x());
    qreal length = std::sqrt(normal.x() * normal.x() + normal.y() * normal.y());
    if (length > 0.0)
        normal /= length;

    return normal;
}
// TODO: change approach to normal vector as QPointF + distance between 2 segments used with anchor point in the middle of track side
std::tuple<QPointF, QPointF> makePerpendicularVector(const QPointF& A, const QPointF& B)
{
    qreal midX = (A.x() + B.x()) / 2.0;
    qreal midY = (A.y() + B.y()) / 2.0;
    QPointF midpoint(midX, midY); // Point in the middle

    qreal dx = B.x() - A.x();
    qreal dy = B.y() - A.y();

    // Perpendicular vector
    qreal perpX = -dy;
    qreal perpY = dx;

    // Length of perpendicular vector
    qreal length = std::sqrt(perpX * perpX + perpY * perpY);
    if (length == 0.0)
    {
        // In case of identical points return tuple of same point
        return std::make_tuple(midpoint, midpoint);
    }
    // Normalize
    length = 1; // !! TEMPORARY FIX
    QPointF perpendicularPoint(midX + perpX / length, midY + perpY / length);

    return std::make_tuple(midpoint, perpendicularPoint);
}

void inline paintWing(QPainter* painter, const QColor& color, qreal initialAlpha, const QPointF (&backgroundPoints)[4], const std::tuple<QPointF, QPointF>& normalVector)
{
    QColor TransparentBackgoundColor = color;
    TransparentBackgoundColor.setAlphaF(initialAlpha);
    QLinearGradient gradient(std::get<0>(normalVector), std::get<1>(normalVector));
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(0.25, TransparentBackgoundColor); // !! TEMPORARY FIX (0.25)
    painter->setBrush(gradient);
    painter->drawConvexPolygon(backgroundPoints, 4);
}

void VerticalBar::paintBacklight(QPainter* painter)
{ // TODO: adjust to indicator y
    const QPointF leftBackgroundPoints[4] = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * (0.2 + 0.2 * m_Value), m_Size - m_Size * m_Value),
       QPointF(3.0 * m_Size / 6.0, m_Size - m_Size * m_Value),
       QPointF(3.0 * m_Size / 6.0, m_Size),
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, m_Size)
    };
    const QPointF rightBackgroundPoints[4] = {
        QPointF(3.0 * m_Size / 6.0, m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0 + m_Size * (0.2 + 0.2 * m_Value), m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, m_Size),
        QPointF(3.0 * m_Size / 6.0, m_Size)
    };
    static const QPointF trackPoints[4] = {
        QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0, m_Size),
        QPointF(2.0 * m_Size / 6.0, m_Size)
    };
    painter->save();
    // TODO:
    {
        painter->translate(QPoint(0, -m_Size * 0.05));
        painter->scale(1, 1 / 0.9);
    }
    painter->setPen(Qt::NoPen);
    QColor backlightColor = m_BacklightColor;
    backlightColor.setAlpha(120);

    paintWing(painter, backlightColor, 0.8, leftBackgroundPoints, makePerpendicularVector(trackPoints[0], trackPoints[3])); // For left
    paintWing(painter, backlightColor, 0.8, rightBackgroundPoints, makePerpendicularVector(trackPoints[2], trackPoints[1])); // For right
    painter->restore();
}

void VerticalBar::paintBackground(QPainter* painter)
{
   static const QPointF leftBackgroundPoints[4] = {
      QPointF(2.0 * m_Size / 6.0 - m_Size * 0.4, 0),
      QPointF(3.0 * m_Size / 6.0, 0),
      QPointF(3.0 * m_Size / 6.0, m_Size),
      QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, m_Size)
   };
   static const QPointF rightBackgroundPoints[4] = {
       QPointF(3.0 * m_Size / 6.0, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * 0.4, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, m_Size),
       QPointF(3.0 * m_Size / 6.0, m_Size)
   };
    static const QPointF trackPoints[4] = {
        QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, 0),
        QPointF(4.0 * m_Size / 6.0, m_Size),
        QPointF(2.0 * m_Size / 6.0, m_Size)
    };

    painter->save();
    // TODO: 
    {
        painter->translate(QPoint(0, - m_Size * 0.05));
        painter->scale(1, 1/0.9);
    }
    painter->setPen(Qt::NoPen);
    paintWing(painter, m_BackgroundColor, 0.8, leftBackgroundPoints, makePerpendicularVector(trackPoints[0], trackPoints[3]));
    paintWing(painter, m_BackgroundColor, 0.8, rightBackgroundPoints, makePerpendicularVector(trackPoints[2], trackPoints[1]));
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

    for (int i = 0; i <= m_MaxValue; i++) // TODO: use m_MinValue
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

        if (i % 5 == 0) // TODO: Parameter for 5
        {
            painter->save();


            if (i == 0 || i == m_MaxValue)
                painter->setPen(QPen(QBrush(m_DialColor), m_DialLineWidth + 1));

            painter->drawLine(pointA, pointB);
            painter->drawLine(pointC, pointD);
            painter->restore();

            if (i % 10 == 0)  // TODO: Parameter for 10
            {
                QString label = QString::number(i);
                QFont font = painter->font();
                font.setPointSize(m_DialFontSize);
                QFontMetrics fm(font);
                int textWidth = fm.width(label);
                int additionalOffset = fm.height() / 2 - 1;
                // TODO: Rethink those formulas, maybe simplify them
                QPointF labelPointL = QPointF(2.0 * m_Size / 6.0 - wingOffset - m_TrackToDialSpacing - lineLength - m_TrackToDialSpacing - textWidth, m_Size - m_Size * i / m_MaxValue + additionalOffset);
                QPointF labelPointR = QPointF(4.0 * m_Size / 6.0 + wingOffset + m_TrackToDialSpacing + lineLength + m_TrackToDialSpacing, m_Size - m_Size * i / m_MaxValue + additionalOffset);

                painter->save();
                painter->setFont(font);
                painter->translate(labelPointL);
                painter->drawText(QPointF(0, 0), label);
                painter->restore();

                painter->save();
                painter->setFont(font);
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
