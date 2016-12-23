#include <iostream>
#include <opencv2/opencv.hpp>
#include "DanceWall.h"

int main(int argc, char *argv[]){
	// some useful variables
	int keyPress =-1;
	bool bSuccess = true;
	Mat inFrame,	// frame read from webcam
		bwFrame,	// black & white frame
		outFrame;	// frame written out to screen/projector
	DanceWall danceWall;

	// main program loop.
	if(!danceWall.init())
	{
		cout << "Failed to init camera. Exiting." << endl;
		return -1;
	}

	while(bSuccess)
	{
		// update the current window frame.
		danceWall.updateFrame();
		
		// check for key presses.
		keyPress = waitKey(30);
		if(keyPress >= 0 )
		{
			bSuccess = danceWall.onKeyPress(keyPress);
		}
	}
   return 0;
}