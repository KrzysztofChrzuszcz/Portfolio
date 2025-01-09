#ifndef ABCONTROL_H
#define ABCONTROL_H

#include "abcontrol.h"
#include <QQuickPaintedItem>
#include <QBrush>

// TODO: Check in derived classes if all parameters/class members are used (some may be forgotten)
class AbstractControl : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(qreal size MEMBER m_Size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qreal scale MEMBER m_Scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(qreal horizontalshift MEMBER m_HorizontalShift WRITE setHorizontalShift NOTIFY horizontalShiftChanged)
    Q_PROPERTY(qreal verticalshift MEMBER m_VerticalShift WRITE setVerticalShift NOTIFY verticalShiftChanged)
    Q_PROPERTY(bool mirrorview MEMBER m_MirrorView WRITE setMirrorView NOTIFY mirrorViewChanged)
    Q_PROPERTY(QColor activecolor MEMBER m_ActiveColor WRITE setActiveColor NOTIFY activeColorChanged)
    Q_PROPERTY(QColor nonactivecolor MEMBER m_NonActiveColor WRITE setNonActiveColor NOTIFY nonActiveColorChanged)
    Q_PROPERTY(QColor backgroundcolor MEMBER m_BackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor backlightcolor MEMBER m_BacklightColor WRITE setBacklightColor NOTIFY backlightColorChanged)
    Q_PROPERTY(QColor dialcolor MEMBER m_DialColor WRITE setDialColor NOTIFY dialColorChanged)
    Q_PROPERTY(int dialfontsize MEMBER m_DialFontSize WRITE setDialFontSize NOTIFY dialFontSizeChanged)
    Q_PROPERTY(int tracktodialspacing MEMBER m_TrackToDialSpacing WRITE setTrackToDialSpacing NOTIFY trackToDialSpacingChanged)
    Q_PROPERTY(qreal minvalue MEMBER m_MinValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(qreal maxvalue MEMBER m_MaxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(qreal value MEMBER m_Value WRITE setValue NOTIFY valueChanged)


public:
            AbstractControl(QQuickItem* parent = 0);
    virtual ~AbstractControl() = default;

    void    paint(QPainter* painter) final;

    void    setSize(qreal size);
    void    setScale(qreal scale);
    void    setHorizontalShift(int value);
    void    setVerticalShift(int value);
    void    setMirrorView(bool mirrorview);
    void    setActiveColor(QColor color);
    void    setNonActiveColor(QColor color);
    void    setBackgroundColor(QColor color);
    void    setBacklightColor(QColor color);
    void    setDialColor(QColor color);
    void    setDialFontSize(int size);
    void    setTrackToDialSpacing(int size);
    void    setMinValue(qreal minvalue);
    void    setMaxValue(qreal maxvalue);
    void    setValue(qreal value);


protected:
    virtual void    setupView(QPainter* painter);           //!< Setup all necessary view configuration
    virtual void    paintBacklight(QPainter* painter) = 0;  //!< Acording to indicator a backlight under background
    virtual void    paintBackground(QPainter* painter) = 0; //!< Background on edges of control 
    virtual void    paintTrack(QPainter* painter) = 0;      //!< Track for indicator on current value
    virtual void    paintIndicator(QPainter* painter) = 0;  //!< Indicator of current value
    virtual void    paintDial(QPainter* painter) = 0;       //!< The dial (markers and labels)

signals:
    void    sizeChanged();
    void    scaleChanged();
    void    horizontalShiftChanged();
    void    verticalShiftChanged();
    void    mirrorViewChanged();
    void    activeColorChanged();
    void    nonActiveColorChanged();
    void    backgroundColorChanged();
    void    backlightColorChanged();
    void    dialColorChanged();
    void    dialFontSizeChanged();
    void    trackToDialSpacingChanged();
    void    minValueChanged();
    void    maxValueChanged();
    void    valueChanged();

protected:
    qreal   m_Size;                 //!< Size of control rectangle (both sides have to be equal)
    qreal   m_Scale;                //!< Scale of control view (centered)
    int     m_HorizontalShift;      //!< Horizontal shift of view
    int     m_VerticalShift;        //!< Vertical shift of view
    bool    m_MirrorView;           //!< Is view mirrored
    QColor  m_ActiveColor;          //!< Color representing value indicator
    QColor  m_NonActiveColor;       //!< Color of track
    QColor  m_BackgroundColor;      //!< Color of background
    QColor  m_BacklightColor;       //!< Color of backlight behind background
    QColor  m_DialColor;            //!< Color of dial
    int     m_DialFontSize;         //!< Size of labels in dial
    // QString m_FontFamily;
    int     m_TrackToDialSpacing;   //!< Spacing between track and dial
    qreal   m_MinValue;             //!< Minimal possible value
    qreal   m_MaxValue;             //!< Maximal possible value
    qreal   m_Value;                //!< Current value to visualize

};

#endif // ABCONTROL_H