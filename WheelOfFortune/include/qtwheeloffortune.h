#ifndef QTWHEELOFFORTUNE_H
#define QTWHEELOFFORTUNE_H

#include "wheeloffortune.h"
#include "dataloader.h"

#include <QObject>
#include <QPainter>

/*!
 * \brief QtWheelOfFortune Qt implementation of main GUI part
 */
class QtWheelOfFortune : public QObject, public WheelOfFortune
{
    Q_OBJECT;
public:
                    QtWheelOfFortune(QPainter& painter);

    void            configureCanvas() override;

    void            drawIcon() override;
    void            drawBoardConture() override;
    void            drawBoardCenter() override;
    void            drawPointer() override;

    void            drawPiePiece(const QString& text, const QColor& color, float rotationAngle, bool highlight = false);
    void            drawPiePiece(const string& text, const Color& color, float rotationAngle, bool highlight = false);

private:
    QPainter&       m_Painter; //!< Connection between GL GUI part and window from Qt Framework
};

#endif //QTWHEELOFFORTUNE_H