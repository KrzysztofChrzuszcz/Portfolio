#ifndef DATALOADER_H
#define DATALOADER_H

#include "color.h"
#include "settings.h"
#include "logger.h"

#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

using std::vector;
using std::string;
using std::bitset;

struct Entry
{
    string  m_LabelName;
    Color   m_Color;
};

/**
 * \brief DataLoader is responsible for load and parse selected input XML file with options to display at WheelOfFortune
 */
class DataLoader
{
public:
                                        DataLoader(std::shared_ptr<ILogger> logger) : m_Logger(std::move(logger)) {}
    virtual                             ~DataLoader() noexcept = default;
    virtual bool                        loadXml(const char* path) = 0; //!< Result informs of success in file open and founding proper structure.
    void                                adjust(const Settings& settings) { std::for_each(m_Entries.begin(), m_Entries.end(), [&](Entry& e) { e.m_Color.adjustBrightness(settings.getMinColorBrightness()); }); } //!< Adjust input data for better visibility. For now I need to adjust only brightness, so it's a good place for simple lambda usage as example.
    const vector<Entry>&                getEntries() noexcept { return m_Entries; } //!< In this case i allow compiler to decide about adding inline to getter
    size_t                              getEntriesCount() noexcept { return m_Entries.size(); } //!< Returns number of entries.
    inline bool                         isCorrupted() const noexcept { return m_DataCorrupted; } //!< Inform about existing some problems with input data
    void                                setErrorFlags(const bitset<4>& mask) noexcept { m_DataCorrupted = true; m_ErrorFlags |= mask; } //!< Set error flags
    void                                setErrorFlag(int position, bool value = true) noexcept { m_DataCorrupted = true; m_ErrorFlags.set(position, value); if (position == 3) m_Logger->Log(LogLevel::Critical, "QtDataLoader", g_WrongPieSettingsMsg); } //!< Set value of flag at given position
    inline const bitset<4>&             getErrorFlags() const noexcept { return m_ErrorFlags; }  //!< Allow to read access to ErrorFlags

protected:
    bool                                m_DataCorrupted = false;    //!< States if exists any problem with input data
    vector<Entry>                       m_Entries;                  //!< Data loaded from XML file.
    bitset<4>                           m_ErrorFlags;               //!< Set of error flags: Unrecognized input type | Channel value is out of range | Channels amount is incorrect | Amount of entries is not possible to visualize within given settings
    std::shared_ptr<ILogger>            m_Logger;                   //!< Common logger for application
};
#endif //DATALOADER_H