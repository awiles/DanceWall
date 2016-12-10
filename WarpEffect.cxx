#include "WarpEffect.h"

WarpEffect::WarpEffect()
{
	// constructor.
	this->init();
}

void WarpEffect::init()
{
	this->m_magnitude = 5.0;
	this->m_period = 5.0;
	this->m_periodRate = 0.1;
	this->m_currentColorMap = 4;
	return;
}

void WarpEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "EdgeEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	Mat bwFrame;
	Mat colFrame;

	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);

	// apply color map.
	applyColorMap(bwFrame, colFrame, this->m_currentColorMap);

	// map functions.
	Mat srcX(this->m_lastFrame.rows, this->m_lastFrame.cols, CV_32F);
	Mat srcY(this->m_lastFrame.rows, this->m_lastFrame.cols, CV_32F);

	// create mapping
	for( int i=0; i<this->m_lastFrame.rows; i++)
	{
		for( int j=0; j<this->m_lastFrame.cols; j++)
		{
			srcX.at<float>(i,j) = (float)j; // remain on same column.
			srcY.at<float>(i,j) = i + this->m_magnitude * sin( j/this->m_period ); // pixels origionally on row i are now moved following the sinusoid.
		}
	}

	remap(colFrame, this->m_outFrame, srcX, srcY, INTER_LINEAR );

	// change the period to create wave effect.
	this->m_period += this->m_periodRate;

	// check for a change in the wave direction.
	if( this->m_period > 15.0 )
		this->m_periodRate = -0.1;
	else if( this->m_period < 2.0 )
		this->m_periodRate = 0.1;

	return;
}

void WarpEffect::togglePresets()
{
	this->m_currentColorMap++;
	if( this->m_currentColorMap < 0 || this->m_currentColorMap >= DW_MAX_COLORMAPS)
		this->m_currentColorMap = 0;
}