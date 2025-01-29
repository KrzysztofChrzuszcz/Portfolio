#include "vbar.h"
#include <tuple>
#include <QtMath> 


VerticalBar::PolygonPoints::PolygonPoints()
{
    m_TopLeftCorner = QPoint(-0.5, 0.5);
    m_TopRightCorner = QPoint(0.5, 0.5);
    m_BottomRightCorner = QPoint(0.5, -0.5);
    m_BottomLeftCorner = QPoint(-0.5, -0.5);
}

VerticalBar::PolygonPoints::PolygonPoints(QPointF&& tl, QPointF&& tr, QPointF&& br, QPointF&& bl)
{
    m_TopLeftCorner = std::move(tl);
    m_TopRightCorner = std::move(tr);
    m_BottomRightCorner = std::move(br);
    m_BottomLeftCorner = std::move(bl);
}

VerticalBar::PolygonPoints::PolygonPoints(PolygonPoints&& pp)
{
    m_TopLeftCorner = std::move(pp.m_TopLeftCorner);
    m_TopRightCorner = std::move(pp.m_TopRightCorner);
    m_BottomRightCorner = std::move(pp.m_BottomRightCorner);
    m_BottomLeftCorner = std::move(pp.m_BottomLeftCorner);
}

VerticalBar::PolygonPoints& VerticalBar::PolygonPoints::operator=(const VerticalBar::PolygonPoints& other)
{
    if (this != &other)
    {
        m_TopLeftCorner = other.m_TopLeftCorner;
        m_TopRightCorner = other.m_TopRightCorner;
        m_BottomRightCorner = other.m_BottomRightCorner;
        m_BottomLeftCorner = other.m_BottomLeftCorner;
    }
    return *this;
}

VerticalBar::PolygonPoints& VerticalBar::PolygonPoints::operator=(VerticalBar::PolygonPoints&& other) noexcept
{
    if (this != &other)
    {
        m_TopLeftCorner = std::move(other.m_TopLeftCorner);
        m_TopRightCorner = std::move(other.m_TopRightCorner);
        m_BottomRightCorner = std::move(other.m_BottomRightCorner);
        m_BottomLeftCorner = std::move(other.m_BottomLeftCorner);
    }
    return *this;
}

std::array<QPointF, 4> VerticalBar::PolygonPoints::toPointsArray() const
{
    return { m_TopLeftCorner, m_TopRightCorner, m_BottomRightCorner, m_BottomLeftCorner };
}

VerticalBar::VerticalBar(QQuickItem* parent)
    : AbstractControl(parent)
{
    // Defaults
    m_TrackToDialSpacing = 5;
    // TODO1: use it  // TODO2: m_SideAngle i dodac zakres <0:18> // Wyjasnienie chcemy aby oba skrzydelka miescily sie w bounding rectangle wiec szerokosc skrzydelka nie moze przekraczac 1/3 wysokosci co daje maksymalny kat 18.44 // TODO3: Na angielski
    m_SideAngle = 0;
    m_DialLineWidth = 1;
    m_DialFontSize = 7;
    m_UpsideDown = false;

    connect(this, SIGNAL(sideAngleChanged()), this, SLOT(calculateVerticies())); // TODO: test in runtime

    //TODO: Assert na range m_SideAngle
}

void VerticalBar::setSideAngle(qreal angle)
{
    if (m_SideAngle == angle)
        return;

    m_SideAngle = angle;
    emit sideAngleChanged();
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

void inline paintWing(QPainter* painter, const QColor& color, qreal initialAlpha, const std::array<QPointF, 4>(&backgroundPoints), const std::tuple<QPointF, QPointF>& normalVector)
{
    QColor TransparentBackgoundColor = color;
    TransparentBackgoundColor.setAlphaF(initialAlpha);
    QLinearGradient gradient(std::get<0>(normalVector), std::get<1>(normalVector));
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(0.25, TransparentBackgoundColor); // !! TEMPORARY FIX (0.25)
    painter->setBrush(gradient);
    painter->drawConvexPolygon(backgroundPoints.data(), 4);
}

void VerticalBar::paintBacklight(QPainter* painter) // NOTE: Problem z angle = 0
{ // TODO: adjust to indicator y
    const std::array<QPointF,4> leftBackgroundPoints = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * (0.2 + qTan(qDegreesToRadians(m_SideAngle)) * m_Value), m_Size - m_Size * m_Value),
       QPointF(3.0 * m_Size / 6.0, m_Size - m_Size * m_Value),
       QPointF(3.0 * m_Size / 6.0, m_Size),
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, m_Size)
    };
    const std::array<QPointF, 4> rightBackgroundPoints = {
        QPointF(3.0 * m_Size / 6.0, m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0 + m_Size * (0.2 + qTan(qDegreesToRadians(m_SideAngle)) * m_Value), m_Size - m_Size * m_Value),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, m_Size),
        QPointF(3.0 * m_Size / 6.0, m_Size)
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

    paintWing(painter, backlightColor, 0.8, leftBackgroundPoints, makePerpendicularVector(m_MainPolygonPoints.m_TopLeftCorner, m_MainPolygonPoints.m_BottomLeftCorner)); // For left
    paintWing(painter, backlightColor, 0.8, rightBackgroundPoints, makePerpendicularVector(m_MainPolygonPoints.m_BottomRightCorner, m_MainPolygonPoints.m_TopRightCorner)); // For right
    painter->restore();
}

