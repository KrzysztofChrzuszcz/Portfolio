#ifndef VERTICALBAR_H
#define VERTICALBAR_H

#include "abcontrol.h"
#include <QPainter>
#include <array>

// TODO: Uporzadkowac komentarze dla tej struktury
/// Srodkowy pasek zajmuje 1/3 szerokosci na bokach zostaja miejsca na skrzydelka i miarke z dekoracjami. 
/// Dodatkowo calosc jest pomniejszona dla lepszej prezencji krancowych wartosci miarki
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
        // TODO3: recreate background: 2 wings instead of current figure; create gradients on base of "sides normal"
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
    qreal   m_SideAngle;        //!< Angle between vertical axle and side of parallelogram
    int     m_DialLineWidth;    //!< Width of dial basic line (some lines will be slightly wider)
    bool    m_UpsideDown;       //!< Rotate control upside down

    PolygonPoints m_MainPolygonPoints;
    PolygonPoints m_LeftWingPolygonPoints;
    PolygonPoints m_RightWingPolygonPoints;

};

#endif // VERTICALBAR_H