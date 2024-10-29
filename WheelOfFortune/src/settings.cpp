#include "settings.h"

Settings::Settings()
{
	m_ScreenRefreshFrequencies = { 24, 30, 50, 55, 60 };
	m_ScreenRefreshFrequencyIndex = 2;

	m_MinColorBrightness = 0.23f;

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

	m_RandomMathod = RandMethod::standart;
}

void Settings::setFilePath(string fileName)
{
	WriteLock wLock(m_Lock);
	m_FilePath = fileName;
	m_DataSelected = true;
	m_DataProcessed = false;
}

void Settings::drawLots()
{
	WriteLock wLock(m_Lock);
	m_DrawLots = true;
}
