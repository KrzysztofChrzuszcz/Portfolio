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

class DataLoader
{
public:
    virtual bool                        loadXml(const char* path) = 0; /// Result informs of success in file open and founding proper structure.
    virtual void                        setMaxPositions(int maxPositionsAmount) { m_MaxPositionsAmount = maxPositionsAmount; }
    virtual void                        adjust(const Settings& conf) { std::for_each(m_Entries.begin(), m_Entries.end(), [&](Entry& e) { e.m_Color.adjustBrightness(conf.getMinColorBrightness()); }); } /// For now I need to adjust only brightness, so it's a good place for simple lambda usage as example.
    virtual const vector<Entry>&        getEntries() { return m_Entries; } /// In this case i allow compiler to decide about adding inline to getter
    // TODO: void logToFile(); // zapis do pliku wraz z flagami o failu ALE TYLKO jesli dataCorrupted = true
    bool                                isCorrupted() inline const { return m_DataCorrupted; }

protected:
    bool                                m_DataCorrupted;
    int                                 m_MaxPositionsAmount = 0;
    vector<Entry>                       m_Entries; //!< Data loaded from XML file.
    bitset<4>                           m_ErrorFlags; /// Unrecognized input type | Channel value is out of range | Channels amount is incorrect | Amount of entries is not possible to visualize within given settings
};
#endif //DATALOADER_H