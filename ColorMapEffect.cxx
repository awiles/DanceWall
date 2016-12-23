#include "ColorMapEffect.h"

ColorMapEffect::ColorMapEffect()
{
	// constructor.
	this->init();
}

void ColorMapEffect::init()
{
	// nothing to initialize just yet.
	this->m_bApplyColorMap = true;
	return;
}

void ColorMapEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "ColorMapEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	// convert to B&W
	cvtColor(this->m_lastFrame, this->m_outFrame, CV_BGR2GRAY);

	// color map is applied on the way out.

	return;
}

void ColorMapEffect::togglePresets()
{
	// TODO:
}