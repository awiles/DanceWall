#ifndef DW_DanceWall
#define DW_DanceWall

// different effects.
enum DW { colorImage =0, thresholdEffect, edgeEffect, cartoonEffect, lineEffect, colorMapEffect, warpEffect, motionFlowEffect, trackingEffect};

#include <iostream>
#include <opencv2/opencv.hpp>
#include "CartoonEffect.h"
#include "ColorEffect.h"
#include "ColorMapEffect.h"
#include "EdgeEffect.h"
#include "LineEffect.h"
#include "MotionFlowEffect.h"
#include "ThresholdEffect.h"
#include "TrackingEffect.h"
#include "WarpEffect.h"

using namespace cv;
using namespace std;

// typdef
typedef std::map<int, AbstractEffect*> EffectMap;

//create an abstract class for all the effects I am going to create.
class DanceWall {
public:
	DanceWall();					// constructor.
	~DanceWall();					// destructor.
	bool init();					// initialize the camera.
	bool updateFrame();				// update the current frame.
	bool onKeyPress(int keyPress);	// reacts to key press.  Returns bExit.
	
protected:
	VideoCapture* m_camera;	// this is the video camera.
	Mat m_inFrame;			// latest frame from the video capture for processing.
	Mat m_outFrame;			// this is the frame that will be displayed.
	int m_curEffect;		// this is the current effect.
	EffectMap m_effectMap;	// this is map containing objects of all the different effects.


};
#endif