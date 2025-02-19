#include "color.h"

#include <regex>
#include <vector>
#include <unordered_map>

using std::vector;

static const std::unordered_map<std::string, ColorF> g_Colors = {
    {"aliceblue",            {0.941f, 0.973f, 1.000f, 1.0f}},
    {"antiquewhite",         {0.980f, 0.922f, 0.843f, 1.0f}},
    {"aqua",                 {0.000f, 1.000f, 1.000f, 1.0f}},
    {"aquamarine",           {0.498f, 1.000f, 0.831f, 1.0f}},
    {"azure",                {0.941f, 1.000f, 1.000f, 1.0f}},
    {"beige",                {0.961f, 0.961f, 0.863f, 1.0f}},
    {"bisque",               {1.000f, 0.894f, 0.769f, 1.0f}},
    {"black",                {0.000f, 0.000f, 0.000f, 1.0f}},
    {"blanchedalmond",       {1.000f, 0.922f, 0.804f, 1.0f}},
    {"blue",                 {0.000f, 0.000f, 1.000f, 1.0f}},
    {"blueviolet",           {0.541f, 0.169f, 0.886f, 1.0f}},
    {"brown",                {0.647f, 0.165f, 0.165f, 1.0f}},
    {"burlywood",            {0.871f, 0.722f, 0.529f, 1.0f}},
    {"cadetblue",            {0.373f, 0.620f, 0.627f, 1.0f}},
    {"chartreuse",           {0.498f, 1.000f, 0.000f, 1.0f}},
    {"chocolate",            {0.824f, 0.412f, 0.118f, 1.0f}},
    {"coral",                {1.000f, 0.498f, 0.314f, 1.0f}},
    {"cornflowerblue",       {0.392f, 0.584f, 0.929f, 1.0f}},
    {"cornsilk",             {1.000f, 0.973f, 0.863f, 1.0f}},
    {"crimson",              {0.863f, 0.078f, 0.235f, 1.0f}},
    {"cyan",                 {0.000f, 1.000f, 1.000f, 1.0f}},
    {"darkblue",             {0.000f, 0.000f, 0.545f, 1.0f}},
    {"darkcyan",             {0.000f, 0.545f, 0.545f, 1.0f}},
    {"darkgoldenrod",        {0.722f, 0.525f, 0.043f, 1.0f}},
    {"darkgray",             {0.663f, 0.663f, 0.663f, 1.0f}},
    {"darkgreen",            {0.000f, 0.392f, 0.000f, 1.0f}},
    {"darkgrey",             {0.663f, 0.663f, 0.663f, 1.0f}},
    {"darkkhaki",            {0.741f, 0.718f, 0.420f, 1.0f}},
    {"darkmagenta",          {0.545f, 0.000f, 0.545f, 1.0f}},
    {"darkolivegreen",       {0.333f, 0.420f, 0.184f, 1.0f}},
    {"darkorange",           {1.000f, 0.549f, 0.000f, 1.0f}},
    {"darkorchid",           {0.600f, 0.196f, 0.800f, 1.0f}},
    {"darkred",              {0.545f, 0.000f, 0.000f, 1.0f}},
    {"darksalmon",           {0.914f, 0.588f, 0.478f, 1.0f}},
    {"darkseagreen",         {0.561f, 0.737f, 0.561f, 1.0f}},
    {"darkslateblue",        {0.282f, 0.239f, 0.545f, 1.0f}},
    {"darkslategray",        {0.184f, 0.310f, 0.310f, 1.0f}},
    {"darkslategrey",        {0.184f, 0.310f, 0.310f, 1.0f}},
    {"darkturquoise",        {0.000f, 0.808f, 0.820f, 1.0f}},
    {"darkviolet",           {0.580f, 0.000f, 0.827f, 1.0f}},
    {"deeppink",             {1.000f, 0.078f, 0.576f, 1.0f}},
    {"deepskyblue",          {0.000f, 0.749f, 1.000f, 1.0f}},
    {"dimgray",              {0.412f, 0.412f, 0.412f, 1.0f}},
    {"dimgrey",              {0.412f, 0.412f, 0.412f, 1.0f}},
    {"dodgerblue",           {0.118f, 0.565f, 1.000f, 1.0f}},
    {"firebrick",            {0.698f, 0.133f, 0.133f, 1.0f}},
    {"floralwhite",          {1.000f, 0.980f, 0.941f, 1.0f}},
    {"forestgreen",          {0.133f, 0.545f, 0.133f, 1.0f}},
    {"fuchsia",              {1.000f, 0.000f, 1.000f, 1.0f}},
    {"gainsboro",            {0.863f, 0.863f, 0.863f, 1.0f}},
    {"ghostwhite",           {0.973f, 0.973f, 1.000f, 1.0f}},
    {"gold",                 {1.000f, 0.843f, 0.000f, 1.0f}},
    {"goldenrod",            {0.855f, 0.647f, 0.125f, 1.0f}},
    {"gray",                 {0.502f, 0.502f, 0.502f, 1.0f}},
    {"green",                {0.000f, 0.502f, 0.000f, 1.0f}},
    {"greenyellow",          {0.678f, 1.000f, 0.184f, 1.0f}},
    {"grey",                 {0.502f, 0.502f, 0.502f, 1.0f}},
    {"honeydew",             {0.941f, 1.000f, 0.941f, 1.0f}},
    {"hotpink",              {1.000f, 0.412f, 0.706f, 1.0f}},
    {"indianred",            {0.804f, 0.361f, 0.361f, 1.0f}},
    {"indigo",               {0.294f, 0.000f, 0.510f, 1.0f}},
    {"ivory",                {1.000f, 1.000f, 0.941f, 1.0f}},
    {"khaki",                {0.941f, 0.902f, 0.549f, 1.0f}},
    {"lavender",             {0.902f, 0.902f, 0.980f, 1.0f}},
    {"lavenderblush",        {1.000f, 0.941f, 0.961f, 1.0f}},
    {"lawngreen",            {0.486f, 0.988f, 0.000f, 1.0f}},
    {"lemonchiffon",         {1.000f, 0.980f, 0.804f, 1.0f}},
    {"lightblue",            {0.678f, 0.847f, 0.902f, 1.0f}},
    {"lightcoral",           {0.941f, 0.502f, 0.502f, 1.0f}},
    {"lightcyan",            {0.878f, 1.000f, 1.000f, 1.0f}},
    {"lightgoldenrodyellow", {0.980f, 0.980f, 0.824f, 1.0f}},
    {"lightgray",            {0.827f, 0.827f, 0.827f, 1.0f}},
    {"lightgreen",           {0.565f, 0.933f, 0.565f, 1.0f}},
    {"lightgrey",            {0.827f, 0.827f, 0.827f, 1.0f}},
    {"lightpink",            {1.000f, 0.714f, 0.757f, 1.0f}},
    {"lightsalmon",          {1.000f, 0.627f, 0.478f, 1.0f}},
    {"lightseagreen",        {0.125f, 0.698f, 0.667f, 1.0f}},
    {"lightskyblue",         {0.529f, 0.808f, 0.980f, 1.0f}},
    {"lightslategray",       {0.467f, 0.533f, 0.600f, 1.0f}},
    {"lightslategrey",       {0.467f, 0.533f, 0.600f, 1.0f}},
    {"lightsteelblue",       {0.690f, 0.769f, 0.871f, 1.0f}},
    {"lightyellow",          {1.000f, 1.000f, 0.878f, 1.0f}},
    {"lime",                 {0.000f, 1.000f, 0.000f, 1.0f}},
    {"limegreen",            {0.196f, 0.804f, 0.196f, 1.0f}},
    {"linen",                {0.980f, 0.941f, 0.902f, 1.0f}},
    {"magenta",              {1.000f, 0.000f, 1.000f, 1.0f}},
    {"maroon",               {0.502f, 0.000f, 0.000f, 1.0f}},
    {"mediumaquamarine",     {0.400f, 0.804f, 0.667f, 1.0f}},
    {"mediumblue",           {0.000f, 0.000f, 0.804f, 1.0f}},
    {"mediumorchid",         {0.729f, 0.333f, 0.827f, 1.0f}},
    {"mediumpurple",         {0.576f, 0.439f, 0.859f, 1.0f}},
    {"mediumseagreen",       {0.235f, 0.702f, 0.443f, 1.0f}},
    {"mediumslateblue",      {0.482f, 0.408f, 0.933f, 1.0f}},
    {"mediumspringgreen",    {0.000f, 0.980f, 0.604f, 1.0f}},
    {"mediumturquoise",      {0.282f, 0.820f, 0.800f, 1.0f}},
    {"mediumvioletred",      {0.780f, 0.082f, 0.522f, 1.0f}},
    {"midnightblue",         {0.098f, 0.098f, 0.439f, 1.0f}},
    {"mintcream",            {0.961f, 1.000f, 0.980f, 1.0f}},
    {"mistyrose",            {1.000f, 0.894f, 0.882f, 1.0f}},
    {"moccasin",             {1.000f, 0.894f, 0.710f, 1.0f}},
    {"navajowhite",          {1.000f, 0.871f, 0.678f, 1.0f}},
    {"navy",                 {0.000f, 0.000f, 0.502f, 1.0f}},
    {"oldlace",              {0.992f, 0.961f, 0.902f, 1.0f}},
    {"olive",                {0.502f, 0.502f, 0.000f, 1.0f}},
    {"olivedrab",            {0.420f, 0.557f, 0.137f, 1.0f}},
    {"orange",               {1.000f, 0.647f, 0.000f, 1.0f}},
    {"orangered",            {1.000f, 0.271f, 0.000f, 1.0f}},
    {"orchid",               {0.855f, 0.439f, 0.839f, 1.0f}},
    {"palegoldenrod",        {0.933f, 0.910f, 0.667f, 1.0f}},
    {"palegreen",            {0.596f, 0.984f, 0.596f, 1.0f}},
    {"paleturquoise",        {0.686f, 0.933f, 0.933f, 1.0f}},
    {"palevioletred",        {0.859f, 0.439f, 0.576f, 1.0f}},
    {"papayawhip",           {1.000f, 0.937f, 0.835f, 1.0f}},
    {"peachpuff",            {1.000f, 0.855f, 0.725f, 1.0f}},
    {"peru",                 {0.804f, 0.522f, 0.247f, 1.0f}},
    {"pink",                 {1.000f, 0.753f, 0.796f, 1.0f}},
    {"plum",                 {0.867f, 0.627f, 0.867f, 1.0f}},
    {"powderblue",           {0.690f, 0.878f, 0.902f, 1.0f}},
    {"purple",               {0.502f, 0.000f, 0.502f, 1.0f}},
    {"red",                  {1.000f, 0.000f, 0.000f, 1.0f}},
    {"rosybrown",            {0.737f, 0.561f, 0.561f, 1.0f}},
    {"royalblue",            {0.255f, 0.412f, 0.882f, 1.0f}},
    {"saddlebrown",          {0.545f, 0.271f, 0.075f, 1.0f}},
    {"salmon",               {0.980f, 0.502f, 0.447f, 1.0f}},
    {"sandybrown",           {0.957f, 0.643f, 0.376f, 1.0f}},
    {"seagreen",             {0.180f, 0.545f, 0.341f, 1.0f}},
    {"seashell",             {1.000f, 0.961f, 0.933f, 1.0f}},
    {"sienna",               {0.627f, 0.322f, 0.176f, 1.0f}},
    {"silver",               {0.753f, 0.753f, 0.753f, 1.0f}},
    {"skyblue",              {0.529f, 0.808f, 0.922f, 1.0f}},
    {"slateblue",            {0.416f, 0.353f, 0.804f, 1.0f}},
    {"slategray",            {0.439f, 0.502f, 0.565f, 1.0f}},
    {"slategrey",            {0.439f, 0.502f, 0.565f, 1.0f}},
    {"snow",                 {1.000f, 0.980f, 0.980f, 1.0f}},
    {"springgreen",          {0.000f, 1.000f, 0.498f, 1.0f}},
    {"steelblue",            {0.275f, 0.510f, 0.706f, 1.0f}},
    {"tan",                  {0.824f, 0.706f, 0.549f, 1.0f}},
    {"teal",                 {0.000f, 0.502f, 0.502f, 1.0f}},
    {"thistle",              {0.847f, 0.749f, 0.847f, 1.0f}},
    {"tomato",               {1.000f, 0.388f, 0.278f, 1.0f}},
    {"turquoise",            {0.251f, 0.878f, 0.816f, 1.0f}},
    {"violet",               {0.933f, 0.510f, 0.933f, 1.0f}},
    {"wheat",                {0.961f, 0.871f, 0.702f, 1.0f}},
    {"white",                {1.000f, 1.000f, 1.000f, 1.0f}},
    {"whitesmoke",           {0.961f, 0.961f, 0.961f, 1.0f}},
    {"yellow",               {1.000f, 1.000f, 0.000f, 1.0f}},
    {"yellowgreen",          {0.604f, 0.804f, 0.196f, 1.0f}}
};

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
	m_RawColor(rawColor)
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

