#include "abcontrol.h"
#include <QPainter>

AbstractControl::AbstractControl(QQuickItem* parent)
    : QQuickPaintedItem(parent),
    m_Size(200),
    m_Scale(0.9),
    m_HorizontalShift(0),
    m_VerticalShift(0),
    m_MirrorView(false),
    m_ActiveColor(Qt::darkCyan), // TODO: Add m_BackgroundColor based on m_ActiveColor if there is non provided
    m_NonActiveColor(Qt::lightGray),
    m_DialColor(Qt::black),
    m_DialFontSize(8),
    m_TrackToDialSpacing(5),
    m_MinValue(0),
    m_MaxValue(100),
    m_Value(0)
{
}

void AbstractControl::paint(QPainter* painter)
{
    setupView(painter);
    paintBacklight(painter); /// We want paint backlight before background because transparency of both
    paintBackground(painter);
    paintTrack(painter);
    paintIndicator(painter);
    paintDial(painter); /// Some controls may need overlapping dial over indicator
}

void AbstractControl::setSize(qreal size)
{
    if (m_Size == size)
        return;
    m_Size = size;
    emit sizeChanged();
    update();
}

void AbstractControl::setScale(qreal scale)
{
    if (m_Scale == scale)
        return;
    m_Scale = scale;
    emit scaleChanged();
    update();
}

void AbstractControl::setHorizontalShift(int value)
{
    if (m_HorizontalShift == value)
        return;
    m_HorizontalShift = value;
    emit horizontalShiftChanged();
    update();
}

void AbstractControl::setVerticalShift(int value)
{
    if (m_VerticalShift == value)
        return;
    m_VerticalShift = value;
    emit verticalShiftChanged();
    update();
}

void AbstractControl::setMirrorView(bool mirrorview)
{
    if (m_MirrorView == mirrorview)
        return;
    m_MirrorView = mirrorview;
    emit mirrorViewChanged();
    update();
}


void AbstractControl::setActiveColor(QColor color)
{
    if (m_ActiveColor == color)
        return;
    m_ActiveColor = color;
    emit activeColorChanged();
    update();
}

void AbstractControl::setNonActiveColor(QColor color)
{
    if (m_NonActiveColor == color)
        return;
    m_NonActiveColor = color;
    emit nonActiveColorChanged();
    update();
}

void AbstractControl::setBackgroundColor(QColor color)
{
    if (m_BackgroundColor == color)
        return;
    m_BackgroundColor = color;
    emit backgroundColorChanged();
    update();
}

void AbstractControl::setBacklightColor(QColor color)
{
    if (m_BacklightColor == color)
        return;
    m_BacklightColor = color;
    emit backlightColorChanged();
    update();
}

void AbstractControl::setDialColor(QColor color)
{
    if (m_DialColor == color)
        return;
    m_DialColor = color;
    emit dialColorChanged();
    update();
}

void AbstractControl::setDialFontSize(int size)
{
    if (m_DialFontSize == size)
        return;
    m_DialFontSize = size;
    emit dialFontSizeChanged();
    update();
}

void AbstractControl::setTrackToDialSpacing(int spacing)
{
    if (m_TrackToDialSpacing == spacing)
        return;
    m_TrackToDialSpacing = spacing;
    emit trackToDialSpacingChanged();
    update();
}

void AbstractControl::setMinValue(qreal minvalue)
{
    if (m_MinValue == minvalue)
        return;
    m_MinValue = minvalue;
    emit minValueChanged();
    update();
}

void AbstractControl::setMaxValue(qreal maxvalue)
{
    if (m_MaxValue == maxvalue)
        return;
    m_MaxValue = maxvalue;
    emit maxValueChanged();
    update();
}

void AbstractControl::setValue(qreal value)
{
    if (m_Value == value || value < 0 || value > 1)
        return;
    m_Value = value;
    emit valueChanged();
    update();
}

void AbstractControl::setupView(QPainter* painter)
{
    /// Scale to the center
    painter->translate(QPoint(this->boundingRect().width() * (1.0 - m_Scale) / 2.0, this->boundingRect().height() * (1.0 - m_Scale) / 2.0));
    painter->scale(m_Scale, m_Scale);

    /// Shift display
    painter->translate(m_HorizontalShift, m_VerticalShift);

    /// Mirror view
    if (m_MirrorView)
    {
        painter->translate(this->boundingRect().topRight());
        painter->scale(-1, 1);
    }
}
