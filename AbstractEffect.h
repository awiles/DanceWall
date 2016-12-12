#ifndef DW_AbstractEffect
#define DW_AbstractEffect

#define DW_MAX_COLORMAPS 12

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class AbstractEffect {
public:
	AbstractEffect();
	
	virtual void init() = 0;			// initialize the set-up if needed.
	void addNewFrame(Mat *frame); 		// copy in the latest frame	
	virtual void drawEffect() = 0;		// draw the intended effect.
	void getOutFrame(Mat *frame);		// obtain the latest graphic
	virtual void togglePresets() = 0;	// toggle to the next set of presets.
	void toggleColorMaps();				// toggle through the colormaps.
	void setColorMapApply(bool bColorMap); // decide whether to use color map.

protected:
	Mat m_lastFrame; // latest frame from the video capture for processing.
	Mat m_outFrame; // this is the frame that will be displayed.

	int m_currentColorMap;		// current colormap being used.
	bool m_bApplyColorMap;		// boolean that decides whether we apply the color map.

};
#endif