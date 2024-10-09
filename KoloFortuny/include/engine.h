#ifndef ENGINE_H
#define ENGINE_H
// https://surfer.nmr.mgh.harvard.edu/ftp/dist/freesurfer/tutorial_versions/freesurfer/lib/qt/qt_doc/html/network-blockingfortuneclient.html

#include "dataloader.h"
#include "wheeloffortune.h"
#include "qtwheeloffortune.h"
#include "mainwindow.h"

#include <memory>

class Engine // : public WorkingThread (albo raczej zwykly i jego zawartosc dac tutaj)
{
	enum class Stage
	{
		Iddle = 0,
		DataLoading = 1,
		Processing = 2,
		FortuneDraw = 3, // pseudoloswe / z silna entropia (na bazie std::normal_distribution / std::gamma_distribution / std::uniform_int_distribution) {selektor w menu GUI}
		Animation = 4,
		SlowingDown = 5,
		SumUp = 6 // napis z winikiem (+animacja znikania za pomoca kanalu alfa), confetti gif, fanfary (w sumie play jakiegos dzwieku na oba sposoby bylby ciekawy)
	};

public:
						Engine(MainWindow&, DataLoader&);
						~Engine();
	void				run();
	//uint				getScreenRefreshFrequency(); TODO

private:
	void				changeState(Stage newState);
	void				waitForOrder();
	void				loadData();
	void				processData();
	void				fortuneDraw();
	void				animate();
	void				animationExtinguishing();
	void				sumUp();
	//...


private:
	Stage				m_Stage;
	DataLoader&			m_DataLoader;
	Settings&			m_Settings;
	std::weak_ptr<CustomOpenGlWidget> 
						m_OpenGlWidget;
	WheelOfFortune*		m_WheelOfFortune;

	int					m_FastAnimationAngle; // Whole angle to rotate during fast animation
	int					m_SlowingDownAngle; // Whole angle to rotate during extinguishing animation
	int					m_CurrentAngle;
	int					m_Step;
};



#endif //ENGINE_H