#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include <shared_mutex>

using std::string;
using std::vector;
using std::shared_mutex;

typedef std::shared_mutex Lock;
typedef std::unique_lock< Lock >  WriteLock;
typedef std::shared_lock< Lock >  ReadLock;
typedef unsigned int uint;


enum class RandMethod // ?Entropy // TODO2: maybe give seed to choose instead, or too
{
    standart = 0,
    linear = 1,
    minstd_rand = 2,
    ranlux24 = 3,
    mt19937 = 4
//https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
};

class   CustomOpenGlWidget;

/**
 * \brief Settings class to store and configure all program settings.
 * Some of them are accessible by GUI menu (TODO part)
 */
class Settings /* TODO : public QWidget*/
{
    friend class    SettingsWidget; // Writer
    friend class    Engine; // Reader/Writer

public:
                    Settings();

    float           getMinColorBrightness() inline const { return m_MinColorBrightness; }
    bool*           getDataReady() inline const { return &m_DataReady; }
    void            setFilePath(string fileName);
    void            drawLots();    // Initiate fortune draw

private:
    bool            m_DataSelected;
    bool            m_DataProcessed;
    mutable bool    m_DataReady;
    bool            m_DrawLots;
    bool            m_AutoStart;
    bool            m_AutoAdjust;
    string          m_FilePath;
    float           m_MinColorBrightness;
    int             m_ScreenRefreshFrequencyIndex;
    vector<uint>    m_ScreenRefreshFrequencies;
    int             m_MinAngle; // https://stackoverflow.com/questions/17361885/range-slider-in-qt-two-handles-in-a-qslider / https://github.com/ThisIsClark/Qt-RangeSlider
    int             m_MaxAngle; // super / range slider for both BUT In first version two sliders
    int             m_MaxPositionsAmount; 
    int             m_MaxDurationTime;
    int             m_MinRandRange;
    int             m_MaxRandRange; // super / range slider for both
    RandMethod      m_RandomMathod;

    Lock            m_Lock;
};
#endif //SETTINGS_H