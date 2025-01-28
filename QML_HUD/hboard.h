#ifndef HORIZONTALBOARD_H
#define HORIZONTALBOARD_H

#include "abcontrol.h"
#include <QPainter>

class HorizontalBoard : public AbstractControl
{
    Q_OBJECT

    Q_PROPERTY(bool diverted MEMBER m_Diverted  WRITE setDiverted NOTIFY divertedChanged)
    Q_PROPERTY(DialType dialtype MEMBER m_DialType  WRITE setDialType NOTIFY dialTypeChanged)

public:
    enum DialType { Bottom, Upper, Full };
    Q_ENUM(DialType)


            HorizontalBoard(QQuickItem* parent = 0);

    void    setDiverted(bool isDiverted);
    void    setDialType(DialType dialType);

protected:
    void    paintBacklight(QPainter* painter) override;
    void    paintBackground(QPainter* painter) override;
    void    paintTrack(QPainter* painter) override;
    void    paintIndicator(QPainter* painter) override;
    void    paintDial(QPainter* painter) override;

signals:
    void    divertedChanged();
    void    dialTypeChanged();

private:
    bool        m_Diverted;        //!< Diverted board indicator
    DialType    m_DialType;        //!< Desired dial representation

};

#endif // HORIZONTALBOARD_H