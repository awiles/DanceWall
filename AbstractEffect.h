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
	AbstractEffect()
	{
		//TODO: Initialize images.
	}
	virtual void init() = 0;			// initialize the set-up if needed.
	void addNewFrame(Mat *frame) 		// copy in the latest frame
	{
		frame->copyTo(this->m_lastFrame);
		return;
	}
	virtual void drawEffect() = 0;		// draw the intended effect.
	void getOutFrame(Mat *frame)		// obtain the latest graphic
	{
		this->m_outFrame.copyTo(*frame);
	}
	virtual void togglePresets() = 0;				// toggle to the next set of presets.
	

protected:
	Mat m_lastFrame; // latest frame from the video capture for processing.
	Mat m_outFrame; // this is the frame that will be displayed.

};
#endif