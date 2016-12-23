#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
	// set up background substraction.
	this->m_bgSubstractor = new BackgroundSubtractorMOG2();

	// set up color maps
	this->m_currentColorMap = 0;
	this->m_bApplyColorMap = false;
}

void AbstractEffect::addNewFrame(Mat frame)
{
	this->m_lastFrame = frame;
}

Mat AbstractEffect::getOutFrame()		// obtain the latest graphic
{
	if(this->m_bApplyColorMap)
	{
		// apply color map.
		applyColorMap(this->m_outFrame, this->m_outFrame, this->m_currentColorMap);
	}

	//  copy to the output.
	return this->m_outFrame;
}

void AbstractEffect::changeColorMap()
{
	this->m_currentColorMap++;
	if( this->m_currentColorMap < 0 || this->m_currentColorMap >= DW_MAX_COLORMAPS)
		this->m_currentColorMap = 0;
}

void AbstractEffect::toggleColorMapsOn()
{
	this->m_bApplyColorMap = this->m_bApplyColorMap ? false : true;
}

void AbstractEffect::setColorMapApply(bool bColorMapApply)
{
	this->m_bApplyColorMap = bColorMapApply;
}

void AbstractEffect::setOutFrameToBlack()
{
	// create a black background.
	this->m_outFrame = Mat(this->m_lastFrame.rows, this->m_lastFrame.cols, this->m_lastFrame.type(), Scalar(0,0,0));
}

void AbstractEffect::doBGSubtraction()
{
	this->m_bgSubstractor->operator()(this->m_lastFrame, this->m_fgMask);
}

