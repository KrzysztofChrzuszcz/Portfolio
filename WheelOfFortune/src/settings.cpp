#include "settings.h"

Settings::Settings()
{
	m_ScreenRefreshFrequencies = { 24, 30, 50, 55, 60 };
	m_ScreenRefreshFrequencyIndex = 2;

	m_MinColorBrightness = 0.23f;

	
	m_AutoStart = false;
	m_DrawLots = false;
	//m_DrawLots = m_AutoStart; // other option on auto start after selecting file (with additional idle state between) REMEMBER in that case to always set it too after file choose
	m_AutoAdjust = true;

	m_MinAngle = 20;
	m_MaxAngle = 30;

	m_MaxDurationTime = 12;

	m_MinRandRange = 2500;
	m_MaxRandRange = 3000;

	m_RandomGenerator = RandGenerator::standard;
	m_DataState = DataState::NotSelected;

	updateLastChangeTime();
}

bool Settings::isDataReady() const noexcept
{
	return m_DataState == DataState::Ready;
}

void Settings::setFilePath(string fileName)
{
	if (fileName.empty())
		return;

	/// It is forbidden to modify state and name in consequence during data validation and processing
	if (m_DataState == DataState::NotSelected || m_DataState == DataState::Ready)
	{
		WriteLock wLock(m_Lock);
		m_FilePath = fileName;
		m_DataState = DataState::Selected;
	}
}

void Settings::drawLots()
{
	WriteLock wLock(m_Lock);
	m_DrawLots = true;
}

void Settings::updateLastChangeTime()
{
	auto now = std::chrono::system_clock::now();
	m_LastChangeTime = std::chrono::system_clock::to_time_t(now);
}

std::time_t Settings::getTimestamp() const
{
	return m_LastChangeTime;
}

bool Settings::hasChanged(std::time_t since)
{
	return since != m_LastChangeTime;
}
