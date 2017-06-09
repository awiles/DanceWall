#include "ColorEffect.h"

ColorEffect::ColorEffect()
{
	// constructor.
	this->init();
}

void ColorEffect::init()
{
	this->m_colorType = DW_COLOR_FULL;
	return;
}

void ColorEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "EdgeEffect Warning: Last frame is empty." << endl;
		return;
	}
	
	// black and white.
	if (this->m_colorType == DW_GRAYSCALE)
	{
		cvtColor(m_lastFrame, m_outFrame, CV_BGR2GRAY);
	}
	// pick one color channel.
	else if (this->m_colorType == DW_BLUE_CHANNEL ||
		this->m_colorType == DW_GREEN_CHANNEL ||
		this->m_colorType == DW_RED_CHANNEL)
	{
		// TODO: create a generic function in the parent class to isolate a color channel.
		Mat bgr[3];   //destination array for split images.
		split(this->m_lastFrame, bgr); // split source
		
		// Create an zero pixel image for filling purposes - will become clear later
		// Also create container images for B G R channels as colour images
		Mat empty_image = Mat::zeros(m_lastFrame.rows, m_lastFrame.cols, CV_8UC1);
		Mat result(m_lastFrame.rows, m_lastFrame.cols, CV_8UC3); // notice the 3 channels here!
		
		// Create single channel image
		Mat in[3];
		switch( m_colorType) {
		case DW_BLUE_CHANNEL:
			in[0] = bgr[this->m_colorType];
			in[1] = empty_image;
			in[2] = empty_image;
			break;
		case DW_GREEN_CHANNEL:
			in[0] = empty_image;
			in[1] = bgr[this->m_colorType];
			in[2] = empty_image;
			break;
		case DW_RED_CHANNEL:
			in[0] = empty_image;
			in[1] = empty_image;
			in[2] = bgr[this->m_colorType];
			break;
		}

		int from_to1[] = { 0,0, 1,1, 2,2 };
		mixChannels( in, 3, &result, 1, from_to1, 3 );
		
		// copy to outFrame.
		result.copyTo(this->m_outFrame);
	}
	// else choose the full color image
	else
	{
		this->m_lastFrame.copyTo(this->m_outFrame);
	}
	return;
}

void ColorEffect::togglePresets()
{
	this->nextOneWayParm(&this->m_colorType, DW_GRAYSCALE);
	cout << "Change color type to: " << this->getColorType() << endl;
	return;
}

void ColorEffect::getRandomConfig(bool doGrid)
{
	AbstractEffect::getRandomConfig(doGrid);
	
	// let's get random color type.
	setColorType(getRandInt(DW_COLOR_FULL));

}