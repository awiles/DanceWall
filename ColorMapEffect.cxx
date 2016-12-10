#include "ColorMapEffect.h"

ColorMapEffect::ColorMapEffect()
{
	// constructor.
	this->init();
}

void ColorMapEffect::init()
{
	// nothing to initialize just yet.
	this->m_currentColorMap = 0;
	return;
}

void ColorMapEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "EdgeEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	// convert to B&W
	Mat bwFrame;
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);

	// apply color map.
	applyColorMap(bwFrame, this->m_outFrame, this->m_currentColorMap);

	return;
}

void ColorMapEffect::togglePresets()
{
	this->m_currentColorMap++;
	if( this->m_currentColorMap < 0 || this->m_currentColorMap >= DW_MAX_COLORMAPS)
		this->m_currentColorMap = 0;
}