#include "ofxDepthStream.h"
#include "OpenNI.h"
#include "ofxProfile.h"

string toString( const openni::VideoMode& v )
{
	openni::PixelFormat p = v.getPixelFormat();
	string format = (p == openni::PIXEL_FORMAT_DEPTH_1_MM) ? "1mm" : (p == openni::PIXEL_FORMAT_DEPTH_100_UM) ? "0.1mm" : "other";
	stringstream ss;
	ss << v.getResolutionX() << "x" << v.getResolutionY() << " @" << v.getFps() << " " << format;
	return ss.str();
}

void ofxDepthStream::setup(ofPtr<openni::Device> device, bool isVerbose)
{
	this->device = device;

	openni::Status rc;

	depthSensorInfo = device->getSensorInfo(openni::SENSOR_DEPTH);
	if (depthSensorInfo != NULL)
	{
		stream = ofPtr<openni::VideoStream>(new openni::VideoStream);
		rc = stream->create(*device, openni::SENSOR_DEPTH);
		if (rc != openni::STATUS_OK)
		{
			throw ("Couldn't create depth stream\n%s\n", openni::OpenNI::getExtendedError());
		}
	}

	//populate videoModes stringArray
	const openni::Array<openni::VideoMode>& v = depthSensorInfo->getSupportedVideoModes();
	for (int i = 0; i < v.getSize(); i++)
	{
		videoModesString.push_back(toString(v[i]));
	}

	//setResolution640x480(); //HACKHACK

	allocateBuffers();

	rc = stream->start();
	if (rc != openni::STATUS_OK)
	{
		throw ("Couldn't start the depth stream\n%s\n", openni::OpenNI::getExtendedError());

	}

	startThread(false, isVerbose);
}

void ofxDepthStream::exit()
{
	stopThread();
	waitForThread();
}

void ofxDepthStream::threadedFunction()
{
	openni::Status rc;

	while (isThreadRunning())
	{
		ofxProfileSectionPush("depthStream update");
		
		openni::VideoFrameRef frame;
		rc = stream->readFrame(&frame);
		if (rc != openni::STATUS_OK)
		{
			printf("Wait failed\n");
			continue;
		}

		if (frame.getVideoMode().getPixelFormat() != openni::PIXEL_FORMAT_DEPTH_1_MM && frame.getVideoMode().getPixelFormat() != openni::PIXEL_FORMAT_DEPTH_100_UM)
		{
			printf("Unexpected frame format\n");
			continue;
		}

		openni::DepthPixel* pDepth = (openni::DepthPixel*)frame.getData();
		int middleIndex = (frame.getHeight()+1)*frame.getWidth()/2;
		
		//printf("[%08llu] %8d fps:%d\n", (long long)frame.getTimestamp(), pDepth[middleIndex], stream->getVideoMode().getFps());

		pixels[1]->setFromPixels((const unsigned short*)frame.getData(), pixels[1]->getWidth(), pixels[1]->getHeight(), OF_IMAGE_GRAYSCALE);
		swap(pixels[0], pixels[1]);

		ofxProfileSectionPop();
	}

	stream->stop();
	stream->destroy();

}

ofVec3f ofxDepthStream::cameraToWorld(ofVec2f p)
{
	ofVec3f world;

	ofPtr<ofShortPixels> pix = getPixels();

	int index = pix->getPixelIndex(int(p.x), int(p.y));
	unsigned short z = (*pix)[index];

	openni::CoordinateConverter::convertDepthToWorld(*stream, p.x, p.y, float(z), &world.x, &world.y, &world.z);
	return world;
}



void ofxDepthStream::allocateBuffers()
{
	int w = stream->getVideoMode().getResolutionX();
	int h = stream->getVideoMode().getResolutionY();

	for (int i = 0; i < 2; i++)
	{
		pixels[i] = ofPtr<ofShortPixels>(new ofShortPixels);
		pixels[i]->allocate(w, h, OF_IMAGE_GRAYSCALE);
	}
	
}

void ofxDepthStream::setResolution640x480()
{
	const openni::Array<openni::VideoMode>& v = depthSensorInfo->getSupportedVideoModes();
	for (int i = 0; i < v.getSize(); i++)
	{
		openni::PixelFormat p = v[i].getPixelFormat();
		if (p == openni::PIXEL_FORMAT_DEPTH_1_MM && v[i].getResolutionX() == 640 && v[i].getResolutionY()==480)// << v.getFps() << " " << format;
			stream->setVideoMode(v[i]);
	}
}
