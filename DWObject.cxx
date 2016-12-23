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