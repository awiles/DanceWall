#ifndef DW_WarpEffect
#define DW_WarpEffect

// different warp effects.
#define DW_WE_MAXWARPEFFECTS 4
enum DW_Warp {horizontal=0, vertical, hv, swirl};

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class WarpEffect : public AbstractEffect {
public:
	WarpEffect();
	virtual string const className() { return "WarpEffect";}
	void init();
	void drawEffect();
	void togglePresets();
	virtual void getRandomConfig(bool doGrid=false);

private:
	// for the linear waves
	double m_magnitude;		// magnitude of the sine wave in warp.
	double m_period;		// phase of the sine in warp.			
	double m_periodRate;	// rate at which we change the wave period.

	// for the swirl, see: http://scikit-image.org/docs/dev/auto_examples/transform/plot_swirl.html
	double m_strength;		// indicates the strength of the swirl
	double m_strengthRate;  // rate at which the strength changes.
	double m_radius;		// indicates the swirl extent
	double m_rotation;		// adds a rotation angle to the swirl

	int m_currentColorMap;  // for changing the colors.
	int m_currentWarpType;  // for choosing the warp type, e.g. vertical, horizontal, etc.

};
#endif