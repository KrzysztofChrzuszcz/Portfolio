#ifndef ABCONTROL_H
#define ABCONTROL_H

#include "abcontrol.h"
#include <QQuickPaintedItem>
#include <QBrush>

class AbstractControl : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal size MEMBER m_Size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qreal value MEMBER m_Value WRITE setValue NOTIFY sizeChanged)
    Q_PROPERTY(QColor activecolor MEMBER m_ActiveColor WRITE setActiveColor NOTIFY ActiveColorChanged)
    Q_PROPERTY(QColor nonactivecolor MEMBER m_NonActiveColor WRITE setNonActiveColor NOTIFY NonActiveColorChanged)

public:
    AbstractControl(QQuickItem* parent = 0);
    /// IMPORTANT NOTE:
    /// sic!) virtual void QQuickPaintedItem::paint(QPainter *painter) = 0;
    /// paint from QQuickPaintedItem has to be override in derived class
    // TODO: void paint(QPainter* painter);
    // virtualization intentionally disabled for better performance

    void setSize(qreal size);
    void setValue(qreal value);
    void setActiveColor(QColor color);
    void setNonActiveColor(QColor color);

signals:
    void sizeChanged();
    void valueChanged();
    void ActiveColorChanged();
    void NonActiveColorChanged();

protected:
    qreal   m_Size;
    qreal   m_Value;
    QColor  m_ActiveColor;
    QColor  m_NonActiveColor;
};

#endif // ABCONTROL_H