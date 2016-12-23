#ifndef DW_LineEffect
#define DW_LineEffect

#define DW_LE_HOUGHLINE		0
#define DW_LE_HOUGHLINEP	1

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class LineEffect : public AbstractEffect {
public:
	LineEffect();
	virtual string const className() { return "LineEffect";}
	void init();
	void drawEffect();
	void togglePresets();

private:
	int m_effectOption;

};
#endif