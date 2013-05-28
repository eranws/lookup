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

	setupNite();

	startThread(false, isVerbose);
}

void ofxDepthStream::exit()
{
	stopThread();
	waitForThread();
	nite::NiTE::shutdown();
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
		updateNite();

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



void ofxDepthStream::setupNite()
{
	nite::NiTE::initialize();
	nite::Status niteRc;

	niteRc = userTracker.create();
	if (niteRc != nite::STATUS_OK)
	{
		printf("Couldn't create user tracker\n");
		return;
	}

	for (int i = 0; i < 2; i++)
	{
//		MAP MKP MAPAMPAMAPAPAP  userTrackerFrame[i] = nite::UserTrackerFrameRef();
	}
	
	printf("\nuserTracker.create - OK\n");

}



void ofxDepthStream::updateNite()
{
	nite::UserTrackerFrameRef  userTrackerFrame;
	nite::Status niteRc = userTracker.readFrame(&userTrackerFrame);
	if (niteRc != nite::STATUS_OK)
	{
		printf("Get next frame failed\n");
		//			return;
	}

	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
	for (int i = 0; i < users.getSize(); ++i)
	{

		const nite::UserData& user = users[i];
		nite::UserId id = user.getId();

		//			updateUserState(user,userTrackerFrame.getTimestamp());
		if (user.isNew())
		{
			userTracker.startSkeletonTracking(id);
			userMap[id];

		}
		else if (user.isVisible() && user.getSkeleton().getState() == nite::SKELETON_TRACKED)
		{
			userMap[id].update(user);
			if (userMap[id].status.valid)
			{
				printf("FFF\n");
//send event/SiGNAL!				nodeSwarm.addParticle(userMap[id].status.position, userMap[id].status.velocity);
			}
		}


		else if (user.isLost())
		{
			userMap.erase(id);
		}

	}

}

