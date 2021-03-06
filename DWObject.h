#ifndef DW_DWObject
#define DW_DWObject

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//this is a top level class with some simple utilities.
class DWObject {
public:
	DWObject();
	void turnOnDebug();
	void turnOffDebug();
	void toggleDebug();

protected:
	// functions
	virtual string const className() = 0;
	void printError( const string& msg );
	void showDebugImage( const string& name, Mat image);
	
	// helpful functions for incrementing parameters.
	int getRandInt( int maxVal, int minVal=0 );
	void nextOneWayParm( int* curValue, int maxVal, int minVal=0, int increment=1);
	void nextTwoWayParm( double* curValue, double* curRate, double minVal, double maxVal);
	
	// variables
	bool m_debug;
};

#endif