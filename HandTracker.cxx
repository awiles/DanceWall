#include "HandTracker.h"

HandTracker::HandTracker()
{
	//constructor
	this->m_morphElement = getStructuringElement(CV_SHAPE_RECT, Size(9,9), Point(4,4));

	// set debug for now.
	this->m_debug = true;

}

HandTracker::~HandTracker()
{
	//destructor
	
}

void HandTracker::setInputImage(Mat input)
{
	this->m_inputImage = input.clone();
}

void HandTracker::process()
{
	if (!this->doFilterAndThreshold()) return;
	if(!this->doContour()) return;
}



Mat HandTracker::getOutputImage()
{
	return this->m_outputImage;
}

bool HandTracker::doFilterAndThreshold()
{
	if( this->m_inputImage.empty() )
	{
		printError( "Tried to operate on input image that doesn't exist.");
		this->m_outputImage = Mat::zeros(640,480, CV_8UC3);
		return false;
	}

	// show image if m_debug is set.
	showDebugImage("Input Image", this->m_inputImage);
	
	// soften the image.
	GaussianBlur( this->m_inputImage, this->m_tempImage3, Size(11,11), 0, 0);
	// remove some impulsive noise.
	medianBlur( this->m_tempImage3, this->m_tempImage3, 11);
	// change from BFR to HSV format.
	cvtColor( this->m_tempImage3, this->m_tempImage3, CV_BGR2HSV_FULL);

	// show image if m_debug is set.
	showDebugImage("HSV Image", this->m_tempImage3);

	// apply the threshold to detect skin colors.
	//inRange(this->m_tempImage3, Scalar(0, 55, 90, 255), Scalar(28, 175, 230, 255), this->m_threshImage);
	inRange(this->m_tempImage3, Scalar(0, 48, 80, 255), Scalar(20, 255, 255, 255), this->m_threshImage);

	// show image if m_debug is set.
	showDebugImage("Skin Detection", this->m_threshImage);

	// apply morpohological opening.
	morphologyEx(this->m_threshImage, this->m_threshImage, MORPH_OPEN, this->m_morphElement);
	// smooth again.
	GaussianBlur(this->m_threshImage, this->m_threshImage, Size(3,3), 0, 0);

	// show image if m_debug is set.
	showDebugImage("Threshold Image", this->m_threshImage);
	
	return true;
}

bool HandTracker::doContour()
{
	double area, max_area = 0.0;

	// convert thresh image to grayscale.
	this->m_threshImage.copyTo(this->m_tempImage1);
	//cvtColor(this->m_tempImage1, this->m_tempImage1, CV_HSV2BGR);
	//cvtColor(this->m_tempImage1, this->m_tempImage1, CV_BGR2GRAY);

	// find the contours.
	findContours(this->m_threshImage, this->m_contours, this->m_hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	
	// draw the contours.
	m_contourImage = Mat::zeros(this->m_threshImage.size(), CV_8UC3);
	for(int i=0; i < m_contours.size(); i++)
	{
		drawContours(m_contourImage, m_contours, i, Scalar(255,255,255), 2, 8, m_hierarchy, 0, Point());
	}

	m_contourImage.copyTo(this->m_outputImage);



	return true;
}