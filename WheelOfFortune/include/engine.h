#ifndef ENGINE_H
#define ENGINE_H
// https://surfer.nmr.mgh.harvard.edu/ftp/dist/freesurfer/tutorial_versions/freesurfer/lib/qt/qt_doc/html/network-blockingfortuneclient.html

#include "dataloader.h"
#include "wheeloffortune.h"
#include "qtwheeloffortune.h"
#include "mainwindow.h"

#include <memory>

/**
 * \brief Engine is core class of Wheel Of Fortune project.
 * It is state machine run by fuel (thread) that is responsive to menu buttons signals.
 * Main goal is to prepare and animate the wheel of fortune.
 */
class Engine
{
    enum class Stage
    {
        Idle = 0,
        DataLoading = 1,
        Processing = 2,
        FortuneDraw = 3,
        Animation = 4,
        SlowingDown = 5,
        SumUp = 6
    };

public:
                        Engine(MainWindow&, DataLoader&);
                        ~Engine();

    void                run();
    uint                getScreenRefreshFrequency();

private:
    void                changeState(Stage newState);    //!< Changes engine state
    bool                validate();                     //!< Validates loaded data

    void                waitForOrder();                 //!< Idle state method. Depends on settings and menu buttons
    void                loadData();                     //!< Load and adjust data from selected input file if the one is correct
    void                processData();                  //!< Calculate entries and pass them to main GUI part
    void                fortuneDraw();                  //!< Random draw of rotation and time of execution
    void                animate();                      //!< Fast animation of the wheel
    void                animationExtinguishing();       //!< Slowing down the wheel
    void                sumUp();                        //!< Result presentation within sparks/water feature


private:
    DataLoader&         m_DataLoader;
    Settings&           m_Settings;
    MainWindow&         m_MainWindow;
    Stage               m_Stage;                        //!< Current stage
    std::weak_ptr<CustomOpenGlWidget> 
                        m_OpenGlWidget;                 //!< Widget with OpenGL (canvas)
    WheelOfFortune*     m_WheelOfFortune;               //!< Main part of GUI

    int                 m_FastAnimationAngle;           //!< Whole angle to rotate during fast animation
    int                 m_SlowingDownAngle;             //!< Whole angle to rotate during extinguishing animation
    int                 m_CurrentAngle;                 //!< Current rotation angle of the wheel
    int                 m_Step;                         //!< Step of rotation angle animation while one widget refresh
};

#endif //ENGINE_H