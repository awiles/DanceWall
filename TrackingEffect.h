#ifndef DW_TrackingEffect
#define DW_TrackingEffect

#define DW_TE_MAXSUBEFFECTS 5

#include <iostream>
#include <opencv2/opencv.hpp>
#include "AbstractEffect.h"
#include "HandTracker.h"
#include "SimpleMotionTracker.h"
#include "MotionEffect.h"
#include <time.h>

using namespace cv;
using namespace std;

//create an abstract class for all the effects I am going to create.
class TrackingEffect : public AbstractEffect {
public:
	TrackingEffect();
	virtual string const className() { return "TrackingEffect";}
	void init();
	void drawEffect();
	void togglePresets();
	virtual void getRandomConfig(bool doGrid=false);

protected:
	// functions.
	void drawHappyFaces();
	void drawHandSpots();
	void drawBGSubtraction();
	void drawHandTracking();
	void drawMotion();

	// variables.
	int m_subEffect;		// which sub-effect to create.
	CascadeClassifier m_faceCascade;
	std::vector<Rect> m_faces;
	CascadeClassifier m_eyesCascade;
	std::vector<Rect> m_eyes;
	
	CascadeClassifier m_palmCascade;
	std::vector<Rect> m_palms;
	CascadeClassifier m_closedPalmCascade;
	std::vector<Rect> m_closedPalms;
	CascadeClassifier m_fistCascade;
	std::vector<Rect> m_fists;

	// hand tracking
	HandTracker m_handTracker;

	// motion detection.
	SimpleMotionTracker m_motionTracker;
	MotionEffect m_motionEffect;


};
#endif