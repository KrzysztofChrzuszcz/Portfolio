#include "color.h"
#include <regex>
#include <vector>

using std::vector;

// TODO:
// Dodac opcjonalny kanal Alpha
// Dac dla brakujacego alpha domyslna wartosc 1

extern const Color g_DafaultColor(string("0.5,0.5,0.5,1.0"));

inline vector<string> split(string s, const string& delimiter) /// We would like to have a copy due to not integrate in original input string
{
	vector<string> subStrings;
	string subString;
	size_t position = 0;

	while ((position = s.find(delimiter)) != string::npos) {
		subString = s.substr(0, position);
		subStrings.push_back(subString);
		s.erase(0, position + delimiter.length());
	}
	subStrings.push_back(s);

	return subStrings;
}

Color::Color(string rawColor) :
	m_RawColor(rawColor),
	m_ColorF({ 0.5f,0.5f,0.5f,0.5f })
{
	parse();
}

string Color::getHex()
{
	if (isHex())
		return m_RawColor;

	// TODO: przerobic z m_ColorF na postac #RRGGBBAA
	return m_RawColor;
}

ColorI Color::getInt() const
{
	return ColorI({ 
		static_cast<int>(255 * m_ColorF.m_R), 
		static_cast<int>(255 * m_ColorF.m_G), 
		static_cast<int>(255 * m_ColorF.m_B), 
		static_cast<int>(255 * m_ColorF.m_A) });
}

QColor Color::getQColor() const
{
	return QColor::fromRgbF(m_ColorF.m_R, m_ColorF.m_G, m_ColorF.m_B, m_ColorF.m_A);
}

float Color::getBrightness() const
{
	/// Those "magic numbers" are adjusted to human eye capability to interpret colors and are based on https://www.w3.org/TR/AERT/#color-contrast
	return 0.299f * m_ColorF.m_R + 0.587f * m_ColorF.m_G + 0.114f * m_ColorF.m_B;
}
void Color::adjustBrightness(float minLevel)
{
	// TODO1: rozwazyc validacje minLevel np <0;1>
	// TODO2: Dodac obsluge wyjatku dla czarnego
	// 1. dodac 0.1
	// 2. nastpenie dopiero rozjasnic
	
	float currentLevel = getBrightness();
	if (minLevel > currentLevel)
	{
		m_IsBrighten = true;
		float delta = minLevel - currentLevel;
		// weighted product to fit brightness formula
		m_ColorF.m_R = std::min(1.f, m_ColorF.m_R * (delta / 0.299f + 1.f));
		m_ColorF.m_G = std::min(1.f, m_ColorF.m_G * (delta / 0.587f + 1.f));
		m_ColorF.m_B = std::min(1.f, m_ColorF.m_B * (delta / 0.114f + 1.f));

		// If that will be a little bit not enough then color will by incremented by small value.
		currentLevel = getBrightness();
		if (minLevel > currentLevel)
		{
			m_ColorF = m_ColorF + 0.01;
		}
	}
}

bool Color::isHex()
{
	// TODO dodac obsluge formatu 0xFFFFFFFF
	//std::regex pattern("#([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
	std::regex pattern("#([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{0,2})"); // TODO: poprawic na opcjonalny ostatni segment OBECNIE DOPUSZCZAM #RRGGBBA A MA BYC TYLKO DOPUSZCZONE #RRGGBB LUB #RRGGBBAA
	// TODO : dwie wersja z ? lub z ilosca powtorzen par 3 lub 4 razy
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

bool Color::isText()
{
	if (isHex())
		return false;
	if (isIntNo())
		return false;
	if (isFloatNo())
		return false;
	if (m_RawColor.empty())
		return false;

	return true;
}

bool Color::isKonwn()
{
	// Wersja Qt
	if (QColor::colorNames().contains(QString(m_RawColor.c_str()), Qt::CaseInsensitive))
		return true;

	return false;
}

bool Color::isIntNo() // TODO: Poprawic tak by uwzgkednialo i RGB i RGBA w wariantach 001 jak i 1
{
	//std::regex pattern("([0-9]{1,3},[0-9]{1,3},[0-9]{1,3},[0-9]{1,3})");
	std::regex pattern("([0-9]{1,3},[0-9]{1,3},[0-9]{1,3})");
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

bool Color::isFloatNo()
{
	std::regex pattern("([0-1].[0-9]{1,5},[0-1].[0-9]{1,5},[0-1].[0-9]{1,5},[0-1].[0-9]{1,5})");
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

void Color::parse()
{
	//https://gist.github.com/olmokramer/82ccce673f86db7cda5e

	if (isHex())
	{
		parseHex();
		return;
	}

	if (isIntNo())
	{
		parseInt();
		return;
	}

	if (isFloatNo())
	{
		parseFloat();
		return;
	}

	if (isText() && isKonwn())
	{
		parseText(); // TODO1: z uzyciem Qt ; TODO2: z wlasna lista kolorow
		return;
	}

	throw WrongInputException();
}

void Color::parseHex()
{
	// TODO dodac obsluge formatu 0xFFFFFFFF
	int r, g, b, a;
	r = g = b = a = 255;
	int foundAmmount = sscanf(m_RawColor.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a); // TODO: podac tu sama wartosc po # lub 0x
	// TODO: warning C4996 : 'sscanf' : This function or variable may be unsafe.Consider using sscanf_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.

	/// kod martwy z uwagi na pattern isHex ale sluzy do demonstracji mozliwych wyjatkow 
	// Jednak moze okazac sie pomocny po zmianach w wzorcu z isHex()
	if (foundAmmount != 3 && foundAmmount != 4)
	{
		// TODO: dodac log gdy brakuje alphy ze dodalismy ja rowna 1
		throw WrongChannelAmountException();
		return;
	}

	m_ColorF = ColorF({ r / 255.f, g / 255.f, b / 255.f, a / 255.f });
}

void Color::parseInt()
{
	vector<string> chanels = split(m_RawColor, ",");

	float r = atoi(chanels[0].c_str()) / 255.f;
	float g = atoi(chanels[1].c_str()) / 255.f;
	float b = atoi(chanels[2].c_str()) / 255.f;
	float a = chanels.size() == 4 ? atoi(chanels[3].c_str()) / 255.f : 1.0f;

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
	{
		throw ChanelOutOfRangeException();
		return;
	}

	m_ColorF = ColorF({ r, g, b, a });
}

void Color::parseFloat()
{
	vector<string> chanels = split(m_RawColor, ",");

	float r = atof(chanels[0].c_str());
	float g = atof(chanels[1].c_str());
	float b = atof(chanels[2].c_str());
	float a = atof(chanels[3].c_str());

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
	{
		throw ChanelOutOfRangeException();
		return;
	}

	m_ColorF = ColorF({ r, g, b, a });
}

void Color::parseText()
{
	/*
			A name from the list of colors defined in the list of SVG color keyword names provided by the World Wide Web Consortium;
			for example, "steelblue" or "gainsboro". These color names work on all platforms.
	*/

	QColor qColor(m_RawColor.c_str());

	if (!qColor.isValid())
		throw WrongInputException(); /// A dead code, but serves as exception usage example

	qreal r, g, b, a;
	qColor.getRgbF(&r, &g, &b, &a);
	m_ColorF = ColorF({ (float)r, (float)g, (float)b, (float)a }); /// For demonstration purposes I've used C style cast instead C++ static_cast which would be better
}