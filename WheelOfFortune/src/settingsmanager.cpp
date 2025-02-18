#include "settingsmanager.h"

//const std::string g_SettingsDumpFileName("settings.csv"); // Other way of data storage, left to highlight the issues of extensibility
const std::string g_SettingsDumpFileName("settings.json");

SettingsManager::SettingsManager(std::shared_ptr<ILogger> logger) :
    m_Logger(std::move(logger)),
    m_DataProvider(g_SettingsDumpFileName)
{
}

SettingsManager::~SettingsManager()
{
    dump();
}

void SettingsManager::dump()
{
    m_DataProvider.dump(m_Settings.toJSON());
}

void SettingsManager::load()
{
    // Example usage of loading dumped data from CVS file
    //std::list<std::string> serializedSettings;
    //if(m_DataProvider.load(serializedSettings))
    //    m_Settings.deserializeAndSet(serializedSettings);
    
    // Better way
    string json;
    if (m_DataProvider.load(json))
    {
        try
        {
            m_Settings.fromJSON(json);  
        }
        catch (const std::exception& e)
        {
            m_Logger->Log(LogLevel::Error, "SettingsManager", "Serious issue with settings.json file! Data corrupted.");
            return;
        }
    }
}

bool SettingsManager::isDataReady() const noexcept
{
    return m_Settings.m_DataState == DataState::Ready;
}

void SettingsManager::setFilePath(string fileName) noexcept
{
    if (fileName.empty())
        return;

    /// It is forbidden to modify state and name in consequence during data validation and processing
    if (m_Settings.m_DataState == DataState::NotSelected || m_Settings.m_DataState == DataState::Ready)
    {
        WriteLock wLock(m_Settings.m_Lock);
        m_Settings.m_FilePath = fileName;
        m_Settings.m_DataState = DataState::Selected;
    }
}

void SettingsManager::setDrawLots() noexcept
{
    WriteLock wLock(m_Settings.m_Lock);
    m_Settings.m_DrawLots = true;
}

std::time_t SettingsManager::getTimestamp() const noexcept
{
    return m_Settings.getTimestamp();
}

bool SettingsManager::hasChanged(const std::time_t& since) const noexcept
{
    return m_Settings.hasChanged(since);
}
