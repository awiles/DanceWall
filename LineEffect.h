#ifndef DW_LineEffect
#define DW_LineEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class LineEffect : public AbstractEffect {
public:
	LineEffect();
	void init();
	void drawEffect();
	void togglePresets();

private:
	

};
#endif