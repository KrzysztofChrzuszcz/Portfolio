#include "tinydataloader.h"

using namespace std;
using namespace tinyxml2;

extern const Color g_DafaultColor;

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
                const char* title = optionElem->GetText(); // NOTE: Issue with polish special signs TODO: Consider some solution
                printf("%s: ", title);

                const char* color = optionElem->Attribute("color");
                printf("%s\n", color);

                try
                {
                    m_Entries.push_back({ string(title), Color(string(color)) });
                }
                catch (...)
                {
                    printf("Nothing wrong has happen thanks to default color, but there is issue with given color: %s\n", color);
                    m_DataCorrupted = true;
                    // TODO: Warning Window

                    try
                    {
                        throw;
                    }
                    catch (WrongInputException& wie)
                    {
                        m_ErrorFlags.set(0);
                    }
                    catch (ChanelOutOfRangeException& coore)
                    {
                        m_ErrorFlags |= bitset<4>(1 << 1);
                    }
                    catch (WrongChannelAmountException& wcae)
                    {
                        m_ErrorFlags |= bitset<4>(1 << 2);
                    }
                }

                optionElem = optionElem->NextSiblingElement();
            } while (optionElem != nullptr);

            if (m_MaxPositionsAmount)
                if (m_Entries.size() <= 1 || m_Entries.size() > m_MaxPositionsAmount)
                {   
                    m_ErrorFlags.set(3);
                    fullSuccess = false; // ??
                    printf("It is not possible to visualize %d position with given settings for range of pie piece angle!\n", m_Entries.size());
                    // TODO: Zdecydowac czy ustawic fullSuccess na false czy pozwolic na udane wyjscie z funkcji za to np. wywolac warningwindow
                    // TODO: Decide about setting up fullSucces to false value
                }
        }

    // TODO: use result to call AlarmLoadingDataProblem in Engine
    // Add WarningWindow. Maybe within some additional info like
    // [Data corrupted, for more info check log file]

    // TODO2: add logFile'a 
    return fullSuccess; 
}