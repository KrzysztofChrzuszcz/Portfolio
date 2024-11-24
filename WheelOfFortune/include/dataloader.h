#ifndef DATALOADER_H
#define DATALOADER_H

#include "color.h"
#include "settings.h"

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
    virtual                             ~DataLoader() = default;
    virtual bool                        loadXml(const char* path) = 0; //!< Result informs of success in file open and founding proper structure.
    void                                setMaxPositions(int maxPositionsAmount) { m_MaxPositionsAmount = maxPositionsAmount; } //!< Sets maximum amount of positions that can be display.
    void                                adjust(const Settings& conf) { std::for_each(m_Entries.begin(), m_Entries.end(), [&](Entry& e) { e.m_Color.adjustBrightness(conf.getMinColorBrightness()); }); } //!< Adjust input data for better visibility. For now I need to adjust only brightness, so it's a good place for simple lambda usage as example.
    const vector<Entry>&                getEntries() { return m_Entries; } //!< In this case i allow compiler to decide about adding inline to getter
    // TODO: void logToFile(); // zapis do pliku wraz z flagami o failu ALE TYLKO jesli dataCorrupted = true
    inline bool                         isCorrupted() const { return m_DataCorrupted; } //!< Inform about existing some problems with input data
    inline const bitset<4>&             getErrorFlags() const { return m_ErrorFlags; }  //!< Allow to read access to ErrorFlags

protected:
    bool                                m_DataCorrupted = false;    //!< States is exists any problem with input data
    int                                 m_MaxPositionsAmount = 0;   //!< Maximum amount of positions that can be load and display.
    vector<Entry>                       m_Entries;                  //!< Data loaded from XML file.
    bitset<4>                           m_ErrorFlags;               //!< Set of error flags: Unrecognized input type | Channel value is out of range | Channels amount is incorrect | Amount of entries is not possible to visualize within given settings
};
#endif //DATALOADER_H