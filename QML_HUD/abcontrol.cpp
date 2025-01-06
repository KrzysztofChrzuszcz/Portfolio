#include "abcontrol.h"

AbstractControl::AbstractControl(QQuickItem* parent)
    : QQuickPaintedItem(parent),
    m_Size(200),
    m_Value(0),
    m_ActiveColor(Qt::darkCyan), // TODO: Add m_BackgroundColor based on m_ActiveColor if there is non provided
    m_NonActiveColor(Qt::lightGray),
    m_DialColor(Qt::black),
    m_MirrorView(false),
    m_TrackToDialSpacing(5),
    m_MinValue(0),
    m_MaxValue(100),
    m_DialFontSize(8)
{
}

void AbstractControl::paint(QPainter* painter)
{
    paintBackground(painter);
    paintTrack(painter);
    paintIncicator(painter);
    paintDial(painter);
}

void AbstractControl::setSize(qreal size)
{
    if (m_Size == size)
        return;
    m_Size = size;
    emit sizeChanged();
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
