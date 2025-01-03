﻿#include "engine.h"
#include "customopenglwidget.h"

#include <cmath>
#include <stdexcept>

const double M_PHI = (1.0 + sqrt(5)) / 2.0;
const int g_MinimumDurationTime = 8;
const int g_MaxDuplications = 24; // = 360 / 15 (minimal pie angle range in settings)

Engine::Engine(MainWindow& mainWindow, DataLoader& dataLoader):
	m_MainWindow(mainWindow),
	m_Settings(mainWindow.getSettings()),
	m_DataLoader(dataLoader),
	m_WheelOfFortune(nullptr),
	m_Stage(Stage::Idle),
	generateRandData(nullptr)
{
	m_CurrentAngle = 0;
	m_FastAnimationAngle = 0;
	m_SlowingDownAngle = 0;
	m_Step = 0;

	m_OpenGlWidget = mainWindow.getWidget();
	if (!m_OpenGlWidget.expired())
		m_WheelOfFortune = (*m_OpenGlWidget.lock()).getGui();
}

Engine::~Engine()
{
}

void Engine::run()
{
	switch (m_Stage)
	{
		case Stage::Idle:
		{
			waitForOrder();
			break;
		}
		case Stage::DataLoading:
		{
			loadData();
			break;
		}
		case Stage::Processing:
		{
			processData();
			break;
		}
		case Stage::FortuneDraw:
		{
			fortuneDraw();
			break;
		}
		case Stage::Animation:
		{
			animate();
			break;
		}
		case Stage::SlowingDown:
		{
			animationExtinguishing();
			break;
		}
		case Stage::SumUp:
		{
			sumUp();
			break;
		}
	}
}

void Engine::setRandomMethod(int index)
{
	switch (index)
	{
		case 0:
			generateRandData = &Engine::generateRandDataWithStandardRand;
			break;
		case 1:
			generateRandData = &Engine::generateRandDataTemplate<std::knuth_b>;
			break;
		case 2:
			generateRandData = &Engine::generateRandDataTemplate<std::minstd_rand>;
			break;
		case 3:
			generateRandData = &Engine::generateRandDataTemplate<std::ranlux24>;
			break;
		case 4:
			generateRandData = &Engine::generateRandDataTemplate<std::mt19937>;
			break;
		case 5:
			generateRandData = &Engine::generateRandDataTemplate<std::subtract_with_carry_engine<unsigned, 24, 10, 24>>; //  unsigned – the type of random numbers generated by the generator. | 24 – the number of bits in the random number. | 10 – the number of bits used as the window size. | 24 – the number of bits representing the buffer.
			break;
		default:
			throw std::out_of_range("Invalid RandGenerator value");
	}
}

uint Engine::getScreenRefreshFrequency()
{
	ReadLock rLock(m_Settings.m_Lock);
	return m_Settings.m_ScreenRefreshFrequencies[m_Settings.m_ScreenRefreshFrequencyIndex];
}

void Engine::changeState(Stage newState)
{
	m_Stage = newState;
}

bool Engine::validate()
{
	int entriesAmount = m_DataLoader.getEntriesCount();
	int maxPositionsAmount = 360.f / m_Settings.m_MinAngle;

	// Check entries amount 
	/// There is only point to visualise at least 2 positions and it is not possible to visualize more then max amount with given min pie angle range setting
	if (entriesAmount <= 1 || entriesAmount > maxPositionsAmount)
		m_DataLoader.setErrorFlag(3);

	// Check pie angle range
	float pieAngle = 360.f / entriesAmount;
	bool anglePossibleToVisualise = false;
	for (int duplications = 1; duplications < g_MaxDuplications; duplications++)
	{
		float consideredPieAngle = pieAngle / duplications;

		if (consideredPieAngle >= m_Settings.m_MinAngle && consideredPieAngle <= m_Settings.m_MaxAngle)
		{
			anglePossibleToVisualise = true;
			break;
		}
	}

	if (!anglePossibleToVisualise)
		m_DataLoader.setErrorFlag(3);

	return !m_DataLoader.isCorrupted();
}

void Engine::generateRandDataWithStandardRand(double& randomAngle, int& durationInSeconds)
{
	ReadLock rLock(m_Settings.m_Lock);

	std::srand(time(NULL));

	if (m_Settings.m_MaxRandRange != m_Settings.m_MinRandRange)
		randomAngle = static_cast<double>(std::rand() % (m_Settings.m_MaxRandRange - m_Settings.m_MinRandRange) + m_Settings.m_MinRandRange);
	else
		randomAngle = m_Settings.m_MinRandRange;

	durationInSeconds = g_MinimumDurationTime + std::rand() % m_Settings.m_MaxDurationTime;
}

void Engine::loadData()
{
	WriteLock wLock(m_Settings.m_Lock);

	if (m_DataLoader.loadXml(m_Settings.m_FilePath.c_str()) && validate())
	{
		m_Settings.m_DataState = DataState::Loaded;
		if (m_Settings.m_AutoAdjust)
			m_DataLoader.adjust(m_Settings);

		changeState(Stage::Processing);
	}
	else
	{
		if (m_DataLoader.isCorrupted())
			m_MainWindow.alarmLoadingDataError(m_DataLoader.getErrorFlags());

		m_Settings.m_DataState = DataState::NotSelected;
		changeState(Stage::Idle);
	}
}

