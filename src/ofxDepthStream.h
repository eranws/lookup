#pragma once
#include "ofThread.h"
#include "ofPixels.h"
#include "NiTE.h"

namespace openni
{
	class Device;
	class VideoStream;
	class SensorInfo;
}

class ofxDepthStream : public ofThread
{

public:

	void setup(ofPtr<openni::Device> device = ofPtr<openni::Device>(), bool isVerbose = false);

	void exit();

	ofPtr<openni::Device> getDevice() const { return device; }
	ofPtr<openni::VideoStream> getStream() const { return stream; }

	ofPtr<ofShortPixels> getPixels() const { return pixels[0]; }
//	nite::UserTrackerFrameRef getUserTrackerFrame() const { return userTrackerFrame[0]; }

	const vector<string>& getVideoModesString() const { return videoModesString; }
	void setResolution640x480();

	ofVec3f cameraToWorld(ofVec2f p); //Depth Camera?
	ofVec2f worldToCamera(ofVec3f p);
	nite::UserTracker userTracker;

protected:
	ofPtr<ofShortPixels> pixels[2];

	virtual void threadedFunction();
	void allocateBuffers();
	ofPtr<openni::Device> device;
	ofPtr<openni::VideoStream> stream;

	const openni::SensorInfo* depthSensorInfo;
	vector<string> videoModesString;
	
	void setupNite();
	void updateNite();

};
