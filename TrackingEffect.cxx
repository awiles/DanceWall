#include "TrackingEffect.h"

TrackingEffect::TrackingEffect()
{
	// constructor.
	this->init();

}

void TrackingEffect::init()
{
	// initialize the sub-effect.
	this->m_subEffect = 0;

	// initialize the cascades.
	this->m_faceCascade.load("resources/haarcascade_frontalface_alt2.xml");
	this->m_eyesCascade.load("resources/haarcascade_eye_tree_eyeglasses.xml");
	this->m_palmCascade.load("resources/haarcascade_palm.xml");
	this->m_closedPalmCascade.load("resources/haarcascade_closed_frontal_palm.xml");
	this->m_fistCascade.load("resources/haarcascade_fist.xml");

	// set default for colormaps not to be used, but this can be changed in presets.
	this->m_bApplyColorMap = false;

	this->m_debug = true;

	return;
}

void TrackingEffect::drawEffect()
{
	if( this->m_lastFrame.empty() )
	{
		cout << "TrackingEffect Warning: Last frame is empty." << endl;
		return;
	}

	switch(this->m_subEffect) {
	case 0:
		this->drawHappyFaces();	
		break;
	case 1:
		this->drawMotion();
		break;
	case 2:
		this->drawBGSubtraction();
		break;
	case 3: 
		this->drawHandTracking();
		break;
	case 4:
		this->drawHandSpots();
		break;
	default:
		cout << "TrackingEffect Error: Incorrect subeffect attempted." << endl;
		break;
	}

	return;
}

void TrackingEffect::togglePresets()
{
	this->m_subEffect++;

	if( this->m_subEffect >= DW_TE_MAXSUBEFFECTS)
		this->m_subEffect = 0;
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

void TrackingEffect::drawHandSpots()
{
	//initialize outframe
	this->m_lastFrame.copyTo(this->m_outFrame);

	// create a grayscale image to process.
	Mat bwFrame;
	cvtColor(this->m_lastFrame, bwFrame, CV_BGR2GRAY);
	equalizeHist(bwFrame, bwFrame);

	// detect palms.
	this->m_palms.clear();
	this->m_palmCascade.detectMultiScale(bwFrame, this->m_palms, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));

	// draw pink circles for the hands.
	for( int i=1; i<this->m_palms.size(); i++ )
	{
		// this is the center & radius of the hand.
		Point center( m_palms[i].x + m_palms[i].width*0.5, m_palms[i].y + m_palms[i].height*0.5 );
		int radius = max( m_palms[i].width, m_palms[i].height);
		// draw a pink round circle for the hands.
		circle( this->m_outFrame, center, radius, Scalar(213,0,255), -1 /*filled*/, 8, 0);
	}

	//// detect closed palms.
	//this->m_closedPalms.clear();
	//this->m_closedPalmCascade.detectMultiScale(bwFrame, this->m_closedPalms, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));

	//// draw blue circles for the hands.
	//for( int i=1; i<this->m_closedPalms.size(); i++ )
	//{
	//	// this is the center & radius of the hand.
	//	Point center( m_closedPalms[i].x + m_closedPalms[i].width*0.5, m_closedPalms[i].y + m_closedPalms[i].height*0.5 );
	//	int radius = max( m_closedPalms[i].width, m_closedPalms[i].height);
	//	// draw a pink round circle for the hands.
	//	circle( this->m_outFrame, center, radius, Scalar(255,0,0), -1 /*filled*/, 8, 0);
	//}

	// detect closed palms.
	this->m_fists.clear();
	this->m_fistCascade.detectMultiScale(bwFrame, this->m_fists, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30,30));

	// draw red circles for the fists.
	for( int i=1; i<this->m_fists.size(); i++ )
	{
		// this is the center & radius of the hand.
		Point center( m_fists[i].x + m_fists[i].width*0.5, m_fists[i].y + m_fists[i].height*0.5 );
		int radius = max( m_fists[i].width, m_fists[i].height);
		// draw a pink round circle for the hands.
		circle( this->m_outFrame, center, radius, Scalar(0,0,255), -1 /*filled*/, 8, 0);
	}

}

void TrackingEffect::drawBGSubtraction()
{
	// initilize out frame to black.
	this->doBGSubtraction();
	this->m_fgMask.copyTo(this->m_outFrame);
}

void TrackingEffect::drawHandTracking()
{
	this->m_handTracker.setInputImage(this->m_lastFrame);
	this->m_handTracker.process();
	this->m_outFrame = this->m_handTracker.getOutputImage();
}

void TrackingEffect::drawMotion()
{
	this->m_motionTracker.setInputImage(this->m_lastFrame);
	this->m_motionTracker.process();
	vector<Point> tmp = this->m_motionTracker.getMotionCentroids();

	/*this->m_outFrame = this->m_lastFrame.clone();

	// draw some circles on the image where motion is detected.
	for( int i= 0; i<tmp.size(); i++)
	{
		circle(this->m_outFrame, tmp[i], 10, Scalar(255, 0, 0), 3 );
	}*/

	this->m_motionEffect.addSeedPoints(tmp);
	this->m_motionEffect.process();
	this->m_outFrame = this->m_motionEffect.getOutputImage();
}