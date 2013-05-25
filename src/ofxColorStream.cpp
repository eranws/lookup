#include "ofxColorStream.h"
#include "OpenNI.h"


void ofxColorStream ::setup(ofPtr<openni::Device> device, bool isVerbose)
{
	this->device = device;

	openni::Status rc;

	if (device->getSensorInfo(openni::SENSOR_COLOR) != NULL)
	{
		stream = ofPtr<openni::VideoStream>(new openni::VideoStream);
		rc = stream->create(*device, openni::SENSOR_COLOR);
		if (rc != openni::STATUS_OK)
		{
			throw ("Couldn't create color stream\n%s\n", openni::OpenNI::getExtendedError());
		}
	}

	allocateBuffers();

	rc = stream->start();
	if (rc != openni::STATUS_OK)
	{
		throw ("Couldn't start the color stream\n%s\n", openni::OpenNI::getExtendedError());

	}

	startThread(false, isVerbose);
}

void ofxColorStream ::exit()
{
	stopThread();
	waitForThread();
}

void ofxColorStream ::threadedFunction()
{
	openni::Status rc;

	openni::VideoFrameRef frame;
	while (isThreadRunning())
	{
		rc = stream->readFrame(&frame);
		if (rc != openni::STATUS_OK)
		{
			printf("Wait failed\n");
			continue;
		}

		openni::PixelFormat pixelFormat = frame.getVideoMode().getPixelFormat();

		if (pixelFormat != openni::PIXEL_FORMAT_GRAY8)
		{
			printf("Unexpected frame format\n");
			continue;
		}

		const unsigned char* pcolor = (const unsigned char*)frame.getData();
		int middleIndex = (frame.getHeight()+1)*frame.getWidth()/2;

		//printf("[%08llu] %8d fps:%d\n", (long long)frame.getTimestamp(), pcolor[middleIndex].r, stream->getVideoMode().getFps());

		pixels[1]->setFromPixels((const unsigned char*)frame.getData(), pixels[1]->getWidth(), pixels[1]->getHeight(), OF_IMAGE_GRAYSCALE);
		swap(pixels[0], pixels[1]);

	}

	stream->stop();
	stream->destroy();

}

void ofxColorStream::allocateBuffers()
{
	int w = 640;//stream->getVideoMode().getResolutionX();
	int h = 1024;//stream->getVideoMode().getResolutionY();

	for (int i = 0; i < 2; i++)
	{
		pixels[i] = ofPtr<ofPixels>(new ofPixels);
		pixels[i]->allocate(w, h, OF_IMAGE_GRAYSCALE);
	}
//	texture.allocate(w, h, GL_RGB);
}



