#include "DWObject.h"

DWObject::DWObject()
{
	this->m_debug =  false;
}

void DWObject::turnOnDebug()
{
	this->m_debug = true;
}

void DWObject::turnOffDebug()
{
	this->m_debug = false;
}

void DWObject::toggleDebug()
{
	this->m_debug = this->m_debug ? false : true;
}

void DWObject::printError(const string& msg)
{
	cout << "Error in " << this->className() << ": " << msg << endl; 
}

void DWObject::showDebugImage(const string& name, Mat image)
{
	if( this->m_debug)
	{
		string winname;
		winname = this->className() + "::" + name;
		imshow( winname, image);
	}
}
// This function is used to step up a value until a maximum and
// then reset to the minimum value (default=0).
void DWObject::nextOneWayParm(int* curValue, int maxVal, int minVal, int increment)
{
	// sanity check.
	if( maxVal < minVal)
	{
		this->printError("Max/Min parameters in nextOneWayParm are invalid.");
		return;
	}

	// increment and check for ceiling.
	*curValue += increment;
	if( *curValue >= maxVal)
		*curValue = minVal;
	return;
}

// This function is used to cycle a parameter through equal steps
// up and down.
void DWObject::nextTwoWayParm(double* curValue, double* curRate, double minVal, double maxVal)
{
	// sanity check.
	if( maxVal < minVal)
	{
		this->printError("Max/Min parameters in nextTwoWayParm are invalid.");
		return;
	}

	// change the parm value by the linear rate.
	*curValue += *curRate;

	// check for a change in the rate direction.
	if( (*curValue < minVal) || (*curValue > maxVal) )
		*curRate *= -1;
}