bool Color::isKnown()
{
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
	// Qt version
	if (QColor::colorNames().contains(QString(m_RawColor.c_str()), Qt::CaseInsensitive))
		return true;
#endif
	// non-Qt version
    auto it = g_Colors.find(toLowerString(m_RawColor));
    if (it != g_Colors.end())
        return true;

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

	if (isText() && isKnown())
	{
		parseText();
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

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
		throw ChannelOutOfRangeException();

	m_ColorF = { r, g, b, a };
}

void Color::parseFloat()
{
	vector<string> chanels = split(m_RawColor, ",");

	float r = atof(chanels[0].c_str());
	float g = atof(chanels[1].c_str());
	float b = atof(chanels[2].c_str());
	float a = chanels.size() == 4 ? atof(chanels[3].c_str()) : 1.0f;

	if (r > 1.f || g > 1.f || b > 1.f || a > 1.f)
		throw ChannelOutOfRangeException();

	m_ColorF = { r, g, b, a };
}

void Color::parseText()
{
	/*
	A name from the list of colors defined in the list of SVG color keyword names provided by the World Wide Web Consortium;
	for example, "steelblue" or "gainsboro". These color names work on all platforms.
    basic color name: https://www.w3.org/TR/SVG11/types.html#ColorKeywords
	*/

    // non-Qt Version
    auto it = g_Colors.find(toLowerString(m_RawColor));
    if (it != g_Colors.end())
    {
        m_ColorF = it->second;
        return;
    }
#ifndef MINIMUM_USAGE_OF_QT_FRAMEWORK
    // Qt Version (has to be second because throws exception in case of lack of color name in the base)
    QColor qColor(m_RawColor.c_str());

    if (!qColor.isValid())
        throw WrongInputException();
    else
    {
        qreal r, g, b, a;
        qColor.getRgbF(&r, &g, &b, &a);
        m_ColorF = { (float)r, (float)g, (float)b, (float)a }; /// For demonstration purposes I've used C style cast instead C++ static_cast which would be better
        return;
    }
#endif
}