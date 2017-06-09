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
	this->m_cameraID = 0;
	this->m_camera = new VideoCapture(this->m_cameraID);

	// random effects set-up.
	this->m_bDoRandom = false;
	this->m_nRandomChanges = 0;
	this->m_startTime = clock();
	srand(time(NULL));
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
	//m_camera->set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//m_camera->set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	// set-up the window.
	namedWindow("Dance Wall", CV_WINDOW_NORMAL); // create a window called MyVideo
	cvSetWindowProperty("Dance Wall", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	return true;
}

bool DanceWall::changeCamera()
{
	this->m_cameraID++;
	if(this->m_cameraID >= DW_MAX_CAMERAS)
		this->m_cameraID = 0;

	cvDestroyAllWindows();
	this->m_camera->open(this->m_cameraID);
	return this->init();
}

bool DanceWall::updateFrame()
{
	bool bSuccess = this->m_camera->read(this->m_inFrame); // read a new frame from video.
	// flip the image from the web cam.
	flip(this->m_inFrame, this->m_inFrame, 1);

	if(!bSuccess) //if no success, break loop
	{
		cout << "Cannot read a frame from video stream" << endl;
		return false;
	}

	// are we doing random mode?
	if(this->m_bDoRandom)
	{
		double duration = ( clock() - m_startTime ) / (double) CLOCKS_PER_SEC;
		cout << "Time since last change: " << duration << " seconds." << endl;
		if (duration > DW_EFFECT_TIME_LIMIT )
			doNextRandom();
	}

	m_effectMap[m_curEffect]->addNewFrame(this->m_inFrame);
	m_effectMap[m_curEffect]->drawEffect();
	this->m_outFrame = m_effectMap[m_curEffect]->getOutFrame();

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
	case '1': 
		effect = DW::colorImage;
		break;
	case '2': 
		effect = DW::thresholdEffect;
		break;
	case '3': 
		effect = DW::edgeEffect;
		break;
	case '4': 
		effect = DW::cartoonEffect;
		break;
	case '5': 
		effect = DW::lineEffect;
		break;
	case '6': 
		effect = DW::colorMapEffect;
		break;
	case '7': 
		effect = DW::warpEffect;
		break;
	case '8':
		effect = DW::motionFlowEffect;
		break;
	case '9':
		effect = DW::trackingEffect;
		break;
	case 'c':
		this->changeCamera();
		break;
	case 'm':  // toggle through different colormaps.
		m_effectMap[m_curEffect]->changeColorMap();
		bParm = true;
		break;
	case 'o': // turn colormap effect on and off.
		m_effectMap[m_curEffect]->toggleColorMapsOn();
		bParm = true;
		break;
	case 'g': // increment grid order.
		m_effectMap[m_curEffect]->incrementGridOrder();
		bParm = true;
		break;
	case 'r': // toggle the random mode on.
		this->m_bDoRandom = this->m_bDoRandom?false:true;
		bParm = true;
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

void DanceWall::doNextRandom()
{
	
	// determine the next effect.
	this->m_curEffect = this->getRandInt(m_effectMap.size()-1);

	// do we want the grid effect this time?
	this->m_nRandomChanges++;
	bool doGrid = (m_nRandomChanges % DW_GRID_EFFECT_INTERVAL)?true:false;

	m_effectMap[m_curEffect]->init();
	m_effectMap[m_curEffect]->getRandomConfig(doGrid);


	// reset start time.
	this->m_startTime = clock();
}