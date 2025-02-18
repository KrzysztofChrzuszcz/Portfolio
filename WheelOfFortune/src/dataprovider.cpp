#include "dataprovider.h"
#include "rapidjson/document.h" 
#include "rapidjson/filewritestream.h" 
#include "rapidjson/writer.h" 

#include <regex>
#include <fstream>
#include <iostream>
#include <cassert>

CSVDataProvider::CSVDataProvider(const std::string& filePath) :
    m_FilePath(filePath.c_str())
{
    assert(!filePath.empty());
    assert(filePath.substr(filePath.length() - 4) == ".csv");
}

CSVDataProvider::~CSVDataProvider()
{
    if (m_File.is_open())
    {
        m_File.close();
    }
}

bool CSVDataProvider::dump(StringList& input) noexcept
{
    m_File.open(m_FilePath, std::ios::out);
    if (m_File.is_open())
    {
        auto it = input.begin();
        while (it != input.end())
        {
            m_File << *it;
            ++it;
            if (it != input.end())
                m_File << ',';
        }

        m_File.flush();
        m_File.close();
        return true;
    }

    return false;
}

bool CSVDataProvider::load(StringList& output) noexcept
{
    m_File.open(m_FilePath, std::ios::in);
    if (!m_File.is_open())
    {
        return false;
    }
    else
    {
        std::string data;
        m_File >> data;

        if (data.empty())
            return false;

        std::regex delimiter(",");
        std::sregex_token_iterator it(data.begin(), data.end(), delimiter, -1);
        std::sregex_token_iterator end;

        while (it != end)
            output.push_back(*(it++));

        m_File.close();
        return true;
    }

    return false;
}

JSONDataProvider::JSONDataProvider(const std::string& filePath) :
    m_FilePath(filePath.c_str())
{
    assert(!filePath.empty());
    assert(filePath.substr(filePath.length() - 5) == ".json");
}

JSONDataProvider::~JSONDataProvider()
{
}

bool JSONDataProvider::dump(std::string& input) noexcept
{
    std::ofstream file(m_FilePath);
    if (!file.is_open())
    {
        return false;
    }

    file << input;

    if (!file.good())
    {
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool JSONDataProvider::load(std::string& output) noexcept
{
    std::ifstream file(m_FilePath);

    std::string json((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    rapidjson::Document document;
    document.Parse(json.c_str());

    if (document.HasParseError())
    {
        return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    output = buffer.GetString();

    return true;
}
