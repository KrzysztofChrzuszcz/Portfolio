#include "color.h"

#include <regex>
#include <vector>

using std::vector;

extern const Color g_DafaultColor(string("0.5,0.5,0.5,1.0"));

inline vector<string> split(string s, const string& delimiter) /// We would like to have a copy due to not integrate in original input string
{
	vector<string> subStrings;
	string subString;
	size_t position = 0;

	while ((position = s.find(delimiter)) != string::npos)
	{
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

	string resultHex;
	char redHex[2];
	itoa(m_ColorF.m_R * 255, redHex, 16);
	char greenHex[2];
	itoa(m_ColorF.m_G * 255, greenHex, 16);
	char blueHex[2];
	itoa(m_ColorF.m_B * 255, blueHex, 16);
	char alphaHex[2];
	itoa(m_ColorF.m_A * 255, alphaHex, 16);

	resultHex = string('#' + string(redHex) + greenHex + blueHex + alphaHex);
	for (char& c : resultHex)
		 c = toupper(c);

	return resultHex;
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
	float currentLevel = getBrightness();
	if (minLevel > currentLevel)
	{
		m_IsBrighten = true;
		float delta = minLevel - currentLevel;
		m_ColorF = m_ColorF + delta;
	}
}

bool Color::isHex()
{
	std::regex pattern("#([0-9a-fA-F]{2}){3,4}");
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	pattern = std::regex("0x([0-9a-fA-F]{2}){3,4}");

	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

bool Color::isText()
{
	if (isHex())
		return false;
	if (isIntForm())
		return false;
	if (isFloatForm())
		return false;
	if (m_RawColor.empty())
		return false;

	return true;
}

bool Color::isKonwn()
{
	// Qt version
	if (QColor::colorNames().contains(QString(m_RawColor.c_str()), Qt::CaseInsensitive))
		return true;

	// TODO: non-Qt version

	return false;
}

bool Color::isIntForm()
{
	std::regex pattern("([0-9]{1,3}[,]?){3,4}");
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

bool Color::isFloatForm()
{
	std::regex pattern("([0-1].[0-9]{1,5}[,]?){3,4}");
	std::smatch match;
	if (std::regex_match(m_RawColor, match, pattern))
		return true;

	return false;
}

void Color::parse()
{
	if (isHex())
	{
		parseHex();
		return;
	}

	if (isIntForm())
	{
		parseInt();
		return;
	}

	if (isFloatForm())
	{
		parseFloat();
		return;
	}

	if (isText() && isKonwn())
	{
		parseText(); // TODO: non-Qt version
		return;
	}

	throw WrongInputException();
}

void Color::parseHex()
{
	int r, g, b, a;
	r = g = b = a = 255;
	int foundAmmount = sscanf_s(m_RawColor.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
	if (foundAmmount == 0)
		foundAmmount = sscanf_s(m_RawColor.c_str(), "0x%02x%02x%02x%02x", &r, &g, &b, &a);


	/// A dead code (due to previous isHex()), but serves as exception usage example
	if (foundAmmount != 3 && foundAmmount != 4)
		throw WrongChannelAmountException();

	//if ((foundAmmount == 3)
		// TODO: Add log about missing alpha and info We fix that

	/// Method parseHex doesn't need range check because max number that can reach is FF that is 255,
	/// so its not possible to go out of range in this case.

	m_ColorF = { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
}

void Color::parseInt()
{
	vector<string> chanels = split(m_RawColor, ",");

	float r = atoi(chanels[0].c_str()) / 255.f;
	float g = atoi(chanels[1].c_str()) / 255.f;
	float b = atoi(chanels[2].c_str()) / 255.f;
	float a = chanels.size() == 4 ? atoi(chanels[3].c_str()) / 255.f : 1.0f;

	//if (chanels.size() == 3)
	// TODO: Add log about missing alpha and info We fix that

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
		throw ChanelOutOfRangeException();

	m_ColorF = { r, g, b, a };
}

void Color::parseFloat()
{
	vector<string> chanels = split(m_RawColor, ",");

	float r = atof(chanels[0].c_str());
	float g = atof(chanels[1].c_str());
	float b = atof(chanels[2].c_str());
	float a = chanels.size() == 4 ? atof(chanels[3].c_str()) : 1.0f;

	//if (chanels.size() == 3)
	// TODO: Add log about missing alpha and info We fix that

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
		throw ChanelOutOfRangeException();

	m_ColorF = { r, g, b, a };
}

void Color::parseText()
{
	/*
	A name from the list of colors defined in the list of SVG color keyword names provided by the World Wide Web Consortium;
	for example, "steelblue" or "gainsboro". These color names work on all platforms.
	*/

	// TODO: non-Qt version
	// A. switch with listed color names
	// B. use external file
	// C. use third-party library for example : https://github.com/svgpp/svgpp

	QColor qColor(m_RawColor.c_str());

	if (!qColor.isValid())
		throw WrongInputException();
	//+ log about unknown

	qreal r, g, b, a;
	qColor.getRgbF(&r, &g, &b, &a);
	m_ColorF = { (float)r, (float)g, (float)b, (float)a }; /// For demonstration purposes I've used C style cast instead C++ static_cast which would be better
}