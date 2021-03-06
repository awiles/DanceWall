#ifndef DW_ColorMapEffect
#define DW_ColorMapEffect



#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class ColorMapEffect : public AbstractEffect {
public:
	ColorMapEffect();
	virtual string const className() { return "ColorMapEffect";}
	void init();
	void drawEffect();
	void togglePresets();
	virtual void getRandomConfig(bool doGrid=false);

private:
	
};
#endif