void Engine::processData()
{
	WriteLock wLock(m_Settings.m_Lock);

	const vector<Entry>& entries = m_DataLoader.getEntries();
	int noOfVatiants = entries.size();
	float basicPieAngle = -1;
	if (noOfVatiants > 0)
		basicPieAngle = 360.f / noOfVatiants;

	float optimalPieAngle = basicPieAngle;

	/// Find smallest possible angle in given range
	int duplicationsAmount = 0;
	for (int divisor = 2; divisor <= g_MaxDuplications; ++divisor)
	{
		float consideredPieAngle = basicPieAngle / divisor;

		if (duplicationsAmount > 0)
			if (!(consideredPieAngle >= m_Settings.m_MinAngle && consideredPieAngle <= m_Settings.m_MaxAngle))
				break;

		if (consideredPieAngle >= m_Settings.m_MinAngle && consideredPieAngle <= m_Settings.m_MaxAngle)
		{
			optimalPieAngle = consideredPieAngle;
			duplicationsAmount = divisor;
			// In range is wide maybe there is smaller possible pie angle
		}
	}

	if (m_WheelOfFortune)
		m_WheelOfFortune->setPositions(entries, duplicationsAmount, optimalPieAngle);
	if (!m_OpenGlWidget.expired())
		(*m_OpenGlWidget.lock()).update();

	m_Settings.m_DataState = DataState::Ready;
	
	if (m_Settings.m_AutoStart)	// NOTE: m_AutoStart in settings should only change m_Settings.m_DrawLots on true
		changeState(Stage::FortuneDraw);
	else
		changeState(Stage::Idle);
}

void Engine::waitForOrder()
{
	WriteLock wLock(m_Settings.m_Lock);

	if(m_Settings.m_DataState == DataState::Selected)
	{
		if (m_WheelOfFortune) // TODO: clear later (after Task9)
			m_WheelOfFortune->resetHighlight();

		changeState(Stage::DataLoading);
	}
	if (m_Settings.m_DataState == DataState::Ready && m_Settings.m_DrawLots)
	{
		if (m_WheelOfFortune) // TODO: clear later (after Task9)
			m_WheelOfFortune->resetHighlight();
		m_Settings.m_DrawLots = false;
		changeState(Stage::FortuneDraw);
	}
}

void Engine::fortuneDraw()
{
	setRandomMethod(static_cast<int>(m_Settings.m_RandomGenerator));

	double randomAngle;
	int durationInSeconds;

	if (generateRandData == nullptr)
	{
		changeState(Stage::Idle);
		throw std::out_of_range("Invalid RandGenerator value");
		// TODO: Log
		// return;
	}

	(this->*generateRandData)(randomAngle, durationInSeconds);

	double goldenRatio = 1.f / M_PHI;
	m_FastAnimationAngle = randomAngle * goldenRatio;
	m_SlowingDownAngle = randomAngle * ( 1.f - goldenRatio);

	m_Step = randomAngle * getScreenRefreshFrequency() / durationInSeconds / 1000.0; // step angle per millisecond

	m_CurrentAngle = m_SlowingDownAngle; /// reset sign for local static variable inside Engine::animationExtinguishing()
	changeState(Stage::Animation);
}

void Engine::animate()
{
	if (m_FastAnimationAngle - m_Step >= 0)
	{
		m_FastAnimationAngle = m_FastAnimationAngle - m_Step;
		if (m_WheelOfFortune)
			m_WheelOfFortune->increaseRotation(m_Step);
	}
	else
		changeState(Stage::SlowingDown);

	if (!m_OpenGlWidget.expired())
		(*m_OpenGlWidget.lock()).update();
}

void Engine::animationExtinguishing()
{
	/// Because there is only one instance of engine I could present here unusual usage of local static variable.
	static int changeAngle; // angle on which step value decrease
	if (m_SlowingDownAngle == m_CurrentAngle)
		changeAngle = m_SlowingDownAngle / (m_Step - 1);

	if (m_Step > 0 && m_CurrentAngle / changeAngle == m_Step - 2)
		--m_Step;

	if (m_CurrentAngle - m_Step >= 0)
	{
		m_CurrentAngle -= m_Step;
		if (m_WheelOfFortune)
			m_WheelOfFortune->increaseRotation(m_Step);
		if (!m_OpenGlWidget.expired())
			(*m_OpenGlWidget.lock()).update();
	}
	else
		changeState(Stage::SumUp);
}

void Engine::sumUp()
{
	// TODO: TASK 9
	// Add label(wordart) with result, slowly disappearing after time; confetti gif, sound effect (fanfare)
	if (m_WheelOfFortune)
	{
		m_WheelOfFortune->highlightPie();
		if (!m_OpenGlWidget.expired())
			(*m_OpenGlWidget.lock()).update();
	}
	// IDEA
	// Class Timestamp
	// Timestamp ticket1 = now() + sumUpSomeAnimationTime; // play gif, show wordart, slowly hid wordart, timeout/finishTime/changeStateTime, maybe play sound effect time

	//if (isEnclapsed())
	//if (m_WheelOfFortune)
	//	m_WheelOfFortune->resetHighLight();
	changeState(Stage::Idle);
}
