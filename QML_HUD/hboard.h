#ifndef HORIZONTALBOARD_H
#define HORIZONTALBOARD_H

#include "abcontrol.h"
#include <QPainter>

class HorizontalBoard : public AbstractControl
{
    Q_OBJECT

    Q_PROPERTY(bool diverted MEMBER m_Diverted  WRITE setDiverted NOTIFY divertedChanged)
    Q_PROPERTY(QString dialtype MEMBER m_DialType  WRITE setDialType NOTIFY dialTypeChanged) // TODO: full (default) upper, bottom // TODO2: property string + enum member // TODO3: Ask AI about what it's thinking about duplications in this case

public:
            HorizontalBoard(QQuickItem* parent = 0);

    void    setDiverted(bool isDiverted);
    void    setDialType(const QString& dialType);

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
    bool    m_Diverted;         //!< Diverted board indicator
    QString m_DialType;         //!< Desired dial representation. Possible values: full, upper, bottom

};

#endif // HORIZONTALBOARD_H