#include "gauge.h"
#include <QtMath> 
#include <QFontMetrics>
#include <QRadialGradient>

Gauge::Gauge(QQuickItem* parent)
    : AbstractControl(parent)
{
    // Defaults
    m_MinValue = 0;
    m_MaxValue = 280;
    m_SpanAngle = 280; // TODO2: zastanowic sie czy rozwazyc ujemny i obrot progressu w druga strone // TODO1: dodac walidacje dla angle ze nie moze byc wieksze niz 360, ze span dodatnie i wieksze= od jakiegos minimum np 90
    m_StartAngle = -50;
    m_ProgressWidth = 10;
    m_BoundingToTrackSpacing = 35;
    m_TrackToDialSpacing = 5;
    m_DialFontSize = 7;
}

void Gauge::setBoundingToTrackSpacing(int boundingToTrackSpacing)
{
    if (m_BoundingToTrackSpacing == boundingToTrackSpacing)
        return;
    m_BoundingToTrackSpacing = boundingToTrackSpacing;
    emit boundingToTrackSpacingChanged();
    update();
}

void Gauge::setSpanAngle(qreal spanangle)
{
    if (m_SpanAngle == spanangle)
        return;
    m_SpanAngle = spanangle;
    emit spanAngleChanged();
    update();
}

void Gauge::setStartAngle(qreal startangle)
{
    if (m_StartAngle == startangle)
        return;
    m_StartAngle = startangle;
    emit startAngleChanged();
    update();
}

void Gauge::setProgressWidth(int progresswidth)
{
    if (m_ProgressWidth == progresswidth)
        return;
    m_ProgressWidth = progresswidth;
    emit progressWidthChanged();
    update();
}

void Gauge::paintBackground(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QRadialGradient gradient(boundingRect().center(), boundingRect().width()/2);
    gradient.setColorAt(0.8, QColor(0, 0, 0, 0)); // TODO: Use m_BoundingToTrackSpacing to calculate value to replace 0.8
    QColor TransparentBackgoundColor = m_ActiveColor;
    TransparentBackgoundColor.setAlphaF(0.8);
    gradient.setColorAt(1.0, TransparentBackgoundColor);
    QRectF borderRect = boundingRect(); // NOTE!: Too small!
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->setBrush(gradient);
    painter->drawEllipse(borderRect);
    painter->setBrush(Qt::gray);
    QRectF trackRect = boundingRect().adjusted(m_BoundingToTrackSpacing, m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing);
    painter->drawEllipse(trackRect);
    painter->restore();
}

void Gauge::paintTrack(QPainter* painter)
{
    QRectF trackRect = boundingRect().adjusted(m_BoundingToTrackSpacing, m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing);
    QPen pen = painter->pen();
    painter->save();
    painter->translate(this->boundingRect().topRight());
    painter->scale(-1, 1);
    painter->setBrush(Qt::NoBrush);
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(m_ProgressWidth);
    pen.setColor(m_NonActiveColor);
    painter->setPen(pen);
    QRectF valueRect = trackRect;
    valueRect.adjust(m_ProgressWidth / 2, m_ProgressWidth / 2, -m_ProgressWidth / 2, -m_ProgressWidth / 2);
    painter->drawArc(valueRect, m_StartAngle * 16, m_SpanAngle  * 16);
    painter->restore();
}

void Gauge::paintIndicator(QPainter* painter)
{
    qreal valueAngle = (((m_SpanAngle * m_Value) - m_MinValue) / (m_MaxValue - m_MinValue)) * m_SpanAngle;
    QRectF trackRect = boundingRect().adjusted(m_BoundingToTrackSpacing, m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing, -m_BoundingToTrackSpacing);
    QPen pen = painter->pen();
    painter->save();
    painter->translate(this->boundingRect().topRight());
    painter->scale(-1, 1);
    painter->setBrush(Qt::NoBrush);
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(m_ProgressWidth);
    pen.setColor(m_ActiveColor);
    painter->setPen(pen);
    QRectF valueRect = trackRect;
    valueRect.adjust(m_ProgressWidth / 2, m_ProgressWidth / 2, -m_ProgressWidth / 2, -m_ProgressWidth / 2);
    painter->drawArc(valueRect, m_StartAngle * 16, valueAngle * 16);
    painter->restore();
}

