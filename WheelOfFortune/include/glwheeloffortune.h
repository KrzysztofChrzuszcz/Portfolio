#ifndef GLWHEELOFFORTUNE_H
#define GLWHEELOFFORTUNE_H

#include "wheeloffortune.h"
#include "dataloader.h"

/**
 * \brief GlWheelOfFortune pure OpenGl implementation of main GUI part
 */
class GlWheelOfFortune : public WheelOfFortune
{
public:
                    GlWheelOfFortune();

    void			configureCanvas() override;

    void			drawIcon() override;
    void			drawBoardConture() override;
    void			drawBoardCenter() override;
    void			drawPointer() override;
    void			drawPiePiece(const string& text, const Color& color, float rotationAngle, bool highlight = false);

    void            drawCircleShape(float cx, float cy, float radius, bool fill = false, bool dashed = false, int lineStripFactor = 1);
    void			drawPieShape(float radius, float angle, float rotation, bool drawJustRim = false);
    void			drawLabel(float radiusRatio, float rotationAngle, const string& name, float lineWidth, bool highlight = false);
};

#endif //GLWHEELOFFORTUNE_H