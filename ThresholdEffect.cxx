#include "ThresholdEffect.h"

ThresholdEffect::ThresholdEffect()
{
	// constructor.
	this->init();
	
}

void ThresholdEffect::init()
{
	this->m_thresholdLevel = 100;
	this->m_thresholdType = THRESH_BINARY;
	this->m_bApplyColorMap = true;
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
	this->m_thresholdLevel += 50;

	if(this->m_thresholdLevel > DW_MAX_THRESHOLD )
	{
		this->m_thresholdLevel = 50;
	}
}

void ThresholdEffect::getRandomConfig(bool doGrid)
{
	AbstractEffect::getRandomConfig(doGrid);
	
	// if doing grid, always apply colormap.
	if( this->m_gridOrder > 1 )
		this->m_bApplyColorMap = true;

}