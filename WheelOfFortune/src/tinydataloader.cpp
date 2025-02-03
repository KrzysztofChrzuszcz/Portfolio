#include "tinydataloader.h"

#include <codecvt>
#include <locale>
#include <unordered_map>

using namespace std;
using namespace tinyxml2;

extern const Color g_DafaultColor;

TinyDataLoader::TinyDataLoader(std::shared_ptr<ILogger> logger) :
    DataLoader(logger)
{
}

bool TinyDataLoader::loadXml(const char* path)
{
    if (path == nullptr)
        return false;

    m_ErrorFlags.reset();
    m_Entries.clear();

    m_DataCorrupted = false;
    bool fullSuccess = false;

    XMLDocument doc;
    XMLError err = doc.LoadFile(path); // NOTE: Issue with polish special signs in the path

    if (XMLElement* rootElem = doc.FirstChildElement("root"))
        if (XMLElement* optionsElem = rootElem->FirstChildElement("options"))
        {
            if (err == XML_SUCCESS)
                fullSuccess = true;

            XMLElement* optionElem = optionsElem->FirstChildElement("option");
            do
            {
                const char* encodedText = optionElem->GetText(); 
                std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                std::wstring wtitle = converter.from_bytes(encodedText ? encodedText : "");
                std::string title = depolonise(wtitle); /// Used later freeglut2 library is not applicable for polish signs. So there is a need to change all special signs to English equivalent. 
                std::string color = string(optionElem->Attribute("color"));
                m_Logger->log(LogLevel::Info, "TinyDataLoader", "Loaded positon name: " + title + ", color: " + color);

                try
                {
                    m_Entries.push_back({ title, Color(string(color)) });
                }
                catch (...)
                {
                    m_Logger->log(LogLevel::Warning, "TinyDataLoader", "Nothing wrong has happen thanks to default color, but there is issue with given color: " + std::string(color));
                    m_DataCorrupted = true;

                    try
                    {
                        throw;
                    }
                    catch (WrongInputException& wie)
                    {
                        m_ErrorFlags.set(0);
                        m_Logger->log(LogLevel::Critical, "TinyDataLoader", g_WrongInputMsg);
                    }
                    catch (ChannelOutOfRangeException& coore)
                    {
                        m_ErrorFlags |= bitset<4>(1 << 1);
                        m_Logger->log(LogLevel::Critical, "TinyDataLoader", g_ChannelOutOfRangeMsg);
                    }
                    catch (WrongChannelAmountException& wcae)
                    {
                        m_ErrorFlags |= bitset<4>(1 << 2);
                        m_Logger->log(LogLevel::Critical, "TinyDataLoader", g_WrongChannelAmountMsg);
                    }
                }

                optionElem = optionElem->NextSiblingElement();
            } while (optionElem != nullptr);
        }

    if (err != XML_SUCCESS)
        m_Logger->log(LogLevel::Error, "TinyDataLoader", std::string("Cannot read file ") + path);

    return fullSuccess; 
}

string TinyDataLoader::depolonise(const std::wstring& text)
{
    static const std::unordered_map<wchar_t, char> polishToEnglish = {
        {L'ą', 'a'}, {L'ć', 'c'}, {L'ę', 'e'}, {L'ł', 'l'}, {L'ń', 'n'}, {L'ó', 'o'}, {L'ś', 's'}, {L'ź', 'z'}, {L'ż', 'z'},
        {L'Ą', 'A'}, {L'Ć', 'C'}, {L'Ę', 'E'}, {L'Ł', 'L'}, {L'Ń', 'N'}, {L'Ó', 'O'}, {L'Ś', 'S'}, {L'Ź', 'Z'}, {L'Ż', 'Z'}
    };

    std::string result;
    result.reserve(text.size());

    for (wchar_t wc : text) 
    {
        auto it = polishToEnglish.find(wc);
        if (it != polishToEnglish.end())
        {
            result += it->second;
        }
        else
        {
            result += static_cast<char>(wc);
        }
    }

    return result;
}
