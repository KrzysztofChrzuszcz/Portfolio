#include "abcontrol.h"

AbstractControl::AbstractControl(QQuickItem* parent)
    : QQuickPaintedItem(parent),
    m_Size(200),
    m_Value(0),
    m_ActiveColor(Qt::darkCyan),
    m_NonActiveColor(Qt::lightGray)
{
    // TODO: konfiguracja m_ActiveColor & m_NonActiveColor

}

void AbstractControl::setSize(qreal size)
{
    if (m_Size == size)
        return;
    m_Size = size;
    emit sizeChanged();
}

void AbstractControl::setValue(qreal value)
{
    if (m_Value == value || value < 0 || value > 1)
        return;
    m_Value = value;
    emit valueChanged();
    update();
}

void AbstractControl::setActiveColor(QColor color)
{
    if (m_ActiveColor == color)
        return;
    m_ActiveColor = color;
    emit ActiveColorChanged();
    update();
}

void AbstractControl::setNonActiveColor(QColor color)
{
    if (m_NonActiveColor == color)
        return;
    m_NonActiveColor = color;
    emit NonActiveColorChanged();
    update();
}
