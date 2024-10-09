#ifndef WHEELOFFORTUNE_H
#define WHEELOFFORTUNE_H

#include "dataloader.h"
#include "settings.h"

#include <string>
#include <vector>

typedef	unsigned int uint;

class WheelOfFortune
{
public:
	struct Size
	{
		int m_Width;
		int m_Height;
	};

public:
						WheelOfFortune();
	virtual				~WheelOfFortune() = default;

	virtual void		setupPositions(const vector<Entry>& entries, int duplicationsAmount, float pieAngle) final;
	virtual void		increaseRotation(uint step) final;
	virtual void		configureCanvas() = 0;

	virtual void		paintGL(const Size& size) final;

	virtual void		drawIcon() = 0;
	virtual void		drawBoardConture() = 0;
	virtual void		drawBoardCenter() = 0;
	virtual void		drawPointer() = 0;
	virtual void		drawPiePiece(const string& text, const Color& color, float rotationAngle) = 0;
	// TODO: virtual void		drawPiePiece(const string& text, const Color& color, float rotationAngle, bool highlight = false) = 0;
	virtual void		drawWheel();

	// TODO 
	void hightlightPie();  // Podswietla : dodaje bialo-zolta obwodke; Zmiania text:font na bold
	// resetHighLight() // zeruje powyzszy efekt || w nowszej wersji zeruje flage

public:
	bool*				m_Ready;

protected:
	Size				m_Size;			/// Size of canvas to present maximized wheel of fortune with decorations.
	std::string*		m_Result;		/// Result of the lottery.
	std::vector<Entry>	m_Positions;	/// Variants of wheel of fortune.
	float				m_PieAngle;		/// Angle of pie piece with a variant from possible positions.
	uint				m_Rotation;		/// Current rotation of wheel of fortune in compare to its start position.
};

#endif //WHEELOFFORTUNE_H