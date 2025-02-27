#ifndef RANGESLIDER_H
#define RANGESLIDER_H

// ATENTION!
// This class is external resource taken from
// https://github.com/ThisIsClark/Qt-RangeSlider (author: ThisIsClark)
// Licensed under MIT License - Copyright (c) 2019 ThisIsClark
// It serves to demonstrate the use of available resources that suit the goal, rather than reinventing the wheel from scratch.


#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class RangeSlider : public QWidget
{
    Q_OBJECT
    Q_ENUMS(RangeSliderTypes)

public:
    enum Option {
        NoHandle = 0x0,
        LeftHandle = 0x1,
        RightHandle = 0x2,
        DoubleHandles = LeftHandle | RightHandle
    };
    Q_DECLARE_FLAGS(Options, Option)

    RangeSlider( QWidget* aParent = Q_NULLPTR);
    RangeSlider( Qt::Orientation ori, Options t = DoubleHandles, QWidget* aParent = Q_NULLPTR);

    QSize minimumSizeHint() const override;

    int GetMinimum() const;
    void SetMinimum(int aMinimum);

    int GetMaximum() const;
    void SetMaximum(int aMaximum);

    int GetLowerValue() const;
    void SetLowerValue(int aLowerValue);

    int GetUpperValue() const;
    void SetUpperValue(int aUpperValue);

    void SetRange(int aMinimum, int aMaximum);

    void SetType(Options type); /// My own prosthesis

protected:
    void paintEvent(QPaintEvent* aEvent) override;
    void mousePressEvent(QMouseEvent* aEvent) override;
    void mouseMoveEvent(QMouseEvent* aEvent) override;
    void mouseReleaseEvent(QMouseEvent* aEvent) override;
    void changeEvent(QEvent* aEvent) override;

    QRectF firstHandleRect() const;
    QRectF secondHandleRect() const;
    QRectF handleRect(int aValue) const;

signals:
    void lowerValueChanged(int aLowerValue);
    void upperValueChanged(int aUpperValue);
    void rangeChanged(int aMin, int aMax);

public slots:
    void setLowerValue(int aLowerValue);
    void setUpperValue(int aUpperValue);
    void setMinimum(int aMinimum);
    void setMaximum(int aMaximum);

private:
    Q_DISABLE_COPY(RangeSlider)
    int validLength() const;

    int mMinimum;
    int mMaximum;
    int mLowerValue;
    int mUpperValue;
    bool mFirstHandlePressed;
    bool mSecondHandlePressed;
    int mInterval;
    int mDelta;
    QColor mBackgroundColorEnabled;
    QColor mBackgroundColorDisabled;
    QColor mBackgroundColor;
    Qt::Orientation orientation;
    Options type;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RangeSlider::Options)

#endif //RANGESLIDER_H