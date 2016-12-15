#ifndef DW_TrackingEffect
#define DW_TrackingEffect

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class TrackingEffect : public AbstractEffect {
public:
	TrackingEffect();
	void init();
	void drawEffect();
	void togglePresets();

protected:
	// functions.
	void drawHappyFaces();

	// variables.
	CascadeClassifier m_faceCascade;
	std::vector<Rect> m_faces;
	CascadeClassifier m_eyesCascade;
	std::vector<Rect> m_eyes;

};
#endif