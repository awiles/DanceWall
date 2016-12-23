#ifndef DW_AbstractEffect
#define DW_AbstractEffect

#define DW_MAX_COLORMAPS 12

#include <iostream>
#include <opencv2/opencv.hpp>
#include "DWObject.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class AbstractEffect : public DWObject {
public:
	AbstractEffect();
	virtual string const className() = 0; // need class name.
	virtual void init() = 0;			// initialize the set-up if needed.
	void addNewFrame(Mat frame); 		// copy in the latest frame	
	virtual void drawEffect() = 0;		// draw the intended effect.
	Mat getOutFrame();		// obtain the latest graphic
	virtual void togglePresets() = 0;	// toggle to the next set of presets.
	void changeColorMap();				// toggle through the colormaps.
	void toggleColorMapsOn();			// toggle colormaps on/off switch.
	void setColorMapApply(bool bColorMap); // decide whether to use color map.
	void setOutFrameToBlack();			// set outFrame to same size as last and black.
	
	// do background subtraction.
	void doBGSubtraction();				// do background substraction.

protected:
	Mat m_lastFrame; // latest frame from the video capture for processing.
	Mat m_outFrame; // this is the frame that will be displayed.

	// background substraction.
	Mat m_fgMask;				// this is the foreground mask generated during background subtraction.
	Ptr<BackgroundSubtractorMOG2> m_bgSubstractor;  // this is the actual background subtractor.

	// come colormap items.
	int m_currentColorMap;		// current colormap being used.
	bool m_bApplyColorMap;		// boolean that decides whether we apply the color map.

};
#endif