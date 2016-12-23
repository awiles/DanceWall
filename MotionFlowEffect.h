#ifndef DW_MotionFlowEffect
#define DW_MotionFlowEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class MotionFlowEffect : public AbstractEffect {
public:
	MotionFlowEffect();
	virtual string const className() { return "MotionFlowEffect";}
	void init();
	void drawEffect();
	void togglePresets();

private:
	Mat m_previousFrame;
	Mat m_nextFrame;

};
#endif