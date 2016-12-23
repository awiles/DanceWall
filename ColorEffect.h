#ifndef DW_ColorEffect
#define DW_ColorEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

#define DW_BLUE_CHANNEL 0
#define DW_GREEN_CHANNEL 1
#define DW_RED_CHANNEL 2
#define DW_COLOR_FULL 3
#define DW_GRAYSCALE 4

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class ColorEffect : public AbstractEffect {
public:
	ColorEffect();
	virtual string const className() { return "ColorEffect";}
	void init();
	void drawEffect();

	// specific to this class.
	inline void setColorType(int colorType) {this->m_colorType = colorType;}
	inline int getColorType() {return this->m_colorType;}
	void togglePresets();

private:
	int m_colorType;

};
#endif