#ifndef DW_EdgeEffect
#define DW_EdgeEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class EdgeEffect : public AbstractEffect {
public:
	EdgeEffect();
	virtual string const className() { return "EdgeEffect";}
	void init();
	void drawEffect();
	void togglePresets();

private:
	int m_blurKernelSize;		// kernal size for the
	int m_cannyThreshold1;		// Canny edge detector threshold1
	int m_cannyThreshold2;		// Canny edge detector threshold2
	int m_cannyApertureSize;	// Canny edge detector apeture size

};
#endif