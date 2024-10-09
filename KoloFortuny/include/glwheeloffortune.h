#ifndef GLWHEELOFFORTUNE_H
#define GLWHEELOFFORTUNE_H

#include "wheeloffortune.h"
#include "dataloader.h"

class GlWheelOfFortune : public WheelOfFortune
{
public:
					GlWheelOfFortune();
	void			configureCanvas() override;

	void			drawIcon() override;
	void			drawBoardConture() override;
	void			drawBoardCenter() override;
	void			drawPointer() override;
	void			drawPiePiece(const string& text, const Color& color, float rotationAngle);

	// TODO hightlightPie() // Podswietla : dodaje bialo-zolta obwodke; Zmiania text:font na bold
	// resetHighLight() // zeruje powyzszy efekt

	void            drawCircleShape(float cx, float cy, float radius, bool fill = false, bool dashed = false, int lineStripFactor = 1);
	void			drawPieShape(float radius, float angle, float rotation);
	void			drawLabel(float x, float y, float ange, const string& name, float lineWidth);
};

#endif //GLWHEELOFFORTUNE_H