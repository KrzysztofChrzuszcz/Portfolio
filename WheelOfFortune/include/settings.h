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


enum class RandGenerator : int
{
    standard = 0,                   // Standard LCG (Linear Congruential Generator)
    knuth_b = 1,                    // Knuth B generator, based on the shift register method
    minstd_rand = 2,                // minstd_rand generator (LCG - Linear Congruential Generator)
    ranlux24 = 3,                   // Ranlux24 generator (high quality, 24-bit precision)
    mt19937 = 4,                    // Mersenne Twister generator (good general-purpose generator)
    subtract_with_carry_engine = 5  // Subtract with Carry generator (numerical method)
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

    inline float    getMinColorBrightness() const { return m_MinColorBrightness; }
    inline bool*    getDataReady() const { return &m_DataReady; }
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
    RandGenerator   m_RandomGenerator;

    Lock            m_Lock;
};
#endif //SETTINGS_H