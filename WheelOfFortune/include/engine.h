#ifndef ENGINE_H
#define ENGINE_H
// https://surfer.nmr.mgh.harvard.edu/ftp/dist/freesurfer/tutorial_versions/freesurfer/lib/qt/qt_doc/html/network-blockingfortuneclient.html

#include "dataloader.h"
#include "wheeloffortune.h"
#include "qtwheeloffortune.h"
#include "mainwindow.h"

#include <memory>
#include <random> 

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

    void                run();                          //!< Main Engine method
    void                setRandomMethod(int index);     //!< Set method to generate random data
    uint                getScreenRefreshFrequency();    //!< Returns current screen refresh frequency from settings

private:
    void                changeState(Stage newState);    //!< Changes engine state
    bool                validate();                     //!< Validates loaded data

    // Example usage of function pointer instead preferred std::function (suits demonstration purpose)
    void                (Engine::*generateRandData)(double&, int&); //!< Function pointer to use selected generate method
    void                generateRandDataWithStandardRand(double& randomAngle, int& durationInSeconds); //!< Use Standard rand STL method to generate data
    template <typename T>
    void                generateRandDataTemplate(double& randomAngle, int& durationInSeconds) //!< Use given STL generator method to generate data  {Knuth B, Minstd, ranlux24, mt19937, Subtract With Carry Engine}
    {
        ReadLock rLock(m_Settings.m_Lock);

        std::random_device rd;
        T genEngine(rd());
        std::uniform_real_distribution<double> angleDist(m_Settings.m_MinRandRange, m_Settings.m_MaxRandRange);

        if (m_Settings.m_MaxRandRange != m_Settings.m_MinRandRange)
            randomAngle = angleDist(genEngine);
        else
            randomAngle = m_Settings.m_MinRandRange;
        extern const int g_MinimumDurationTime;
        std::uniform_int_distribution<int> durationDist(g_MinimumDurationTime, g_MinimumDurationTime + m_Settings.m_MaxDurationTime);

        durationInSeconds = durationDist(genEngine);
    }
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