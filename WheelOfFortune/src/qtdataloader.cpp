#include "qtdataloader.h"

#include <QXmlStreamReader>

extern const Color g_DafaultColor;

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
#ifdef DEBUG
        qDebug() << "Cannot read file" << file.errorString();
#endif // DEBUG
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
                            QXmlStreamAttributes& attributes = reader.attributes();
                            QString color = "Qt::black";
                            if (attributes.hasAttribute("color"))
                                color = attributes.value("color").toString();

                            QString title = reader.readElementText();
#ifdef DEBUG
                            qDebug(qPrintable(title));
                            qDebug(qPrintable(color));
#endif // DEBUG
                            try
                            {
                                m_Entries.push_back({ title.toStdString(), Color(color.toStdString()) });
                            }
                            catch (...)
                            {
                                m_Entries.push_back({ title.toStdString(), g_DafaultColor });
                                m_DataCorrupted = true;
#ifdef DEBUG
                                qDebug("Nothing wrong has happen thanks to default color, but there is issue with given color: %s\n", color.toStdString().c_str());
#endif // DEBUG
                                try
                                {
                                    throw;
                                }
                                catch (WrongInputException& wie)
                                {
                                    m_ErrorFlags.set(0);
                                }
                                catch (ChannelOutOfRangeException& coore)
                                {
                                    m_ErrorFlags |= bitset<4>(1 << 1);
                                }
                                catch (WrongChannelAmountException& wcae)
                                {
                                    m_ErrorFlags |= bitset<4>(1 << 2);
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
#ifdef DEBUG      
        qDebug() << reader.errorString();
#endif // DEBUG
        // TODO: log error
    }

    return fullSuccess;
}
