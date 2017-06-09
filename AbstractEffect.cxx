#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
	// set up background substraction.
	this->m_bgSubstractor = new BackgroundSubtractorMOG2();

	// set up color maps
	this->m_currentColorMap = 0;
	this->m_bApplyColorMap = false;

	// set up grid order
	this->m_gridOrder = 1;
}

void AbstractEffect::addNewFrame(Mat frame)
{
	this->m_lastFrame = frame;
}

Mat AbstractEffect::getOutFrame()		// obtain the latest graphic
{
	if( this->m_gridOrder == 1 )
	{
		if(this->m_bApplyColorMap)
		{
			// apply color map.
			applyColorMap(this->m_outFrame, this->m_outFrame, this->m_currentColorMap);
		}

		//  copy to the output.
		return this->m_outFrame;
	}

	// build a grid.
	int nImages = pow(this->m_gridOrder, 2);
	
	// resize the base image.
	Mat baseImage, tempImage;
	Size smallSize(this->m_outFrame.cols/this->m_gridOrder, 
		this->m_outFrame.rows/this->m_gridOrder);
	resize(this->m_outFrame, baseImage, smallSize);

	cout << "Small image has size: " << baseImage.cols << " x " << baseImage.rows << endl;
	this->setOutFrameToBlack();
	// add image to grid.
	for( int i=0; i< nImages; i++ )
	{
		// determine x,y location in grid.
		int x = (i % this->m_gridOrder) * smallSize.width;
		int y = (i / this->m_gridOrder) * smallSize.height;

		// select the ROI in the grid.
		Rect roi(x, y, smallSize.width, smallSize.height);

		// optionally apply colormap and copy the image into the ROI.
		baseImage.copyTo(tempImage);
		if(this->m_bApplyColorMap)
		{
			// apply color map.
			applyColorMap(tempImage, tempImage, this->m_randColorMapList[i]);
		}
		tempImage.copyTo(this->m_outFrame(roi));
	}
	return this->m_outFrame;
}

void AbstractEffect::changeColorMap()
{
	this->nextOneWayParm(&this->m_currentColorMap, DW_MAX_COLORMAPS);
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

void AbstractEffect::setGridOrder(int order)
{
	if( order < 1)
	{
		this->printError("setGridOrder(): Grid order was set to an invalid value. Setting to default=1");
		this->m_gridOrder = 1;
	}
	else
		this->m_gridOrder = order;

	this->computeRandColorMapList();
}

void AbstractEffect::incrementGridOrder()
{
	this->nextOneWayParm(&m_gridOrder, DW_MAX_GRIDORDER, 1);
	this->computeRandColorMapList();
}

void AbstractEffect::computeRandColorMapList()
{
	int sz = m_gridOrder*m_gridOrder;
	this->m_randColorMapList.resize(sz);

	for( int i=0; i<sz; i++)
	{
		this->m_randColorMapList[i] = this->getRandInt(DW_MAX_COLORMAPS-1);
	}
}

void AbstractEffect::getRandomConfig(bool doGrid)
{
	// if we want a grid, what order?
	if( doGrid )
		setGridOrder(getRandInt(DW_MAX_GRIDORDER-1, 2));
	else
		setGridOrder(1);

	// do we want the color maps?
	int bColorMaps = getRandInt(10);
	if( bColorMaps < 7 ) 
	{
		//apply color maps.
		setColorMapApply(true);
		this->m_currentColorMap = getRandInt(DW_MAX_COLORMAPS-1);
	}
	else
	{
		setColorMapApply(false);
	}


}

void AbstractEffect::doBGSubtraction()
{
	this->m_bgSubstractor->operator()(this->m_lastFrame, this->m_fgMask);
}

