#ifndef COLOR_H
#define COLOR_H

#include "global.h"

#include <string>
#include <exception>
#include <stdexcept>
#include <QColor>

using std::string;

/// Examples of different exceptions
class WrongInputException : public std::exception
{
public:
	char* what() 
	{
		return "Color input not recognized";
	}
};

class ChanelOutOfRangeException : public std::runtime_error
{
public:
	ChanelOutOfRangeException() : runtime_error("") {}

	const char* what() const override
	{
		return "Channel value out of range";
	}
};

class WrongChannelAmountException : public std::logic_error
{
public:
	WrongChannelAmountException() : logic_error("") {}

	const char* what() const override 
	{
		return "Channels amount is not correct";
	}
};

//provides maximum precision
struct ColorF
{
	float	m_R;
	float	m_G;
	float	m_B;
	float	m_A;

	ColorF operator+(float const& skalar)
	{
		ColorF result;
		result.m_R = m_R + skalar > 1.f ? 1.f : m_R + skalar;
		result.m_G = m_G + skalar > 1.f ? 1.f : m_G + skalar;
		result.m_B = m_B + skalar > 1.f ? 1.f : m_B + skalar;

		return result;
	}

	ColorF operator*=(float const& skalar)
	{
		ColorF result;
		result.m_R = m_R * skalar > 1.f ? 1.f : m_R * skalar;
		result.m_G = m_G * skalar > 1.f ? 1.f : m_G * skalar;
		result.m_B = m_B * skalar > 1.f ? 1.f : m_B * skalar;

		return result;
	}
};

struct ColorI
{
	int		m_R;
	int		m_G;
	int		m_B;
	int		m_A;
};

// TODO: dodac tez ten opis do readme i do doxygena
// Supported input RGB formating: hex (ex. #FF00FF) ; csv RGB  (ex. 255,0,255) ; by percent/factor (ex. 1.0,0.0,1.0) ; by name (ex. "Magenta")
// Please notice! CYMK, HLS, HSV, and names out of Qt basic colors enum list are not supported.

class Color
{
public:
					Color(string rawColor);

	string			getHex();
	ColorI			getInt() const;
	ColorF&			getFloat() inline const { return const_cast<ColorF&>(m_ColorF); }
	QColor			getQColor() const;
	float			getBrightness() const;
	bool			isBrighten() inline const { return m_IsBrighten; }
	void			adjustBrightness(float minLevel);

private:
	bool			isHex();
	bool			isText(); // or IsColorName()
	bool			isKonwn();
	bool			isIntNo(); /// No. stands for Notation
	bool			isFloatNo();

	void			parse(); 
	void			parseHex();
	void			parseInt();
	void			parseFloat();
	void			parseText();

private:
	string			m_RawColor;
	ColorF			m_ColorF;
	bool			m_IsBrighten;
};

#endif //COLOR_H