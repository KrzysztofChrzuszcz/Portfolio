#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "ui_settings.h"
#include "settings.h"

#include <QDialog>

namespace Ui {
class Settings;
}

class SettingsWidget : public QDialog
{
    Q_OBJECT

public:
    explicit                                SettingsWidget(Settings& settings, QWidget *parent = nullptr);
                                            ~SettingsWidget();

    inline Settings&                        getSettings() { return m_Settings; } //!< Getter defined in the header to inline it in order to reduce the overhead associated with function calls, such as saving and restoring registers, stack management

private:
    void                                    connectControllers();               //!< Connect GUI controllers signals with private slots methods
    void                                    disconnectControllers();            //!< Disconnects all current connections.
    void                                    setInitValues();                    //!< Set initial values for GUI controllers on base of current settings 
    void                                    displayValues();                    //!< Displays current values of controllers on LCD widgets

   /*!
    * Set of setter slots to configure settings in real time
    */
private slots:
    void                                    setAutoStart(int state);
    void                                    setAutoAdjust(int state);
    void                                    setRandMethod(int index);
    void                                    setRefreshFrequency(int index);
    void                                    setMinBrightness(int value);
    void                                    setMaxTime(int value);
    void                                    setMinPieAngle(int value);
    void                                    setMaxPieAngle(int value);
    void                                    setRandMin(int value);
    void                                    setRandMax(int value);

private:
    Ui::Settings*                           m_Ui;                               //!< Graphic User Interface form
    Settings&                               m_Settings;                         //!< Connection with settings
    bool                                    m_Initialized;                      //!< Helper flag to solve issue with reloading connected controller
};

#endif // SETTINGSWIDGET_H
