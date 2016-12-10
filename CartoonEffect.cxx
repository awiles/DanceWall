#include "CartoonEffect.h"

CartoonEffect::CartoonEffect()
{
	// constructor.
	this->m_numDown = 2;
	this->m_numBilateral = 7;
	this->m_blurKernelSize = 7;
}

void CartoonEffect::init()
{
	// nothing to initialize just yet.
	return;
}

void CartoonEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "CartoonEffect Warning: Last frame is empty." << endl;
		return;
	}
	// // Cartoon Effect: http://www.askaswiss.com/2016/01/how-to-create-cartoon-effect-opencv-python.html
	// vars for the effect.
	
	Mat colorFrame, bwFrame, blurFrame, edgeFrame, tempImg;

	// down sample using Gaussian pyramid.
	this->m_lastFrame.copyTo(colorFrame);
	for( int i=0; i<this->m_numDown; i++)
	{
		pyrDown(colorFrame, colorFrame);
	}

	// repeatedly apply small bilateral filer instead of applying one large filter.
	for( int i=0; i<this->m_numDown; i++)
	{
		bilateralFilter(colorFrame, tempImg, 9, 9, 7);
		tempImg.copyTo(colorFrame);
	}
	

	// upsample back to original size
	for( int i=0; i<this->m_numDown; i++)
	{
		pyrUp(colorFrame, colorFrame);
	}

	// covert to B&W and blur
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);
	medianBlur(bwFrame, blurFrame, this->m_blurKernelSize);

	// detect edges.
	adaptiveThreshold(blurFrame, edgeFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);

	// convert back to color.
	cvtColor(edgeFrame, edgeFrame, COLOR_GRAY2RGB);

	// combine two images.
	bitwise_and(colorFrame, edgeFrame, this->m_outFrame);
	
	return;
}

void CartoonEffect::togglePresets()
{
	//TODO: set-up code to toggle presets.
}