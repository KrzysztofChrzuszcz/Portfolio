#include "settings.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <iomanip>
#include <sstream>

const int g_MinAngleRange = 15;
const int g_MaxAngleRange = 60;
const int g_MinDurationTime = 8;
const int g_MaxDurationTime = 20;
const int g_MinRandRange = 2000;
const int g_MaxRandRange = 5000;

Settings::Settings()
{
	setDefaultValues();
	updateLastChangeTime();
}

void Settings::setDefaultValues() noexcept
{
	m_AutoStart = false;
	m_DrawLots = false;
	m_AutoAdjust = true;

	m_ScreenRefreshFrequencies = { 24, 30, 50, 55, 60 };
	m_ScreenRefreshFrequencyIndex = 2;

	m_MinColorBrightness = 0.23f;

	m_MinAngle = 20;
	m_MaxAngle = 30;

	m_MaxDurationTime = 12;

	m_MinRandRange = 2500;
	m_MaxRandRange = 3000;

	m_RandomGenerator = RandGenerator::standard;
	m_DataState = DataState::NotSelected;
}

void Settings::updateLastChangeTime() noexcept
{
	auto now = std::chrono::system_clock::now();
	m_LastChangeTime = std::chrono::system_clock::to_time_t(now);
}

std::time_t Settings::getTimestamp() const noexcept
{
	return m_LastChangeTime;
}

bool Settings::hasChanged(const std::time_t& since) const noexcept
{
	return since != m_LastChangeTime;
}

string time_t_to_string(time_t time)
{
	std::tm* timeinfo = std::localtime(&time);

	char buffer[80];

	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

	return string(buffer);
}

