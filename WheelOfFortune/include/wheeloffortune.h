﻿#ifndef WHEELOFFORTUNE_H
#define WHEELOFFORTUNE_H

#include "dataloader.h"
#include "settings.h"

#include <string>
#include <vector>

typedef	unsigned int uint;

/**
 * \brief WheelOfFortune interface of main GUI part. 
 * Providing universal usage of GUI regardless of used library to implement it.
 */
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

    virtual void		setupPositions(const vector<Entry>& entries, int duplicationsAmount, float pieAngle) final; //!< Setup positions to display on the wheel within all necessary information
    virtual void		increaseRotation(uint step) final;
    virtual void		configureCanvas() = 0; //!< Configure basic setup of canvas necessary for used OpenGL approach

    virtual void		paintGL(const Size& size) final; //!< Main and final function to visualize wheel of fortune

    virtual void		drawIcon() = 0;         //!< Draw start screen. That is wheel of fortune image used also as program icon.
    virtual void		drawBoardConture() = 0; //!< Draw outer decorations
    virtual void		drawBoardCenter() = 0;  //!< Draw inner decorations
    virtual void		drawPointer() = 0;
    virtual void		drawPiePiece(const string& text, const Color& color, float rotationAngle) = 0; //!< Draw a position as pie piece on the wheel
    // TODO: virtual void		drawPiePiece(const string& text, const Color& color, float rotationAngle, bool highlight = false) = 0;
    virtual void		drawWheel();            //!< Draw all pie pieces as whole wheel

    void highlightPie();						//!< Highlight winning position
    // resetHighLight();						//!< Cancel winning piece highlight // zeruje powyzszy efekt || w nowszej wersji zeruje flage

public:
    bool*				m_Ready;		//!< Link to extern information of finished preparations to start animation

protected:
    Size                m_Size;         //!< Size of canvas to present maximized wheel of fortune with decorations.
    std::string*        m_Result;       //!< Result of the lottery.
    std::vector<Entry>  m_Positions;    //!< Variants of wheel of fortune.
    float               m_PieAngle;     //!< Angle of pie piece with a variant from possible positions.
    uint                m_Rotation;     //!< Current rotation of wheel of fortune in compare to its start position.
};

#endif //WHEELOFFORTUNE_H