#include "settings.h"

Settings::Settings()
{
	m_MinColorBrightness = 0.23f;
	m_ScreenRefreshFrequency = 50;

	m_DataSelected = false;
	m_DataProcessed = false;
	m_DataReady = false;
	
	m_AutoStart = false;
	m_DrawLots = false;
	//m_DrawLots = m_AutoStart; // other option on auto start after selecting file (with additional idle state between) REMEMBER in that case to always set it too after file choose
	m_AutoAdjust = true;

	m_MinAngle = 15;
	m_MaxAngle = 30;
	m_MaxPositionsAmount = 360.f / m_MinAngle;

	m_MaxDurationTime = 12;

	m_MinRandRange = 1500;
	m_MaxRandRange = 3000;

	m_RandomEntropy = RandMethod::standart;
}

void Settings::update()
{
	// TODO
}
