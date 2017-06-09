#ifndef DW_AbstractEffect
#define DW_AbstractEffect

#define DW_MAX_COLORMAPS 12
#define DW_MAX_GRIDORDER 10 // this means it stops at 9 because grid order is not zero-based.

#include <iostream>
#include <cmath>
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
	Mat getOutFrame();					// obtain the latest graphic
	virtual void togglePresets() = 0;	// toggle to the next set of presets.
	void changeColorMap();				// toggle through the colormaps.
	void toggleColorMapsOn();			// toggle colormaps on/off switch.
	void setColorMapApply(bool bColorMap); // decide whether to use color map.
	void setOutFrameToBlack();			// set outFrame to same size as last and black.
	void setGridOrder(int order);		// set grid order for output grid.
	void incrementGridOrder();
	void computeRandColorMapList();

	// get a random configuration.
	virtual void getRandomConfig(bool doGrid=false);		
	
	// do background subtraction.
	void doBGSubtraction();				// do background substraction.

protected:
	Mat m_lastFrame; // latest frame from the video capture for processing.
	Mat m_outFrame; // this is the frame that will be displayed.

	// background substraction.
	Mat m_fgMask;				// this is the foreground mask generated during background subtraction.
	Ptr<BackgroundSubtractorMOG2> m_bgSubstractor;  // this is the actual background subtractor.

	// some colormap items.
	int m_currentColorMap;		// current colormap being used.
	bool m_bApplyColorMap;		// boolean that decides whether we apply the color map.

	// image grid -- create a grid of images
	//		- option 1: same image different colour maps.
	//		- option 2: series of images in a grid.
	int m_gridOrder;			// 1x1, 2x2, 3x3, etc.
	vector<int> m_randColorMapList;	// order^2 maps.
};
#endif