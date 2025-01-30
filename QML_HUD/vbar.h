#ifndef VERTICALBAR_H
#define VERTICALBAR_H

#include "abcontrol.h"
#include <QPainter>
#include <array>

/*!
 * Class VerticalBar
 *  \brief  Middle part is vertical bar of width of 1/3 of size centred to the middle.
 *          Wings on sites can take maximum 1/3 of space, so side angle has to be in range between 0 and 18
 *          Additionally whole item has to be scaled due to keep better display of edge values on dial
 *          
 *  \note   Wings have to fit side space of bounding rectangle (1/3 of width). Because of that maximum side angle could be 18.44 degrees.
            It is possible to turn upside down the bar. (TODO)
 */

class VerticalBar : public AbstractControl
{
    struct PolygonPoints
    {
        QPointF m_TopLeftCorner;
        QPointF m_TopRightCorner;
        QPointF m_BottomRightCorner;
        QPointF m_BottomLeftCorner;

        PolygonPoints();
        PolygonPoints(QPointF&& tl, QPointF&& tr, QPointF&& br, QPointF&& bl);
        PolygonPoints(PolygonPoints& pp) = delete;
        PolygonPoints(PolygonPoints&& pp);

        PolygonPoints& operator=(const PolygonPoints& other);
        PolygonPoints& operator=(PolygonPoints&& other) noexcept;

        std::array<QPointF, 4> toPointsArray() const;
    };

    Q_OBJECT
        // X TODO1: use angle to calculate sides bow
        // TODO2: better fit for whole control in bounding rectangle (currently top, bottom, top-sides are cut)
        // X TODO3: recreate background: 2 wings instead of current figure; create gradients on base of "sides normal"
    Q_PROPERTY(qreal sideangle MEMBER m_SideAngle WRITE setSideAngle NOTIFY sideAngleChanged)
    Q_PROPERTY(int diallinewidth MEMBER m_DialLineWidth WRITE setDialLineWidth NOTIFY dialLineWidthChanged)
    Q_PROPERTY(bool upsidedown MEMBER m_UpsideDown WRITE setUpsideDown NOTIFY upsideDownChanged)

public:
            VerticalBar(QQuickItem* parent = 0);

    void    setSideAngle(qreal angle);
    void    setDialLineWidth(int width);
    void    setUpsideDown(bool isUpsideDown);

protected:
    void    paintBacklight(QPainter* painter) override;
    void    paintBackground(QPainter* painter) override;
    void    paintTrack(QPainter* painter) override;
    void    paintIndicator(QPainter* painter) override;
    void    paintDial(QPainter* painter) override;

signals:
    void    sideAngleChanged();
    void    dialLineWidthChanged();
    void    upsideDownChanged();

protected slots:
    void    calculateVerticies();

private:
    qreal   m_SideAngle;        //!< Angle between vertical axle and side of parallelogram. (in degrees)
    int     m_DialLineWidth;    //!< Width of dial basic line (some lines will be slightly wider)
    bool    m_UpsideDown;       //!< Rotate control upside down

    PolygonPoints m_MainPolygonPoints;
    PolygonPoints m_LeftWingPolygonPoints;
    PolygonPoints m_RightWingPolygonPoints;

};

#endif // VERTICALBAR_H