std::time_t string_to_time_t(const std::string& timeStr)
{
	std::tm tm = {};
	std::istringstream ss(timeStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	if (ss.fail())
	{
		throw std::runtime_error("Failed to parse time string: " + timeStr);
	}

	return std::mktime(&tm);
}

string Settings::toJSON() const noexcept
{
	rapidjson::Document document;
	document.SetObject();
	auto& allocator = document.GetAllocator();

	document.AddMember("draw lots", m_DrawLots, allocator);
	document.AddMember("auto start", m_AutoStart, allocator);
	document.AddMember("auto adjust", m_AutoAdjust, allocator);
	document.AddMember("min color brightness", m_MinColorBrightness, allocator);
	document.AddMember("screen refresh frequency index", m_ScreenRefreshFrequencyIndex, allocator);
	document.AddMember("min angle", m_MinAngle, allocator);
	document.AddMember("max angle", m_MaxAngle, allocator);
	document.AddMember("max duration time", m_MaxDurationTime, allocator);
	document.AddMember("min rand range", m_MinRandRange, allocator);
	document.AddMember("max rand range", m_MaxRandRange, allocator);
	document.AddMember("random generator", static_cast<int>(m_RandomGenerator), allocator);

	string s = time_t_to_string(m_LastChangeTime);
	document.AddMember("last change time", rapidjson::Value(s.c_str(), document.GetAllocator()), document.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	return buffer.GetString();
}

void Settings::fromJSON(const string& input)
{
	rapidjson::Document document;
	document.Parse(input.c_str());

	if (document.HasParseError())
	{
		throw std::runtime_error("Failed to parse JSON input");
	}

	if (!document.IsObject())
	{
		throw std::runtime_error("Invalid JSON format: expected an object");
	}

	if (document.HasMember("draw lots") && document["draw lots"].IsBool())
	{
		m_DrawLots = document["draw lots"].GetBool();
	}

	if (document.HasMember("auto start") && document["auto start"].IsBool())
	{
		m_AutoStart = document["auto start"].GetBool();
	}

	if (document.HasMember("auto adjust") && document["auto adjust"].IsBool())
	{
		m_AutoAdjust = document["auto adjust"].GetBool();
	}

	if (document.HasMember("min color brightness") && document["min color brightness"].IsFloat())
	{
		m_MinColorBrightness = document["min color brightness"].GetFloat();
	}

	if (document.HasMember("screen refresh frequency index") && document["screen refresh frequency index"].IsInt())
	{
		m_ScreenRefreshFrequencyIndex = document["screen refresh frequency index"].GetInt();
	}

	if (document.HasMember("min angle") && document["min angle"].IsInt())
	{
		m_MinAngle = document["min angle"].GetInt();
	}

	if (document.HasMember("max angle") && document["max angle"].IsInt())
	{
		m_MaxAngle = document["max angle"].GetInt();
	}

	if (document.HasMember("max duration time") && document["max duration time"].IsInt())
	{
		m_MaxDurationTime = document["max duration time"].GetInt();
	}

	if (document.HasMember("min rand range") && document["min rand range"].IsInt())
	{
		m_MinRandRange = document["min rand range"].GetInt();
	}

	if (document.HasMember("max rand range") && document["max rand range"].IsInt())
	{
		m_MaxRandRange = document["max rand range"].GetInt();
	}

	if (document.HasMember("random generator") && document["random generator"].IsInt())
	{
		m_RandomGenerator = static_cast<RandGenerator>(document["random generator"].GetInt());
	}

	if (document.HasMember("last change time") && document["last change time"].IsString())
	{
		string lastChangeTimeStr = document["last change time"].GetString();
		m_LastChangeTime = string_to_time_t(lastChangeTimeStr);
	}

	validate();
}

list<string> Settings::serialize() const noexcept
{
	return list<string>{
		std::to_string(static_cast<int>(m_DrawLots)),
		std::to_string(static_cast<int>(m_AutoStart)),
		std::to_string(static_cast<int>(m_AutoAdjust)),
		std::to_string(static_cast<float>(m_MinColorBrightness)),
		std::to_string(m_ScreenRefreshFrequencyIndex),
		std::to_string(m_MinAngle),
		std::to_string(m_MaxAngle),
		std::to_string(m_MaxDurationTime),
		std::to_string(m_MinRandRange),
		std::to_string(m_MaxRandRange),
		std::to_string(static_cast<int>(m_RandomGenerator)),
		time_t_to_string(m_LastChangeTime) 
	};
}

void Settings::deserializeAndSet(list<string>& input)
{
	list<string>::iterator s = input.begin();
	try
	{
		m_DrawLots = std::stoi(*(s++));
		m_AutoStart = std::stoi(*(s++));
		m_AutoAdjust = std::stoi(*(s++));
		m_MinColorBrightness = std::stof(*(s++));
		m_ScreenRefreshFrequencyIndex = std::stoi(*(s++));
		m_MinAngle = std::stoi(*(s++));
		m_MaxAngle = std::stoi(*(s++));
		m_MaxDurationTime = std::stoi(*(s++));
		m_MinRandRange = std::stoi(*(s++));
		m_MaxRandRange = std::stoi(*(s++));
		m_RandomGenerator = RandGenerator(std::stoi(*(s++)));
		m_LastChangeTime = string_to_time_t(*(s++)); // NOTE: Not needed but serves as an example of de/serialization of timestamp

		validate();
	}
	catch (const std::exception& e)
	{
		setDefaultValues();
	}
}

/**
 * \note It may happen someone edits settings file manually with wrong values.
 *		 Program suppose be robust to that inappropriate intervention.
 */
void Settings::validate() const
{
	if (m_MinColorBrightness < 0 || m_MinColorBrightness > 1.0)
		throw std::range_error("Invalid m_MinColorBrightness value");
	if (m_ScreenRefreshFrequencyIndex < 0 || m_ScreenRefreshFrequencyIndex > 5)
		throw std::range_error("Invalid m_ScreenRefreshFrequencyIndex value");
	if (m_MinAngle > m_MaxAngle)
		throw std::range_error("m_MinAngle cannot be greater than m_MaxAngle");
	if (m_MinAngle <= g_MinAngleRange || m_MinAngle >= g_MaxAngleRange)
		throw std::range_error("m_MinAngle out of range");
	if (m_MaxAngle <= g_MinAngleRange || m_MaxAngle >= g_MaxAngleRange)
		throw std::range_error("m_MaxAngle out of range");
	if (m_MaxDurationTime < g_MinDurationTime || m_MaxDurationTime > g_MaxDurationTime)
		throw std::range_error("Invalid m_MaxDurationTime value");
	if (m_MinRandRange > m_MaxRandRange)
		throw std::range_error("m_MinRandRange cannot be greater than m_MaxRandRange");
	if (m_MinRandRange < g_MinRandRange || m_MinRandRange > g_MaxRandRange)
		throw std::range_error("m_MinRandRange out of range");
	if (m_MaxRandRange < g_MinRandRange || m_MaxRandRange > g_MaxRandRange)
		throw std::range_error("m_MaxRandRange out of range");
	if (int(m_RandomGenerator) < 0 || int(m_RandomGenerator) > 5)
		throw std::range_error("Invalid m_RandomGenerator value");
}
