#ifndef DW_WarpEffect
#define DW_WarpEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class WarpEffect : public AbstractEffect {
public:
	WarpEffect();
	void init();
	void drawEffect();
	void togglePresets();

private:
	double m_magnitude;		// magnitude of the sine wave in warp.
	double m_period;		// phase of the sine in warp.			
	double m_periodRate;	// rate at which we change the wave period.

	int m_currentColorMap;  // for chaging the colors.

};
#endif