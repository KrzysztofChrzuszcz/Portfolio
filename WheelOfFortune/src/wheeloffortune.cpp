#include "wheeloffortune.h"

#include <cmath>

const int g_DafaultSize = 500;
extern const Color g_HighlightRimColor("#FFFEC8");

WheelOfFortune::WheelOfFortune()
{
	m_Size = { g_DafaultSize, g_DafaultSize };
	m_Result = nullptr;
	m_Ready = nullptr;
	m_Rotation = 0;
	m_PieAngle = 0;
	m_Highlight = false;
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
	int resultIndex = -1;
	for (int i = 0; i < m_Positions.size(); i++)
	{
		if (m_Highlight)
		{
			float rotationAngle = static_cast<float>(m_Rotation) + m_PieAngle * static_cast<float>(i);
			if (rotationAngle > 360)
				rotationAngle -= 360.f;

			if (rotationAngle <= m_PieAngle / 2.f || rotationAngle >= 360.f - m_PieAngle / 2.f)
			{
				m_Result = &m_Positions[i].m_LabelName;
				resultIndex = i;
				continue;
			}
		}

		drawPiePiece(m_Positions.at(i).m_LabelName, m_Positions.at(i).m_Color, m_Rotation + m_PieAngle * i);
	}

	if (m_Highlight)
		if (0 <= resultIndex && resultIndex < m_Positions.size())
			drawPiePiece(m_Positions.at(resultIndex).m_LabelName, m_Positions.at(resultIndex).m_Color, m_Rotation + m_PieAngle * resultIndex, true);
}

void WheelOfFortune::highlightPie()
{
	m_Highlight = true;
}

void WheelOfFortune::resetHighLight()
{
	m_Highlight = false;
}
