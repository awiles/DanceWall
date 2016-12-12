#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
	this->m_currentColorMap = 0;
	this->m_bApplyColorMap = false;
}

void AbstractEffect::addNewFrame(Mat *frame)
{
	frame->copyTo(this->m_lastFrame);
	return;
}

void AbstractEffect::getOutFrame(Mat *frame)		// obtain the latest graphic
{
	if(this->m_bApplyColorMap)
	{
		// apply color map.
		applyColorMap(this->m_outFrame, this->m_outFrame, this->m_currentColorMap);
	}

	//  copy to the output.
	this->m_outFrame.copyTo(*frame);
}

void AbstractEffect::toggleColorMaps()
{
	this->m_currentColorMap++;
	if( this->m_currentColorMap < 0 || this->m_currentColorMap >= DW_MAX_COLORMAPS)
		this->m_currentColorMap = 0;
}

void AbstractEffect::setColorMapApply(bool bColorMapApply)
{
	this->m_bApplyColorMap = bColorMapApply;
}

