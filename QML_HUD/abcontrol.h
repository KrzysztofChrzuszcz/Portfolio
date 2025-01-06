#ifndef ABCONTROL_H
#define ABCONTROL_H

#include "abcontrol.h"
#include <QQuickPaintedItem>
#include <QBrush>

class AbstractControl : public QQuickPaintedItem
{
    Q_OBJECT
        // TODO: Pozmieniac qreal na inty ale najpierw spytac AI co o tym sadzi 
    Q_PROPERTY(qreal size MEMBER m_Size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool mirrorview MEMBER m_MirrorView WRITE setMirrorView NOTIFY mirrorViewChanged)
    Q_PROPERTY(QColor activecolor MEMBER m_ActiveColor WRITE setActiveColor NOTIFY activeColorChanged)
    Q_PROPERTY(QColor nonactivecolor MEMBER m_NonActiveColor WRITE setNonActiveColor NOTIFY nonActiveColorChanged)
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
    void    setMirrorView(bool mirrorview);
    void    setActiveColor(QColor color);
    void    setNonActiveColor(QColor color);
    void    setDialColor(QColor color);
    void    setDialFontSize(int size);
    void    setTrackToDialSpacing(int size);
    void    setMinValue(qreal minvalue);
    void    setMaxValue(qreal maxvalue);
    void    setValue(qreal value);


protected:
    virtual void    paintBackground(QPainter* painter) = 0;
    virtual void    paintTrack(QPainter* painter) = 0;
    virtual void    paintIncicator(QPainter* painter) = 0;
    virtual void    paintDial(QPainter* painter) = 0;

signals:
    void    sizeChanged();
    void    mirrorViewChanged();
    void    activeColorChanged();
    void    nonActiveColorChanged();
    void    dialColorChanged();
    void    dialFontSizeChanged();
    void    trackToDialSpacingChanged();
    void    minValueChanged();
    void    maxValueChanged();
    void    valueChanged();

protected:
    qreal   m_Size;                 //!< Size of control rectangle (both sides have to be equal)
    bool    m_MirrorView;           //!< Is view mirrored
    QColor  m_ActiveColor;          //!< Color representing value indicator
    QColor  m_NonActiveColor;       //!< Color of track
    QColor  m_DialColor;            //!< Color of dial
    int     m_DialFontSize;         //!< Size of labels in dial
    int     m_TrackToDialSpacing;   //!< Spacing between track and dial
    qreal   m_MinValue;             //!< Minimal possible value
    qreal   m_MaxValue;             //!< Maximal possible value
    qreal   m_Value;                //!< Current value to visualize

};

#endif // ABCONTROL_H