#include "wheeloffortune.h"

#include <cmath>

const int g_DefaultSize = 500;
extern const Color g_HighlightRimColor("#FFFEC8"); // TODO: Cross-check that.
constexpr float g_Epsilon = 0.0001f;
constexpr float g_FullRotation = 360.0f;

WheelOfFortune::WheelOfFortune()
{
	m_Size = { g_DefaultSize, g_DefaultSize };
	m_Result = nullptr;
	m_Ready = nullptr;
	m_Rotation = 0;
	m_PieAngle = 0;
	m_Highlight = false;
}

void WheelOfFortune::setPositions(const vector<Entry>& entries, int duplicationsAmount, float pieAngle)
{
	m_Positions.clear();
	if (duplicationsAmount)
	{
		m_Positions.reserve(entries.size() * duplicationsAmount);
		for (int i = 0; i < duplicationsAmount; i++)
			for (const auto& entry : entries)
				m_Positions.push_back(entry);
	}
	else
		m_Positions = vector<Entry>(entries);

	m_Rotation = 0;
	m_PieAngle = pieAngle;
}

void WheelOfFortune::increaseRotation(uint step)
{
	m_Rotation = (m_Rotation + step) % 360;
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
		drawBoardContour();
		drawBoardCenter();
		drawPointer();
	}
}

void WheelOfFortune::drawWheel()
{
	if (m_Positions.empty())
		return;

	int resultIndex = -1;
	for (int i = 0; i < m_Positions.size(); i++)
	{
		if (m_Highlight)
		{
			float rotationAngle = static_cast<float>(m_Rotation) + m_PieAngle * static_cast<float>(i);
			rotationAngle = fmod(rotationAngle, g_FullRotation);

			if (rotationAngle <= m_PieAngle / 2.f + g_Epsilon ||
				rotationAngle >= g_FullRotation - m_PieAngle / 2.f - g_Epsilon)
			{
				m_Result = &m_Positions[i].m_LabelName;
				resultIndex = i;
				continue;
			}
		}

		const Entry& position = m_Positions.at(i);
		drawPiePiece(position.m_LabelName, position.m_Color, m_Rotation + m_PieAngle * i);
	}

	if (m_Highlight)
		if (0 <= resultIndex && resultIndex < m_Positions.size())
			drawPiePiece(m_Positions.at(resultIndex).m_LabelName, m_Positions.at(resultIndex).m_Color, m_Rotation + m_PieAngle * resultIndex, true);
}

void WheelOfFortune::highlightPie()
{
	m_Highlight = true;
}

void WheelOfFortune::resetHighlight()
{
	m_Highlight = false;
}