void VerticalBar::paintBackground(QPainter* painter) // NOTE: Problem z angle = 0
{
    painter->save();
    // TODO: 
    {
        painter->translate(QPoint(0, - m_Size * 0.05));
        painter->scale(1, 1/0.9);
    }
    painter->setPen(Qt::NoPen);
    paintWing(painter, m_BackgroundColor, 0.8, m_LeftWingPolygonPoints.toPointsArray(), makePerpendicularVector(m_MainPolygonPoints.m_TopLeftCorner, m_MainPolygonPoints.m_BottomLeftCorner)); // For left
    paintWing(painter, m_BackgroundColor, 0.8, m_RightWingPolygonPoints.toPointsArray(), makePerpendicularVector(m_MainPolygonPoints.m_BottomRightCorner, m_MainPolygonPoints.m_TopRightCorner)); // For right

    painter->restore();
}

void VerticalBar::paintTrack(QPainter* painter)
{
    painter->save();
    painter->setBrush(m_NonActiveColor);
    painter->setPen(m_NonActiveColor);
    painter->drawConvexPolygon(m_MainPolygonPoints.toPointsArray().data(), 4);
    painter->restore();
}

void VerticalBar::paintIndicator(QPainter* painter)
{
    qreal wingOffset = m_Size * qTan(qDegreesToRadians(m_SideAngle)) * m_Value; // NOTE: W tym wypadku walue jest w zakresie 0-1 ale trzeba by (!! TODO:) uzyc Min i Max Value!
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
        qreal wingOffset = m_Size * qTan(qDegreesToRadians(m_SideAngle)) * i / m_MaxValue;

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

    QPointF pointA = QPointF(2.0 * m_Size / 6.0 - m_Size * qTan(qDegreesToRadians(m_SideAngle)) - m_TrackToDialSpacing, 0);
    QPointF pointB = QPointF(2.0 * m_Size / 6.0 - m_TrackToDialSpacing, m_Size);

    QPointF pointC = QPointF(4.0 * m_Size / 6.0 + m_Size * qTan(qDegreesToRadians(m_SideAngle)) + m_TrackToDialSpacing, 0);
    QPointF pointD = QPointF(4.0 * m_Size / 6.0 + m_TrackToDialSpacing, m_Size);

    painter->drawLine(pointA, pointB);
    painter->drawLine(pointC, pointD);

    painter->restore();
}

void VerticalBar::calculateVerticies()
{
    qreal lenghtFactor = qTan(qDegreesToRadians(m_SideAngle));

    m_MainPolygonPoints = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * lenghtFactor, 0),
       QPointF(4.0 * m_Size / 6.0 + m_Size * lenghtFactor, 0),
       QPointF(4.0 * m_Size / 6.0, m_Size),
       QPointF(2.0 * m_Size / 6.0, m_Size)
    };

    m_LeftWingPolygonPoints = {
       QPointF(2.0 * m_Size / 6.0 - m_Size * (0.2 + lenghtFactor), 0),
       QPointF(3.0 * m_Size / 6.0, 0),
       QPointF(3.0 * m_Size / 6.0, m_Size),
       QPointF(2.0 * m_Size / 6.0 - m_Size * 0.2, m_Size)
    };

    m_RightWingPolygonPoints = {
        QPointF(3.0 * m_Size / 6.0, 0),
        QPointF(4.0 * m_Size / 6.0 + m_Size * (0.2 + lenghtFactor), 0),
        QPointF(4.0 * m_Size / 6.0 + m_Size * 0.2, m_Size),
        QPointF(3.0 * m_Size / 6.0, m_Size)
    };
}