#ifndef DW_CartoonEffect
#define DW_CartoonEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class CartoonEffect : public AbstractEffect {
public:
	CartoonEffect();
	void init();
	void drawEffect();
	void togglePresets();

private:
	int m_numDown;			// number of down-samples on the image for pyrdown
	int m_numBilateral;		// number bilateral filter passes.

	int m_blurKernelSize;   // kernel size for the median blur.

};
#endif