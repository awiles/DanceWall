#ifndef DW_MotionEffect
#define DW_MotionEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "DWObject.h"

using namespace cv;
using namespace std;

struct objectProps {
	Point pt;
	Scalar colour;
	int size;
	double timestamp;
	
};

class MotionEffect : public DWObject {
public:
	MotionEffect();
	~MotionEffect();
	virtual string const className() { return "MotionEffect";}
	void init();			// initialize the image and other objects.
	void addSeedPoints( vector<Point> pts);	// add see points.
	void process();
	Mat getOutputImage() {return this->m_outImage.clone();}

protected:
	Mat m_outImage;				// this is the image being created.
	deque<objectProps> m_objectPts;	// this is the list of object points to be drawn.
	
	int m_numRandPoints;		// number of random points to use in effect.
};


#endif