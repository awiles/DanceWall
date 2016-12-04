#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// function headers.
void getThresholdEffect(Mat *inFrame, Mat *outFrame);
void getEdgeEffect(Mat *inFrame, Mat *outFrame);
void getCartoonEffect(Mat *inFrame, Mat *outFrame);
void getLineEffect(Mat *inFrame, Mat *outFrame);
 
int main(int argc, char *argv[]){
	// some useful variables
	int keyPress =-1;
	bool bExit = false;
	Mat inFrame,	// frame read from webcam
		bwFrame,	// black & white frame
		outFrame;	// frame written out to screen/projector
	enum effect { colorImage =0, bwImage, thresholdEffect, edgeEffect, cartoonEffect, lineEffect};
	int curEffect = colorImage;

	cout << "Hello Dance Wall World!" << endl;
	//VideoCapture cap("C:\\LocalData\\src\\Personal\\FlickAnimation.avi");
	VideoCapture cap(0);
	
	if(!cap.isOpened()) // if not successful, exit program
	{ 
		cout << "Cannot open the video camera." << endl;
		return -1;

	}
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); // get the width of the video frame
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); // get the width of the video frame

	cout << "Initial Frame size: " << dWidth << " x " << dHeight << endl;

	// set to 720p on my laptop.
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	namedWindow("MyVideo", CV_WINDOW_NORMAL); // create a window called MyVideo

	while(1)
	{
		bool bSuccess = cap.read(inFrame); // read a new frame from video.

		if(!bSuccess) //if no success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

	
		switch(curEffect) {
		case bwImage:
			cvtColor(inFrame, outFrame, CV_BGR2GRAY);
			break;
		case thresholdEffect:  
			getThresholdEffect(&inFrame, &outFrame);
			break;
		case edgeEffect:
			getEdgeEffect(&inFrame, &outFrame);
			break;
		case cartoonEffect:
			getCartoonEffect(&inFrame, &outFrame);
			break;
		case lineEffect:
			getLineEffect(&inFrame, &outFrame);
			break;
		case colorImage:
		default:
			inFrame.copyTo(outFrame);
			break;

		}
		
		

		imshow("MyVideo", outFrame); //show the frame 

		// check for key presses.
		keyPress = waitKey(30);
		switch(keyPress) 
		{
		case 99: // 'c'
			curEffect = colorImage;
			break;
		case 98: // 'b'
			curEffect = bwImage;
			break;
		case 116: // 't'
			curEffect = thresholdEffect;
			break;
		case 101: // 'e'
			curEffect = edgeEffect;
			break;
		case 114: // 'r'
			curEffect = cartoonEffect;
			break;
		case 108: // 'l'
			curEffect = lineEffect;
			break;
		case 27: // 'esc' key
			cout << "esc key pressed, exiting..." << endl;
			bExit = true;
			break;
		default:
			break;
		}
		if(bExit) break;
	}
   return 0;
}

void getThresholdEffect(Mat *inFrame, Mat *outFrame)
{
	// convert to B&W
	Mat bwFrame;
	cvtColor(*inFrame, bwFrame, CV_BGR2GRAY);

	// do threshold
	threshold( bwFrame, *outFrame, 100, 255, 3);

	return;
}

void getEdgeEffect(Mat *inFrame, Mat *outFrame)
{
	// convert to B&W
	Mat bwFrame;
	cvtColor(*inFrame, bwFrame, CV_BGR2GRAY);

	// blur and call Canny.
	blur(bwFrame, *outFrame, Size(3,3));
	Canny(*outFrame, *outFrame, 10, 10*3, 3); 

	return;
}

// Cartoon Effect: http://www.askaswiss.com/2016/01/how-to-create-cartoon-effect-opencv-python.html
void getCartoonEffect(Mat *inFrame, Mat *outFrame)
{
	// vars for the effect.
	int num_down =2;
	int num_bilateral = 7;
	Mat colorFrame, bwFrame, blurFrame, edgeFrame, tempImg;

	// down sample using Gaussian pyramid.
	inFrame->copyTo(colorFrame);
	for( int i=0; i<num_down; i++)
	{
		pyrDown(colorFrame, colorFrame);
	}

	// repeatedly apply small bilateral filer instead of applying one large filter.
	for( int i=0; i<num_bilateral; i++)
	{
		bilateralFilter(colorFrame, tempImg, 9, 9, 7);
		tempImg.copyTo(colorFrame);
	}
	

	// upsample back to original size
	for( int i=0; i<num_down; i++)
	{
		pyrUp(colorFrame, colorFrame);
	}

	// covert to B&W and blur
	cvtColor(*inFrame, bwFrame, CV_BGR2GRAY);
	medianBlur(bwFrame, blurFrame, 7);

	// detect edges.
	adaptiveThreshold(blurFrame, edgeFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 2);

	// convert back to color.
	cvtColor(edgeFrame, edgeFrame, COLOR_GRAY2RGB);

	// combine two images.
	bitwise_and(colorFrame, edgeFrame, *outFrame);
	
	return;
}

// Line Effect: http://www.magicandlove.com/blog/2015/07/18/opencv-and-processing-18/ 
//		but the C++ implementation was figured out here: http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html
void getLineEffect(Mat *inFrame, Mat *outFrame)
{
	Mat bwFrame, blurFrame, edgeFrame;
	vector<Vec2f> lines;

	// clear outFrame.
	outFrame->setTo(Scalar(0,0,0));

	// covert to B&W and blur
	cvtColor(*inFrame, bwFrame, CV_BGR2GRAY);
	// blur
	medianBlur(bwFrame, blurFrame, 3);
	// do edge detection
	Canny(blurFrame, edgeFrame, 50, 200, 3);
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
			line( *outFrame, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		}
	}

	return;
}