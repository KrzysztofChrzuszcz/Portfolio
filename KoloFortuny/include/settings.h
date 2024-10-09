#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
using std::string;

typedef unsigned int uint;

/*
enum class Entropy // ?? moze po prostu RandMethod / RandGenerator
{
	stdRand = 0,
	linear = 1,
	minstd_rand0 = 2,
	random_device = 3, // True random number generator
https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
};
*/
class	CustomOpenGlWidget;

class Settings
{
	friend class	MainWindow; // Writer
	friend class	Engine; // Reader/Writer
	friend class	CustomOpenGlWidget; // Reader

public:
					Settings();
	void			Update();
	float			getMinColorBrightness() inline const { return m_MinColorBrightness; }

private:
	bool			m_DataSelected;
	bool			m_DataProcessed;
	mutable bool	m_DataReady; /// keyword mutable allows CustomOpenGlWidget::bindWithSettings to avoid using const_cast
	bool			m_DrawLots;
	bool			m_AutoStart; // TODO: checkbox
	bool			m_AutoAdjust; // TODO: checkbox
	string			m_FilePath;
	float			m_MinColorBrightness; // TODO: slider
	uint			m_ScreenRefreshFrequency; // TODO: combo box 50 55 60
	int				m_MinAngle; // https://stackoverflow.com/questions/17361885/range-slider-in-qt-two-handles-in-a-qslider / https://github.com/ThisIsClark/Qt-RangeSlider
	int				m_MaxAngle; // super / range slider for both BUT In first version two sliders
	int				m_MaxPositionsAmount; 

	//int				m_MinRandRange;
	//int				m_MaxRandRange; // super / range slider for both
	//Entropy/int         m_RandomEntropy; // combo box
};
#endif //SETTINGS_H