#include "MotionFlowEffect.h"

MotionFlowEffect::MotionFlowEffect()
{
	// constructor.
	
}

void MotionFlowEffect::init()
{
	// nothing to initialize just yet.
	return;
}

void MotionFlowEffect::drawEffect()
{
	
	if( this->m_lastFrame.empty() )
	{
		cout << "MotionFlowEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	if( this->m_previousFrame.empty())
	{
		// initialize for the first one.
		cvtColor(this->m_lastFrame, this->m_previousFrame, COLOR_BGR2GRAY);
	}

	// intermediate variables.
	Mat flow;
	Mat hsvComponents[3];
	
	// loop through and initialize hsvComponents.
	for( int i=0; i<3; i++)
	{
		hsvComponents[i] = Mat::zeros(this->m_lastFrame.rows, this->m_lastFrame.cols, CV_32F);
	}

	// initialize saturation channel to 255
	hsvComponents[1] = Scalar(255);	
	
	// set next frame 
	cvtColor(this->m_lastFrame, this->m_nextFrame, COLOR_BGR2GRAY);
	medianBlur(this->m_nextFrame, this->m_nextFrame, 25);
	calcOpticalFlowFarneback(this->m_previousFrame, this->m_nextFrame, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

	// split the array from flow.
	Mat flowSplit[2];
	split(flow, flowSplit);

	cout << "Number of channels in flow: " << flow.channels() << endl;
	// convert from cartesion to polar.
	Mat mag, ang;
	cartToPolar(flowSplit[0], flowSplit[1], mag, ang);
	hsvComponents[0] = ang * 180/CV_PI/2;
	normalize(mag, hsvComponents[2], 0, 255, NORM_MINMAX);

	Mat hsvOut;
	merge(hsvComponents, 3, hsvOut);
	cvtColor(hsvOut, this->m_outFrame, CV_HSV2BGR);

		// copy the next frame to the previous.
	this->m_nextFrame.copyTo(this->m_previousFrame);

	return;
}

void MotionFlowEffect::togglePresets()
{
	//TODO: set-up code to toggle presets.
}