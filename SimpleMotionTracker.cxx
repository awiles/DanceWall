#include "SimpleMotionTracker.h"

SimpleMotionTracker::SimpleMotionTracker()
{
	this->m_height = 0;
	this->m_width = 0;


}

SimpleMotionTracker::~SimpleMotionTracker()
{
}

void SimpleMotionTracker::setInputImage(Mat input)
{
	this->m_inputImage = input.clone();
}

void SimpleMotionTracker::init()
{
	int h = this->m_inputImage.rows;
	int w = this->m_inputImage.cols;
	this->m_height = h;
	this->m_width = w;
	this->m_prevFrame = this->m_inputImage.clone();
	this->m_motionHistory = Mat::Mat(h, w, CV_32FC1, Scalar(0,0,0) );
	this->m_mgMask = Mat::Mat(h, w, CV_8UC1, Scalar(0,0,0) );
	this->m_mgOrient = Mat::Mat(h, w, CV_32FC1, Scalar(0,0,0) ); 
	this->m_segMask = Mat::Mat(h, w, CV_32FC1, Scalar(0,0,0) );
}

bool SimpleMotionTracker::process()
{
	// do we have valid image data?
	if( this->m_inputImage.empty() )
	{
		this->printError("Input Image is empty.");
		return false;
	}

	// have we initialized?
	if( (this->m_height != this->m_inputImage.rows) || (this->m_width != this->m_inputImage.cols) )
	{
		this->init();
	}


	// create a diff image.
	absdiff(this->m_inputImage, this->m_prevFrame, this->m_frameDiff);
	//this->showDebugImage("Frame Diff", this->m_frameDiff);

	// convert to grayscale.
	cvtColor(this->m_frameDiff, this->m_grayDiff, CV_BGR2GRAY);
	//this->showDebugImage("Gray Diff", this->m_grayDiff);

	// threshold.
	threshold(this->m_grayDiff,this->m_motionMask, 32, 255, 0);
	//this->showDebugImage("Motion Mask", this->m_motionMask);

	// compute timestamp.
	double timestamp = 1000.0 * clock()/CLOCKS_PER_SEC;

	// let's update the motion history, compute the gradient and segment.
	updateMotionHistory(this->m_motionMask, this->m_motionHistory, timestamp, 0.05);
	calcMotionGradient(this->m_motionHistory, this->m_mgMask, this->m_mgOrient, 5, 12500, 3);
	segmentMotion(this->m_motionHistory, this->m_segMask, this->m_segBounds, timestamp, 32);

	// initialize outframe to the gray diff image.
	Mat trackedMotionImage = this->m_frameDiff.clone();
	
	// cycle through the segments and draw rectangles around the ones we find.
	int cnt = 0;
	this->m_trackedCentroids.clear();
	for( int i=0; i<m_segBounds.size(); i++ )
	{
		Rect rec = this->m_segBounds[i];
		if( rec.area() > 5000 && rec.area() < 70000 )
		{
			cnt++;
			this->m_trackedCentroids.push_back(Point(rec.x + rec.height/2, rec.y + rec.width/2));
			rectangle(trackedMotionImage, rec, Scalar(255,255,255), 3);
		}
	}

	//this->showDebugImage("Tracked Motion", trackedMotionImage);

	cout << "Found " << this->m_segBounds.size() << " segments, but filtered down to " << cnt << endl;

	// copy the current frame to previous for next time round.
	this->m_prevFrame = this->m_inputImage.clone();

    return true;
}
