#include "EdgeEffect.h"

EdgeEffect::EdgeEffect()
{
	// constructor.
	this->init();
	
}

void EdgeEffect::init()
{
	this->m_blurKernelSize = 3;
	this->m_cannyThreshold1 = 10;
	this->m_cannyThreshold2 = 30;
	this->m_cannyApertureSize = 3;
	return;
}

void EdgeEffect::drawEffect()
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

	// blur and call Canny.
	blur(bwFrame, this->m_outFrame, Size(m_blurKernelSize,m_blurKernelSize));
	Canny(this->m_outFrame, this->m_outFrame, m_cannyThreshold1, m_cannyThreshold2, m_cannyApertureSize); 

	return;
}

void EdgeEffect::togglePresets()
{
	bool bColorMap = (this->m_bApplyColorMap)?false:true;
	this->setColorMapApply(bColorMap);
}