#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <vector>
#include <list>
#include <shared_mutex>
#include <ctime>
#include <chrono>

using std::string;
using std::vector;
using std::list;
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

enum class DataState : int
{
    NotSelected = 0,
    Selected = 1,
    Loaded = 2,
    Ready = 3
};

class   CustomOpenGlWidget;

/**
 * \brief Settings class to store and configure all program settings.
 * Some of them are accessible by GUI menu (TODO part)
 */
class Settings
{
    friend class    SettingsManager;    // Settings manage, provide and storage
    friend class    SettingsWidget;     // Settings GUI
    friend class    Engine;             // Main settings user that thay are created for

public:
                    Settings();

    inline float    getMinColorBrightness() const { return m_MinColorBrightness; }  //!< Inform about currently set minimum color brightness level used to auto adjust colors
    std::time_t     getTimestamp() const noexcept;                                  //!< Return time of last settings change
    bool            hasChanged(const std::time_t& since) const noexcept;            //!< Checks if given timestamp is deferent from last change own timestamp

    // Data dump and load
    string          toJSON() const noexcept;                                        //!< Serialize settings to JSON form
    void            fromJSON(const string& input);                                  //!< Gets input JSON, parse it and set
    list<string>    serialize() const noexcept;                                     //!< Return serialized settings
    void            deserializeAndSet(list<string>& input);                         //!< Gets input settings, deserialize it and set
    void            validate() const;                                               //!< Checks if all settings are in proper range according to SettingsWidget form configuration

private:
    void            setDefaultValues() noexcept;                                    //!< Setup default values in case of any problems with storage data
    void            updateLastChangeTime() noexcept;                                //!< Update timestemp of last settings change
            
private:
    Lock            m_Lock; /// not serialized
    DataState       m_DataState; /// not serialized
    std::time_t     m_LastChangeTime;

    bool            m_DrawLots;
    bool            m_AutoStart;
    bool            m_AutoAdjust;
    string          m_FilePath; /// not serialized
    float           m_MinColorBrightness;
    int             m_ScreenRefreshFrequencyIndex;
    vector<uint>    m_ScreenRefreshFrequencies; /// not serialized
    int             m_MinAngle;
    int             m_MaxAngle; 
    int             m_MaxDurationTime;
    int             m_MinRandRange;
    int             m_MaxRandRange;
    RandGenerator   m_RandomGenerator;
};

#endif //SETTINGS_H