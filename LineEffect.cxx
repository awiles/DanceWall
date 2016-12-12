#include "LineEffect.h"

LineEffect::LineEffect()
{
	// constructor.
	this->init();
}

void LineEffect::init()
{
	this->m_effectOption = DW_LE_HOUGHLINEP;
	return;
}

void LineEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "EdgeEffect Warning: Last frame is empty." << endl;
		return;
	}

	Mat bwFrame, blurFrame, edgeFrame, outFrame;

	// create a black background.
	outFrame = Mat(this->m_lastFrame.rows, this->m_lastFrame.cols, this->m_lastFrame.type(), Scalar(0,0,0));

	// covert to B&W and blur
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);
	// blur
	medianBlur(bwFrame, blurFrame, 3);
	// do edge detection
	Canny(blurFrame, edgeFrame, 50, 200, 3);

	if( this->m_effectOption == DW_LE_HOUGHLINE )
	{
		//	C++ implementation was figured out here: http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html

		vector<Vec2f> lines;

		// get Hough lines.
		HoughLines(edgeFrame, lines, 1, CV_PI/180.0, 100, 0, 0);

		if( lines.size() > 0)
		{
			cout << "lines has " << lines.size() << " items "  << endl;
			for( size_t i=0; i<lines.size(); i++)
			{
				float rho = lines[i][0];
				float theta = lines[i][1];
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				pt1.x = cvRound(x0 + 1000*(-b));
				pt1.y = cvRound(y0 + 1000*(a));
				pt2.x = cvRound(x0 - 1000*(-b));
				pt2.y = cvRound(y0 - 1000*(a));
				line( outFrame, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
			}
		}
	}
	else if( this->m_effectOption == DW_LE_HOUGHLINEP )
	{
		// Line Effect: http://www.magicandlove.com/blog/2015/07/18/opencv-and-processing-18/ which uses Processing.org. 
		//	but the C++ implementation was figured out here: http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html

		vector<Vec4i> lines;
		HoughLinesP(edgeFrame, lines, 1, CV_PI/180, 50, 50, 10 );
		for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			line( outFrame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3, CV_AA);
		}
	}
	else
	{
		this->m_effectOption = DW_LE_HOUGHLINEP;
	}

	outFrame.copyTo(this->m_outFrame);
	return;
}

void LineEffect::togglePresets()
{
	//TODO: set-up code to toggle presets.
}