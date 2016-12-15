#include "TrackingEffect.h"

TrackingEffect::TrackingEffect()
{
	// constructor.
	this->init();
	
}

void TrackingEffect::init()
{
	// initialize the cascades.
	this->m_faceCascade.load("resources/haarcascade_frontalface_alt2.xml");
	this->m_eyesCascade.load("resources/haarcascade_eye_tree_eyeglasses.xml");

	// set default for colormaps not to be used, but this can be changed in presets.
	this->m_bApplyColorMap = false;
	
	return;
}

void TrackingEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "TrackingEffect Warning: Last frame is empty." << endl;
		return;
	}
	
	
	this->drawHappyFaces();	

	return;
}

void TrackingEffect::togglePresets()
{
	this->m_bApplyColorMap = this->m_bApplyColorMap ? false:true;
}

void TrackingEffect::drawHappyFaces()
{
	//initialize outframe
	this->m_lastFrame.copyTo(this->m_outFrame);

	// create a grayscale image to process.
	Mat bwFrame;
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);
	equalizeHist(bwFrame, bwFrame);

	// detect faces.
	this->m_faces.clear();
	this->m_faceCascade.detectMultiScale(bwFrame, this->m_faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));
	
	// draw yellow circles for the faces.
	for( int i=0; i< this->m_faces.size(); i++ )
	{
		// this is the center & radius of the face.
		Point center( m_faces[i].x + m_faces[i].width*0.5, m_faces[i].y + m_faces[i].height*0.5 );
		int radius = max( m_faces[i].width, m_faces[i].height);
		// draw a yellow round circle for the happy face.
		circle( this->m_outFrame, center, radius, Scalar(0,255,255), -1 /*filled*/, 8, 0);
		// draw the smile.
		ellipse( this->m_outFrame, center, Size(0.75*radius, 0.75*radius), 0, 45, 135, Scalar(0,0,0), 4 /* thick line*/, 8, 0);

		// for each face search for the eyes.
		Mat faceROI = bwFrame(m_faces[i]);
		this->m_eyes.clear();
		this->m_eyesCascade.detectMultiScale(faceROI, m_eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

		// draw black circles for the eyes.
		for( int j=0; j < m_eyes.size(); j++ )
		{
			Point center( m_faces[i].x + m_eyes[j].x + m_eyes[j].width*0.5, m_faces[i].y + m_eyes[j].y + m_eyes[j].height*0.5);
			int radius = cvRound( (m_eyes[j].width + m_eyes[j].height) * 0.25);
			circle( this->m_outFrame, center, radius, Scalar(0, 0, 0), -1, 8, 0);
		}
	}
}