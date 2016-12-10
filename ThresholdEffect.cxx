#include "ThresholdEffect.h"

ThresholdEffect::ThresholdEffect()
{
	// constructor.
	this->m_thresholdLevel = 100;
	this->m_thresholdType = THRESH_TRUNC;
}

void ThresholdEffect::init()
{
	// nothing to initialize just yet.
	return;
}

void ThresholdEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "ThresholdEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	// convert to B&W
	Mat bwFrame;
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);

	// do threshold
	threshold( bwFrame, this->m_outFrame, m_thresholdLevel, 255, m_thresholdType);

	return;
}

void ThresholdEffect::togglePresets()
{
	//TODO: set-up code to toggle presets.
}