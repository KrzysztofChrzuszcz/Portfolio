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
    standard = 0,
    linear = 1,
    minstd_rand = 2,
    ranlux24 = 3,
    mt19937 = 4
//https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
};

enum class DataState
{
    NotSelected,
    Selected,
    Loaded,
    Ready
};

class   CustomOpenGlWidget;

/**
 * \brief Settings class to store and configure all program settings.
 * Some of them are accessible by GUI menu (TODO part)
 */
class Settings
{
    friend class    SettingsWidget; // Writer
    friend class    Engine; // Reader/Writer

public:
                    Settings();

    inline float    getMinColorBrightness() const { return m_MinColorBrightness; } //!< Inform about currently set minimum color brightness level used to auto adjust colors
    bool            isDataReady() const noexcept;   //!< Returns true only when loaded data are ready to use
    void            setFilePath(string fileName);   //!< Set selected file path to load attempt
    void            drawLots();                     //!< Initiate fortune draw

private:
    DataState       m_DataState;
    bool            m_DrawLots;
    Lock            m_Lock;

    bool            m_AutoStart;
    bool            m_AutoAdjust;
    string          m_FilePath;
    float           m_MinColorBrightness;
    int             m_ScreenRefreshFrequencyIndex;
    vector<uint>    m_ScreenRefreshFrequencies;
    int             m_MinAngle;
    int             m_MaxAngle; 
    int             m_MaxDurationTime;
    int             m_MinRandRange;
    int             m_MaxRandRange;
    RandMethod      m_RandomMethod;

};
#endif //SETTINGS_H