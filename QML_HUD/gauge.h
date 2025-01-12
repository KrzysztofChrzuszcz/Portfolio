#ifndef GAUGE_H
#define GAUGE_H

#include "abcontrol.h"
#include <QPainter>

/// Positive values for the angles mean clockwise while negative values mean the counter-clockwise direction. Zero degrees is at the 9 o'clock position.
class Gauge : public AbstractControl
{
    Q_OBJECT
        // TODO: Add to Read.me where is start angle and attach pictures with red markings to explain properties
    Q_PROPERTY(int boundingtotrackspacing MEMBER m_BoundingToTrackSpacing WRITE setBoundingToTrackSpacing NOTIFY boundingToTrackSpacingChanged)
    Q_PROPERTY(qreal spanangle MEMBER m_SpanAngle WRITE setSpanAngle NOTIFY spanAngleChanged)
    Q_PROPERTY(qreal startangle MEMBER m_StartAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(int progresswidth MEMBER m_ProgressWidth WRITE setProgressWidth NOTIFY progressWidthChanged)

public:
            Gauge(QQuickItem* parent = 0);

    void    setBoundingToTrackSpacing(int boundingToTrackSpacing);
    void    setSpanAngle(qreal spanangle);
    void    setStartAngle(qreal startangle);
    void    setProgressWidth(int progresswidth);

protected:
    void    setupView(QPainter* painter) override;
    void    paintBacklight(QPainter* painter) override;
    void    paintBackground(QPainter* painter) override;
    void    paintTrack(QPainter* painter) override;
    void    paintIndicator(QPainter* painter) override;
    void    paintDial(QPainter* painter) override;

private:
    void    paintMeasureMark(QPainter* painter, QRectF& outerRect, qreal angle, int i);
    void    paintMeasureLabel(QPainter* painter, QRectF& outerRect, qreal angle, int i);
    
signals:
    void    boundingToTrackSpacingChanged();
    void    spanAngleChanged();
    void    startAngleChanged();
    void    progressWidthChanged();

private:
    int     m_BoundingToTrackSpacing;   //!< Spacing between bounding rectangle and track
    qreal   m_SpanAngle;                //!< Angle from a start angle to end angle
    qreal   m_StartAngle;               //!< Position of start of progress visualization
    int     m_ProgressWidth;            //!< Width of progress indicator

};

#endif // GAUGE_H