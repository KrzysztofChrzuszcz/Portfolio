#ifndef COLOR_H
#define COLOR_H

#include "global.h"

#include <string>
#include <exception>
#include <stdexcept>
#include <type_traits>

#include <QColor>

using std::string;

/// Examples of different exceptions
class WrongInputException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Color input not recognized";
    }
};

class ChannelOutOfRangeException : public std::runtime_error
{
public:
    ChannelOutOfRangeException() : runtime_error("") {}

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

template <typename T>
struct IColor
{
    static_assert(std::is_arithmetic_v<T>, "T must be a numeric type!");

    T     m_R;
    T     m_G;
    T     m_B;
    T     m_A;
    //  Rule of Zero
};

//typedef IColor<int> ColorI; // for pre C++11 change to this
using ColorI = IColor<int>;

// Provides maximal precision
struct ColorF : public IColor<float>
{
    // Rule of Five
    ColorF() { m_R = 0.5f; m_G = 0.5f; m_B = 0.5f; m_A = 1.0f; }

    ColorF(float r, float g, float b, float a = 1.0f) : IColor<float>{ r, g, b, a } {}

    ~ColorF() = default;

    ColorF(const ColorF& other) = default;

    ColorF& operator=(const ColorF& other) = default;

    ColorF(ColorF&& other) noexcept = default;

    ColorF& operator=(ColorF&& other) noexcept = default;

    ColorF operator+(float const& skalar)
    {
        ColorF result;
        result.m_R = (m_R + skalar) > 1.f ? 1.f : (m_R + skalar);
        result.m_G = (m_G + skalar) > 1.f ? 1.f : (m_G + skalar);
        result.m_B = (m_B + skalar) > 1.f ? 1.f : (m_B + skalar);
        result.m_A = m_A;

        return result;
    }

    ColorF& operator*=(float const& skalar)
    {
        m_R = (m_R * skalar) > 1.f ? 1.f : (m_R * skalar);
        m_G = (m_G * skalar) > 1.f ? 1.f : (m_G * skalar);
        m_B = (m_B * skalar) > 1.f ? 1.f : (m_B * skalar);

        return *this;
    }
};

/**
 * \brief Color is universal class to read and parse input color.
 * The class is able to throw exceptions that prevents creating instance.
 * 
 * \note
 * Supported input RGB formating: hex (ex. #FF00FF) ; csv RGB  (ex. 255,0,255) ; by percent/factor (ex. 1.0,0.0,1.0) ; by name (ex. "Magenta")
 * Please notice! CYMK, HLS, HSV, and names out of basic colors list are not supported.
 */
class Color
{
public:
                    Color(string rawColor);

    string          getHex();       //!< Return hexadecimal color representation
    ColorI          getInt() const; //!< Return instance of ColorI
    const ColorF&   getFloat() const { return m_ColorF; } //!< Return reference to ColorF member
    QColor          getQColor() const; //!< Return instance of ColorQ
    float           getBrightness() const; //!< Return human friendly brightness of the color
    inline bool     isBrighten() const { return m_IsBrighten; } //!< Gives information if color has adjusted brightness
    void            adjustBrightness(float minLevel); //!< Adjust color to reach minimum color brightness level

private:
    bool            isHex();        //!< Checks if raw color is written in hexadecimal representation
    bool            isText();       //!< Checks if raw color is written in named text representation
    bool            isKnown();      //!< Checks if raw color is known
    bool            isIntForm();    //!< Checks if raw color is written in integer RGBA form
    bool            isFloatForm();  //!< Checks if raw color is written in float RGBA form

    void            parse(); 
    void            parseHex();     //!< Parse raw hexadecimal color to float RGBA form
    void            parseInt();     //!< Parse raw integer color to float RGBA form
    void            parseFloat();   //!< Parse raw float color to float RGBA form
    void            parseText();    //!< Parse raw color from SVG color names to float RGBA form

private:
    const string    m_RawColor;     //!< Color in raw form reread from file
    ColorF          m_ColorF;       //!< Parsed color in float RGBA form (Can be adjusted)
    bool            m_IsBrighten;   //!< Marks if color was brighten in result of adjustBrightness method
};

const Color g_DafaultColor(string("0.5,0.5,0.5,1.0"));

#endif //COLOR_H