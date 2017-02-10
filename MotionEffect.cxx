#include "MotionEffect.h"

MotionEffect::MotionEffect()
{
	this->m_numRandPoints = 10;
	this->init();
}

MotionEffect::~MotionEffect()
{
}

void MotionEffect::init()
{
	this->m_outImage = Mat::zeros(640,480, CV_8UC3);
	this->m_objectPts.clear();
}

void MotionEffect::addSeedPoints( vector<Point> pts)
{
	objectProps obj;
	RNG rng;
	for( int i=0; i<pts.size(); i++ )
	{
		for( int j=0; j<this->m_numRandPoints; j++)
		{
			obj.pt.x = pts[i].x + rng.gaussian(25.0);
			obj.pt.y = pts[i].y + rng.gaussian(25.0);
			obj.timestamp = 1000.0 * clock()/CLOCKS_PER_SEC;
			obj.colour = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			obj.size = abs(rng.gaussian(5));
			this->m_objectPts.push_front(obj);
		}
	}

	// remove old data.
	double curTime = 1000.0 * clock()/CLOCKS_PER_SEC;

	for( auto iter=this->m_objectPts.begin(); iter != this->m_objectPts.end(); /*no increment*/)
	{
		double diff = curTime - iter->timestamp;
		if( diff > 5.0 )
		{
			iter = this->m_objectPts.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void MotionEffect::process()
{
	this->m_outImage = Mat::zeros(640,480, CV_8UC3);

	// add bubbles.
	for( auto iter=this->m_objectPts.begin(); iter != this->m_objectPts.end(); iter++ )
	{
		circle(this->m_outImage, iter->pt, iter->size, iter->colour, -1 /*filled*/ );
	}
}