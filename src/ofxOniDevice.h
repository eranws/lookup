#pragma once
#include "ofTypes.h"

namespace openni
{
	class Device;
}

class ofxOniDevice
{

public:

	void setup(const char* uri = NULL);
	void exit();

	ofPtr<openni::Device> getDevice() const { return device; }
	
	void setRegistration( bool b ); //TODO: access via settings object
	void setFrameSync( bool b );
	
protected:
	ofPtr<openni::Device> device;

};
