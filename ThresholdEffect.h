#ifndef DW_ThresholdEffect
#define DW_ThresholdEffect

#define DW_MAX_THRESHOLD 255

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class ThresholdEffect : public AbstractEffect {
public:
	ThresholdEffect();
	virtual string const className() { return "ThresholdEffect";}
	void init();
	void drawEffect();
	void togglePresets();
	virtual void getRandomConfig(bool doGrid=false);

protected:
	int m_thresholdLevel;		// value where the threshold level occurs.
	int m_thresholdType;		// type of threshold

};
#endif