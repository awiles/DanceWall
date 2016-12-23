#ifndef DW_HandTracker
#define DW_HandTracker

#include <iostream>
#include <opencv2/opencv.hpp>
#include "DWObject.h"

using namespace cv;
using namespace std;

// based on the work here: https://github.com/bengal/opencv-hand-detection/blob/master/hand.c
class HandTracker : public DWObject {
public:
	HandTracker();
	~HandTracker();
	virtual string const className() { return "HandTracker";}
	void setInputImage( Mat input );	// pass in the input image to track hands within.
	void process();						// process the results.
	Mat getOutputImage();


protected:
	
	// internal functions
	bool doFilterAndThreshold();		// filter and threshold the image.
	bool doContour();
	// images
	Mat m_inputImage;					// input image where we want to track the hands.
	Mat m_threshImage;					// image after threshold and filtering.
	Mat m_tempImage1;					// bw temp image.
	Mat m_tempImage3;					// temp image for threshold.
	Mat m_contourImage;
	Mat m_outputImage;					// output image for display.

	// contours.
	vector<vector<Point>> m_contours;	// contour points.
	vector<Vec4i> m_hierarchy;			// contour hierarchy.

	// useful parameters.
	Mat m_morphElement;					// structuring element for MorphologyEx
};

#endif