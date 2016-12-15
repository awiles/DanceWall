#include "DanceWall.h"

using namespace cv;
using namespace std;

DanceWall::DanceWall()
{
	// set-up effect objects.
	m_effectMap[DW::colorImage] = new ColorEffect();
	m_effectMap[DW::edgeEffect] = new EdgeEffect();
	m_effectMap[DW::thresholdEffect] = new ThresholdEffect();
	m_effectMap[DW::cartoonEffect] = new CartoonEffect();
	m_effectMap[DW::lineEffect] = new LineEffect();
	m_effectMap[DW::colorMapEffect] = new ColorMapEffect();
	m_effectMap[DW::warpEffect] = new WarpEffect();
	m_effectMap[DW::motionFlowEffect] = new MotionFlowEffect();
	m_effectMap[DW::trackingEffect] = new TrackingEffect();
	m_curEffect = DW::colorImage;

	// set-up the videocapture object.
	m_camera = new VideoCapture(0);
}

DanceWall::~DanceWall()
{
	m_camera->release();
	delete m_camera;
	destroyAllWindows();	// this finds all the opencv windows and closes them.
}

bool DanceWall::init()
{
	cout << "Hello Dance Wall World!" << endl;

	if(!this->m_camera->isOpened()) // if not successful, exit program
	{ 
		cout << "Cannot open the video camera." << endl;
		return false;

	}

	double dWidth = this->m_camera->get(CV_CAP_PROP_FRAME_WIDTH); // get the width of the video frame
	double dHeight = this->m_camera->get(CV_CAP_PROP_FRAME_HEIGHT); // get the width of the video frame

	cout << "Initial Frame size: " << dWidth << " x " << dHeight << endl;

	// TODO: add feature to set best resolution
	// set to 720p on my laptop.
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	// set-up the window.
	namedWindow("Dance Wall", CV_WINDOW_NORMAL); // create a window called MyVideo

	return true;
}

bool DanceWall::updateFrame()
{
	bool bSuccess = this->m_camera->read(this->m_inFrame); // read a new frame from video.

	if(!bSuccess) //if no success, break loop
	{
		cout << "Cannot read a frame from video stream" << endl;
		return false;
	}

	m_effectMap[m_curEffect]->addNewFrame(&this->m_inFrame);
	m_effectMap[m_curEffect]->drawEffect();
	m_effectMap[m_curEffect]->getOutFrame(&this->m_outFrame);

	// broadcast.
	imshow("Dance Wall", this->m_outFrame); //show the frame 

	return true;
}

bool DanceWall::onKeyPress( int keyPress)
{
	int effect = m_curEffect;
	bool bSuccess = true;
	bool bUnknown = false;
	bool bParm = false;
	switch((char)keyPress) 
	{
	case 'c': 
		effect = DW::colorImage;
		break;
	case 't': 
		effect = DW::thresholdEffect;
		break;
	case 'e': 
		effect = DW::edgeEffect;
		break;
	case 'r': 
		effect = DW::cartoonEffect;
		break;
	case 'l': 
		effect = DW::lineEffect;
		break;
	case 'm': 
		effect = DW::colorMapEffect;
		cout << "Detected m key press..." << endl;
		break;
	case 'w': 
		effect = DW::warpEffect;
		break;
	case 'o':
		effect = DW::motionFlowEffect;
		break;
	case 'a':
		effect = DW::trackingEffect;
		break;
	case 'p':  // toggle through different colormaps.
		m_effectMap[m_curEffect]->toggleColorMaps();
		bParm = true;
		cout << "Detected p key press..." << endl;
		break;
	case 27: // 'esc' key
		cout << "esc key pressed, exiting..." << endl;
		bSuccess = false;
		break;
	case 'u': // print usage.
		bParm = true;
		cout << "TODO: Print the usage for the short cut keys." << endl;
		break;
	default:
		cout << "Unrecognized key pressed: " << keyPress << endl;
		bUnknown = true;
		break;
	}

	if( bSuccess && !bUnknown && !bParm)
	{
		if(m_curEffect == effect)
		{
			m_effectMap[m_curEffect]->togglePresets();
		}
		else
		{
			m_curEffect = effect;
			m_effectMap[m_curEffect]->init();
		}
	}

	return bSuccess;
}