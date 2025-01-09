#ifndef VERTICALBAR_H
#define VERTICALBAR_H

#include "abcontrol.h"
#include <QPainter>

class VerticalBar : public AbstractControl
{
    Q_OBJECT
        // TODO1: use angle to calculate sides bow
        // TODO2: better fit for whole control in bounding rectangle (currently top, bottom, top-sides are cut)
        // TODO3: recreate background: 2 wings instead of current figure; create gradients on base of "sides normal"
    Q_PROPERTY(qreal angle MEMBER m_Angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(int diallinewidth MEMBER m_DialLineWidth WRITE setDialLineWidth NOTIFY dialLineWidthChanged)
    Q_PROPERTY(bool upsidedown MEMBER m_UpsideDown WRITE setUpsideDown NOTIFY upsideDownChanged)

public:
            VerticalBar(QQuickItem* parent = 0);

    void    setAngle(qreal angle);
    void    setDialLineWidth(int width);
    void    setUpsideDown(bool isUpsideDown);

protected:
    void    paintBacklight(QPainter* painter) override;
    void    paintBackground(QPainter* painter) override;
    void    paintTrack(QPainter* painter) override;
    void    paintIndicator(QPainter* painter) override;
    void    paintDial(QPainter* painter) override;

signals:
    void    angleChanged();
    void    dialLineWidthChanged();
    void    upsideDownChanged();

private:
    qreal   m_Angle;            //!< Angle between vertical axle and side of parallelogram
    int     m_DialLineWidth;    //!< Width of dial basic line (some lines will be slightly wider)
    bool    m_UpsideDown;       //!< Rotate control upside down
    //      m_TrackPoints // TODO: create (track, background wings, blacklight) points calculated with m_Angle change

};

#endif // VERTICALBAR_H