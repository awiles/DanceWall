#ifndef DW_SimpleMotionTracker
#define DW_SimpleMotionTracker

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include "DWObject.h"

using namespace cv;
using namespace std;

// this class simply tracks things that move.  
// Based on work here: https://github.com/abhi-kumar/OPENCV_MISC/blob/master/track_motion.cpp

class SimpleMotionTracker : public DWObject {
public:
	SimpleMotionTracker();
	~SimpleMotionTracker();
	virtual string const className() { return "SimpleMotionTracker";}
	void setInputImage( Mat input );	// pass in the input image to track motion.
	bool process();						// process the results.
	vector<Point> getMotionCentroids() { return this->m_trackedCentroids;}	// this returns the centroid of the motion segments.

protected:
	// functions.
	void init();

	// variables.
	int m_height;			// image height.
	int m_width;			// image width
	Mat m_inputImage;		// this is the image passed in for processing.
	Mat m_prevFrame;		// previous frame.
	Mat m_frameDiff;		// difference between previous and current.
	Mat m_grayDiff;			// gray scale version of the diff.
	Mat m_motionMask;		// threshold of the grayscale diff.
	Mat m_motionHistory;	
	Mat m_mgMask;
	Mat m_mgOrient;
	Mat m_segMask;
	vector<Rect> m_segBounds;	// this contains the detected segments that have moved.

	vector<Point> m_trackedCentroids; // these are the centroid coordinates of the objects that have moved.
};


#endif