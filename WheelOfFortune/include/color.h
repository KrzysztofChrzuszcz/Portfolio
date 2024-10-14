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
        result.m_A = m_A;

        return result;
    }

    ColorF operator*=(float const& skalar)
    {
        ColorF result;
        result.m_R = m_R * skalar > 1.f ? 1.f : m_R * skalar;
        result.m_G = m_G * skalar > 1.f ? 1.f : m_G * skalar;
        result.m_B = m_B * skalar > 1.f ? 1.f : m_B * skalar;
        result.m_A = m_A;

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


/**
 * \brief Color is universal class to read and parse input color.
 * The class is able to throw exceptions that prevents creating instance.
 * 
 * \note
 * Supported input RGB formating: hex (ex. #FF00FF) ; csv RGB  (ex. 255,0,255) ; by percent/factor (ex. 1.0,0.0,1.0) ; by name (ex. "Magenta")
 * Please notice! CYMK, HLS, HSV, and names out of Qt basic colors enum list are not supported.
 */
class Color
{
public:
                    Color(string rawColor);

    string			getHex(); //!< Return hexadecimal color representation
    ColorI			getInt() const; //!< Return instance of ColorI
    ColorF&			getFloat() inline const { return const_cast<ColorF&>(m_ColorF); } //!< Return reference to ColorF member
    QColor			getQColor() const; //!< Return instance of ColorQ
    float			getBrightness() const; //!< Return human friendly brightness of the color
    bool			isBrighten() inline const { return m_IsBrighten; } //!< Gives information if color has adjusted brightness
    void			adjustBrightness(float minLevel); //!< Adjust color to reach minimum color brightness level

private:
    bool			isHex();		//!< Checks if raw color is written in hexadecimal representation
    bool			isText();		//!< Checks if raw color is written in named text representation
    bool			isKonwn();		//!< Checks if raw color is known
    bool			isIntForm();    //!< Checks if raw color is written in integer RGBA form
    bool			isFloatForm();  //!< Checks if raw color is written in float RGBA form

    void			parse(); 
    void			parseHex();		//!< Parse raw hexadecimal color to float RGBA form
    void			parseInt();		//!< Parse raw integer color to float RGBA form
    void			parseFloat();   //!< Parse raw float color to float RGBA form
    void			parseText();    //!< Parse raw color from SVG color names to float RGBA form

private:
    string			m_RawColor;		//!< Color in raw form reread from file
    ColorF			m_ColorF;		//!< Parsed color in float RGBA form
    bool			m_IsBrighten;   //!< Marks if color was brighten in result of adjustBrightness method
};

#endif //COLOR_H