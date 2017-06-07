#include "WarpEffect.h"

WarpEffect::WarpEffect()
{
	// constructor.
	this->init();
}

void WarpEffect::init()
{
	// for the linear waves.
	this->m_magnitude = 5.0;
	this->m_period = 5.0;
	this->m_periodRate = 0.1;

	// for the swirl.
	this->m_strength = 0.1;
	this->m_radius = 120;
	this->m_rotation = 0;

	// other general
	this->m_currentColorMap = 4;
	this->m_currentWarpType = 0;
	return;
}

void WarpEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "WarpEffect Warning: Last frame is empty." << endl;
		return;
	}
	// let's draw the item here.
	Mat bwFrame;
	Mat colFrame;
	// for loops and other general.
	int rows = this->m_lastFrame.rows;
	int cols = this->m_lastFrame.cols;

	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);

	// apply color map.
	applyColorMap(bwFrame, colFrame, this->m_currentColorMap);

	// map functions.
	Mat srcX(rows, cols, CV_32F);
	Mat srcY(rows, cols, CV_32F);

	// helper variables.
	float ci,cj;		// image center
	float x,y;			// current xy coord 
	float rho,phi;		// polar coord.
	float dx, dy;		// distortion.
	float phiPrime;		// new angle in polar.
	float xPrime, yPrime; // new coordinates.
	float swirlRadius = (log(2) * this->m_radius)/1;
	float mx,my;		// new mapping

	// define image origin.
	ci = rows/2;
	cj = cols/2;

	// compute the rho boundary
	float rhoBound = 0.95 * min(ci,cj);  // take 95% of dimension
	//TODO: test using ellipse instead of circle, e.g. mahalanobis distance.

	// create mapping
	for( int i=0; i<rows; i++)
	{
		for( int j=0; j<cols; j++)
		{
			// get cart coords.
			x = j - cj;
			y = i - ci;
			
			// get polar coordinates.
			rho = sqrt(x*x + y*y);
			phi = atan2(y,x);

			// if outside don't apply the filter.
			if( rho > rhoBound)
			{
				srcX.at<float>(i,j) = (float)j; // remain on same column.
				srcY.at<float>(i,j) = (float)i; // remain on same row.
			}
			else
			{
				float rhoFactor = 1 - pow(rho,2)/pow(rhoBound,2); 

				switch (this->m_currentWarpType)
				{
				case DW_Warp::horizontal:
					// horizontal wave.
					mx = (float)j; // remain on same column.
					my = i + rhoFactor * this->m_magnitude * sin( j/this->m_period ); // pixels origionally on row i are now moved following the sinusoid.
					break;
				case DW_Warp::vertical:
					// vertical wave.
					mx = j + rhoFactor * this->m_magnitude * sin( i/this->m_period ); // pixels origionally on column j are now moved following the sinusoid.
					my = (float)i; // remain on same row.
					break;
				case DW_Warp::hv:
					// combined vertical and horizontal wave.
					mx = j + rhoFactor * this->m_magnitude * sin( i/this->m_period ); // pixels origionally on column j are now moved following the sinusoid.
					my = i + rhoFactor * this->m_magnitude * sin( j/this->m_period ); // pixels origionally on row i are now moved following the sinusoid.
					break;
				case DW_Warp::swirl:
					// compute new angle coordinate.
					phiPrime = this->m_rotation + this->m_strength 
						* exp(-1*rho/swirlRadius + phi);
					// unwrap back into cartesian & compute distortion.
					dx = rho * cos(phiPrime) - x;
					dy = rho * sin(phiPrime) - y;
					// compute the map.
					mx = j + rhoFactor * dx;
					my = i + rhoFactor * dy;
					break;
				default:
					// do nothing -- technically should not get here.
					this->printError("Hit the default item in warp effect.  Should not be able to get here.");
					mx = (float)j; // remain on same column.
					my = (float)i; // remain on same row.
					break;
				}
				srcX.at<float>(i,j) = mx; 
				srcY.at<float>(i,j) = my;
			}


		}
	}

	remap(colFrame, this->m_outFrame, srcX, srcY, INTER_LINEAR );

	// change the period to create wave effect.
	this->nextTwoWayParm(&this->m_period, &this->m_periodRate, 2.0, 15.0);

	return;
}

void WarpEffect::togglePresets()
{
	this->m_currentWarpType++;
	if( this->m_currentWarpType < 0 || this->m_currentWarpType >= DW_WE_MAXWARPEFFECTS)
		this->m_currentWarpType = 0;
}