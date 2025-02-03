#include "qtdataloader.h"

#include <QXmlStreamReader>

extern const Color g_DafaultColor;

QtDataLoader::QtDataLoader(std::shared_ptr<ILogger> logger) :
    DataLoader(logger)
{
}

bool QtDataLoader::loadXml(const char* path)
{
    if (path == nullptr)
        return false;
    
    m_ErrorFlags.reset();
    m_Entries.clear();

    m_DataCorrupted = false;
    bool fullSuccess = true;

    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        fullSuccess = false;
        m_Logger->log(LogLevel::Error, "QtDataLoader", std::string("Cannot read file ") + file.errorString().toStdString());
    }
    QXmlStreamReader reader(&file);

    if (reader.readNextStartElement())
    {
        if (reader.name() == "root")
        {
            while (reader.readNextStartElement())
                if (reader.name() == "options")
                {
                    while (reader.readNextStartElement())
                        if (reader.name() == "option")
                        {
                            const QXmlStreamAttributes& attributes = reader.attributes();
                            QString color = "black";
                            if (attributes.hasAttribute("color"))
                                color = attributes.value("color").toString();
                            QString title = reader.readElementText(); /// readElementText must be after get attributes
                            m_Logger->log(LogLevel::Info, "QtDataLoader", "Loaded positon name: " + title.toStdString() + ", color: " + color.toStdString());

                            try
                            {
                                m_Entries.push_back({ title.toStdString(), Color(color.toStdString()) });
                            }
                            catch (...)
                            {
                                m_Entries.push_back({ title.toStdString(), g_DafaultColor });
                                m_DataCorrupted = true;
                                m_Logger->log(LogLevel::Warning, "QtDataLoader", "Nothing wrong has happen thanks to default color, but there is issue with given color: " + color.toStdString());

                                try
                                {
                                    throw;
                                }
                                catch (WrongInputException& wie)
                                {
                                    m_ErrorFlags.set(0);
                                    m_Logger->log(LogLevel::Critical, "QtDataLoader", g_WrongInputMsg);
                                }
                                catch (ChannelOutOfRangeException& coore)
                                {
                                    m_ErrorFlags |= bitset<4>(1 << 1);
                                    m_Logger->log(LogLevel::Critical, "QtDataLoader", g_ChannelOutOfRangeMsg);
                                }
                                catch (WrongChannelAmountException& wcae)
                                {
                                    m_ErrorFlags |= bitset<4>(1 << 2);
                                    m_Logger->log(LogLevel::Critical, "QtDataLoader", g_WrongChannelAmountMsg);
                                }
                            }
                        }
                }
                else
                {
                    m_DataCorrupted = true;
                    reader.skipCurrentElement();
                }
        }
        else
        {
            reader.raiseError(QObject::tr("Incorrect file"));
            fullSuccess = false;
        }
    }
    else if (reader.hasError())
    {
        m_Logger->log(LogLevel::Error, "QtDataLoader", reader.errorString().toStdString());
    }

    return fullSuccess;
}
