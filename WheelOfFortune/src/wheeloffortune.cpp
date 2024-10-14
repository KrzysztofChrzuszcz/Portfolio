#include "wheeloffortune.h"

#include <cmath>

const int g_DafaultSize = 500;

WheelOfFortune::WheelOfFortune()
{
	m_Size = { g_DafaultSize, g_DafaultSize };
	m_Result = nullptr;
	m_Ready = nullptr;
	m_Rotation = 0;
	m_PieAngle = 0;
}

void WheelOfFortune::setupPositions(const vector<Entry>& entries, int duplicationsAmount, float pieAngle)
{
	m_Positions.clear();
	if(duplicationsAmount)
		for (int i = 0; i < duplicationsAmount; i++)
			for (auto entry : entries)
				m_Positions.push_back(entry);
	else
		m_Positions = vector<Entry>(entries);

	m_Rotation = 0;
	m_PieAngle = pieAngle;
}

void WheelOfFortune::increaseRotation(uint step)
{
	m_Rotation += step;
	if (m_Rotation > 360)
		m_Rotation -= 360;
}

void WheelOfFortune::paintGL(const Size& size)
{
	m_Size = size;

	if (m_Ready && !*m_Ready) // if (m_Ready != nullptr && *m_Ready == false)
	{
		// Start view
		drawIcon();
	}
	else
	{
		// Canvas configuration
		configureCanvas();

		// Main part of frontend
		drawWheel();

		// Decorations
		drawBoardConture();
		drawBoardCenter();
		drawPointer();
	}
}

void WheelOfFortune::drawWheel()
{
	for (int i = 0; i < m_Positions.size(); i++)
		drawPiePiece(m_Positions.at(i).m_LabelName, m_Positions.at(i).m_Color, m_Rotation + m_PieAngle * i);
	// TODO: (mozliwosc refaktoryzacji) klasa moglaby posiadac boola m_HighlightResult. Ten for jesli zmienna byla by aktywna to sprawdzilby warunek z WheelOfFortune::hightlightPie() i ustawic w tej funkcji podswietlenei dla rezultatu
	// TODO: Possible refactorization variant: 
	/*
	for (int i = 0; i < m_Positions.size(); i++)
	{
		bool isResult = false;
		if (m_HighlightResult)
		{
			float piePosition = static_cast<float>(static_cast<int>(m_Rotation + m_PieAngle * i + m_PieAngle / 2.0) % 360);

			if (piePosition < m_PieAngle / 2.f || piePosition > 360.f - m_PieAngle / 2.f)
			{
				m_Result = &m_Positions[i].m_LabelName;
				isResult = true;
			}
		}
		
		drawPiePiece(m_Positions.at(i).m_LabelName, m_Positions.at(i).m_Color, m_Rotation + m_PieAngle * i, isResult);// Tyle ze musialby miec blizsze Z by nastepny w kolejce nie narysowal sie na czesci wspolnej
	}
	*/
	// NOTE: ZLE! Powinno najpierw narysowac wszystkie inne, dla podswietlonego pominac i zapisac jego indeks. Nastepnie osobny drawPiePiece dla pominietej pozycji. Wtedy Z nie jest istotne i bardziej wpasowuje sie w koncept 2D.
}

void WheelOfFortune::highlightPie() // to powinna byc czesc drawWheel() ktory powinien dostac dodatkowa flage do drawPiePiece // TODO: Przegadac te koncepcje z ChatGPT i CodeRabbit
{
	// TODO
	for (int i = 0; i < m_Positions.size(); i++)
	{
		float piePosition = static_cast<float>(static_cast<int>(m_Rotation + m_PieAngle * i + m_PieAngle / 2.0) % 360);

		if (piePosition < m_PieAngle / 2.f || piePosition > 360.f - m_PieAngle / 2.f)
			m_Result = &m_Positions[i].m_LabelName;
			//drawPiePiece(m_Positions.at(i).m_LabelName, m_Positions.at(i).m_Color, m_Rotation + m_PieAngle * i, true);
			// TODO 
			// white-yellow rim (obwodka)
			// bold label font
	}
}
