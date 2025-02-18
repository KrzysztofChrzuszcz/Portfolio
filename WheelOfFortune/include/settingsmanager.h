#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "settings.h"
#include "dataprovider.h"
#include "logger.h"

/**
 * \brief SettingsManager Class to manage settings and keep continuity between application runs.
 */
class SettingsManager
{
public:
                                    SettingsManager(std::shared_ptr<ILogger> logger);
                                    ~SettingsManager();

    void                            dump(); 
    void                            load();

    bool                            isDataReady() const noexcept;                               //!< Returns true only when loaded data are ready to use
    void                            setFilePath(string fileName) noexcept;                      //!< Set selected file path to load attempt
    void                            setDrawLots() noexcept;                                     //!< Initiate fortune draw
    inline Settings&                getSettings() { return m_Settings; }                        //!< Return settings object
    std::time_t                     getTimestamp() const noexcept;                              //!< Return time of last settings change
    bool                            hasChanged(const std::time_t& since) const noexcept;        //!< Checks if given timestamp is deferent from last change own timestamp

private:
    Settings                        m_Settings;                                                 //!< Instance of settings is storage in SettingsManager who owns it
    JSONDataProvider                m_DataProvider;                                             //!< Settings data are dumped and loaded to keep its continuity
    std::shared_ptr<ILogger>        m_Logger;                                                   //!< Common logger for application

};

#endif // SETTINGSMANAGER_H
