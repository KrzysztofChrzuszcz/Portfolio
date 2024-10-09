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
	//m_DrawLots = m_AutoStart; // inna opcja automatycznego wyzwolenia, po wyborze pliku, ALE trzeba by tez to ustawiac po wyborze pliku
	m_AutoAdjust = true;

	m_MinAngle = 15;
	m_MaxAngle = 30;
	m_MaxPositionsAmount = 360.f / m_MinAngle;
}

void Settings::Update()
{
	// TODO
}