void Gauge::paintDial(QPainter* painter)
{
    painter->save();
    painter->translate(this->boundingRect().bottomLeft());
    painter->scale(1, -1); // TODO: Use m_MirrorView
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_DialColor);

    QRectF rect = this->boundingRect();
    rect.adjust(m_ProgressWidth, m_ProgressWidth, -m_ProgressWidth, -m_ProgressWidth);
    QRectF outerRect = this->boundingRect();

    for (int i = 0; i <= m_MaxValue; i++)
    {
        qreal angle = (m_StartAngle + (i * (m_SpanAngle / m_MaxValue)));

        paintMeasureMark(painter, outerRect, angle, i);

        if (i == m_MaxValue && m_MaxValue == 360)
            continue; /// fix for a problem with double labels in case of full circle

        if (i % 20 == 0) // TODO parameter instead of 20
            paintMeasureLabel(painter, outerRect, angle, i);
    }

    painter->restore();
}
inline void Gauge::paintMeasureMark(QPainter* painter, QRectF& outerRect, qreal angle, int i) // TODO: Change outerRect on trackRect for proper use of m_TrackToDialSpacing (+adjust value)
{
    painter->save();
    painter->translate(this->boundingRect().topRight());
    painter->scale(-1, 1);
    painter->setPen(QPen(m_DialColor));
    // TODO: 
    qreal distanceBetterName = m_BoundingToTrackSpacing - m_TrackToDialSpacing;
    QPointF pointA = outerRect.center() + QPointF(std::cos(qDegreesToRadians(angle)) * (outerRect.width() / 2 - distanceBetterName), std::sin(qDegreesToRadians(angle)) * (outerRect.height() / 2 - distanceBetterName));
    qreal lineLength = (i % 20 == 0) ? 10 : 6; // TODO parameter instead of 20
    if (i == m_MaxValue)
        lineLength += 2;

    QPointF pointB = pointA + QPointF(std::cos(qDegreesToRadians(angle)) * lineLength, std::sin(qDegreesToRadians(angle)) * lineLength);

    if (i % 5 == 0) // TODO parameter instead of 5 (but inside AbstractControl)
        painter->drawLine(pointA, pointB); // TODO: Add bold for the first and last marks (maybe increase their length also) BUT ONLY IF spanAngle < 360

    painter->restore();
}
inline void Gauge::paintMeasureLabel(QPainter* painter, QRectF& outerRect, qreal angle, int i) // TODO: Consider to pass references to QPointF
{
    int numMarks = m_MaxValue;

    qreal labelRadiance = 13; // TODO: formula for it, like : m_TrackToDialSpacing + 3
    // TODO: Rethink those formulas, maybe simplify them
    // TODO2: 
    qreal distanceBetterName = m_BoundingToTrackSpacing - m_TrackToDialSpacing;
    QPointF pointA = outerRect.center() + QPointF(std::cos(qDegreesToRadians(angle)) * (outerRect.width() / 2 - distanceBetterName), std::sin(qDegreesToRadians(angle)) * (outerRect.height() / 2 - distanceBetterName));
    QPointF pointC = pointA + QPointF(std::cos(qDegreesToRadians(angle)) * labelRadiance, std::sin(qDegreesToRadians(angle)) * labelRadiance);

    QFont font = painter->font();
    //int pointSize = font.pointSize(); // TODO: use it for scalability
    font.setPointSize(m_DialFontSize);

    QFontMetrics fm(font);
    QString label = QString::number(i);
    int textWidth = fm.width(label);
    //int textHeight = fm.height(); // TODO: OR use it for scalability

    painter->setFont(font);
    painter->setPen(QPen(m_DialColor));

    painter->save();
    painter->translate(this->boundingRect().topRight());
    painter->scale(-1, 1);
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
