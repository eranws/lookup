#include "testApp.h"

#include "ofxCv.h"
#include "Utilities.h"
#include "..\BirdEvents.h"

//#define PROFILE
#include "ofxProfile.h"
#include "..\Sound3d.h"

//--------------------------------------------------------------
void testApp::setup(){

	toDrawVideo = false;
	toDrawScene = false;
	toDrawSideViewports = false;
	toggleMirror = false;

	showHelpText = false;

	ofSetVerticalSync(true);
	ofSetWindowTitle("LookUp!");
	ofSetFrameRate(60);
	ofSetFullscreen(true);
	ofHideCursor();

	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	try
	{
		oniDevice.setup();
	}
	catch (exception e)
	{
		oniDevice.setup("c:\\b1.oni");
	}


	depthStream.setup(oniDevice.getDevice());
	colorStream.setup(oniDevice.getDevice());
	oniDevice.setRegistration(true);
	oniDevice.setFrameSync(true);

	recorder.setup();
	recorder.addStream(depthStream.getStream());
	recorder.addStream(colorStream.getStream());
	updateMats();
	allocateTextures();

	setupGui();
	gui1->loadSettings("settings.xml");

	setupScene();

	gui1->setVisible(false);

	
	registerBirdEvents(this);

}

void testApp::drawVideo2D(){

	// Crop relevant rectangle for display
	ofxUIRangeSlider* hCrop = (ofxUIRangeSlider*)gui1->getWidget("GRAY_CROP_H");		
	ofxUIRangeSlider* wCrop = (ofxUIRangeSlider*)gui1->getWidget("GRAY_CROP_W");		

	float sx = wCrop->getScaledValueLow();
	float sw = wCrop->getScaledValueHigh() - sx;

	float sy = hCrop->getScaledValueLow();
	float sh = hCrop->getScaledValueHigh() - sy;

	shadowTex.drawSubsection(ofGetWindowWidth() / 2 - 320, ofGetWindowHeight() - 1024, 640, 1024, sx, sy, sw, sh);

//	Trees::draw();

	drawVideo();
}

void testApp::drawShadow()
{
	ofPushMatrix();
	ofTranslate(-shadowTex.getWidth() / 2, -shadowTex.getHeight() / 2, 0);
	shadowTex.draw(0,0, 1);
	ofPopMatrix();
}

void testApp::drawVideo(){
	if (!toDrawVideo)
		return;
	glDisable(GL_DEPTH_TEST);

	ofPushStyle();
	ofPushMatrix();


	float f = 0.5;
	int dw = depthTex.getWidth() * f, dh = depthTex.getHeight() * f;
	depthTex.draw(0, 0, -0.1, dw, dh);

	ofTranslate(dw, 0, 0);
	ofScale(f, f);

	int cw = colorTex.getWidth();
	int ch = colorTex.getHeight();
	colorTex.draw(0, 0, 0.1, cw, ch);

	ofxUIRangeSlider* hCrop = (ofxUIRangeSlider*)gui1->getWidget("GRAY_CROP_H");		
	ofxUIRangeSlider* wCrop = (ofxUIRangeSlider*)gui1->getWidget("GRAY_CROP_W");		

	float sx = wCrop->getScaledValueLow();
	float sw = wCrop->getScaledValueHigh() - sx;

	float sy = hCrop->getScaledValueLow();
	float sh = hCrop->getScaledValueHigh() - sy;

	ofSetColor(ofColor::red);
	ofSetLineWidth(3);
	ofNoFill();
	ofRect(sx, sy, sw, sh);


	ofPopMatrix();
	ofPopStyle();

	glEnable(GL_DEPTH_TEST);
}

void testApp::setupGui()
{

	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 255-xInit; 

	gui1 = ofPtr<ofxUICanvas>(new ofxUICanvas(0, 0, length+xInit, ofGetHeight())); 

	gui1->addWidgetDown(new ofxUILabel("Look Up", OFX_UI_FONT_LARGE)); 
	gui1->addWidgetDown(new ofxUILabel("Press 'g' to Hide GUIs", OFX_UI_FONT_LARGE)); 

	gui1->addButton("Save Settings", false, dim, dim);

	gui1->addSpacer(length-xInit, 2);

	gui1->addWidgetDown(new ofxUILabel("Depth Cutoff", OFX_UI_FONT_MEDIUM)); 
	gui1->addRangeSlider("RSLIDER", 0.0, 10000.0, 450.0, 2000.0, length-xInit,dim);
	gui1->addSlider("colorThreshold", 1, 255, 50, length-xInit,dim);
	gui1->addSpacer(length-xInit, 2);
	vector<string> depthModes = depthStream.getVideoModesString();
	//gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim); 
	//gui1->addRadio("Depth Modes", depthModes, OFX_UI_ORIENTATION_VERTICAL, dim, dim); 


	gui1->addSpacer(length-xInit, 2);
	//gui1->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 
	//gui1->addButton("DRAW GRID", false, dim, dim);
	gui1->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM)); 
	gui1->addToggle( "Draw Video", &toDrawVideo, dim, dim);
	gui1->addToggle( "Draw Grid", &toDrawGrid, dim, dim);
	gui1->addToggle( "Draw Scene", &toDrawScene, dim, dim);
	gui1->addToggle( "Draw Side Viewports", &toDrawSideViewports, dim, dim);
	gui1->addToggle( "Mirror", &toggleMirror, dim, dim);

	gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM));

	gui1->addRangeSlider("GRAY_CROP_W", 0.0, 640, 0.0, 640.0, length-xInit,dim);
	gui1->addRangeSlider("GRAY_CROP_H", 0.0, 1024, 0.0, 1024.0, length-xInit,dim);

	gui1->addToggle("Toggle Exposure", &toggleExposure, dim, dim);
	gui1->addToggle("Toggle Gain", &toggleGain, dim, dim);
	gui1->addSlider("Gain", 0.0, 1570, 1570, length-xInit,dim);
	gui1->addSlider("Exposure", 1.0, 30.0, 30.0, length-xInit,dim);


	gui1->setDrawBack(true);
	gui1->setColorBack(ofColor::gray);

	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 	

	if(name == "Mirror")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget; 
		depthStream.getStream()->setMirroringEnabled(button->getValue());
		colorStream.getStream()->setMirroringEnabled(button->getValue());
		//bdrawGrid = button->getValue(); 
	}
	else if(name == "Toggle Exposure")
	{
		ofxUIButton* button = (ofxUIButton*) e.widget; 
		openni::Status rc = colorStream.getStream()->getCameraSettings()->setAutoExposureEnabled(button->getValue());
	}
	else if(name == "Toggle Gain")
	{
		ofxUIButton* button = (ofxUIButton*) e.widget; 
		openni::Status rc = colorStream.getStream()->getCameraSettings()->setAutoWhiteBalanceEnabled(button->getValue());
	}
	else if(name == "Exposure")
	{
		ofxUISlider* slider = (ofxUISlider*) e.widget; 
		openni::Status rc = colorStream.getStream()->getCameraSettings()->setExposure((int)slider->getScaledValue());
	}
	else if(name == "Gain")
	{
		ofxUISlider* slider = (ofxUISlider*) e.widget; 
		openni::Status rc = colorStream.getStream()->getCameraSettings()->setGain(slider->getScaledValue());
	}

	else if(name == "D_GRID")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		//bdrawGrid = toggle->getValue(); 
	}
	else if(name == "TEXT INPUT")
	{
		ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
		if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
		{
			cout << "ON ENTER: "; 
			//            ofUnregisterKeyEvents((ofxUItestApp*)this); 
		}
		else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
		{
			cout << "ON FOCUS: "; 
		}
		else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
		{
			cout << "ON BLUR: "; 
			//            ofRegisterKeyEvents(this);             
		}        
		string output = textinput->getTextString(); 
		cout << output << endl; 
	}

	else if(name == "Save Settings")
	{
		gui1->saveSettings("settings.xml");
		gui1->saveSettings("settings_"+ ofGetTimestampString() +".xml");

	}


}

void testApp::setupScene()
{
	iMainCamera = 0;
	bCamParent = false;

	// user camera
	camEasyCam.setTarget(swarm);
	camEasyCam.setDistance(1000);
	cameras[0] = &camEasyCam;

	// front
	camFront.scale = 240; // camera height / 2
	camFront.tilt(-180);
	cameras[1] = &camFront;

	// top
	camTop.scale = 20;
	camTop.tilt(-90);
	cameras[2] = &camTop;

	// left
	camLeft.scale = 20;
	camLeft.pan(-90);
	cameras[3] = &camLeft;

	setupViewports(toDrawSideViewports);

	Trees::init("/trees/");

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	swarm.init();
}

void testApp::setupViewports(bool sideViews){
	if (sideViews)
	{
		float xOffset = ofGetWidth() / 3;
		float yOffset = ofGetHeight() / N_CAMERAS;

		viewMain.x = xOffset;
		viewMain.y = 0;
		viewMain.width = xOffset * 2;
		viewMain.height = ofGetHeight();

		for(int i = 0; i < N_CAMERAS; i++){

			viewGrid[i].x = 0;
			viewGrid[i].y = yOffset * i;
			viewGrid[i].width = xOffset;
			viewGrid[i].height = yOffset;
		}
	}
	else
	{
		viewMain.x = 0;
		viewMain.y = 0;
		viewMain.width = ofGetWidth();
		viewMain.height = ofGetHeight();
	}
}



//--------------------------------------------------------------
void testApp::draw(){

	if (toDrawSideViewports)
	{
		glDisable(GL_DEPTH_TEST);
		ofPushStyle();
		ofSetColor(100, 100, 100);
		ofRect(viewGrid[iMainCamera]);
		ofPopStyle();
		glEnable(GL_DEPTH_TEST);
	}
	glDisable(GL_DEPTH_TEST);
	drawVideo2D();
	drawScene(iMainCamera);
	glEnable(GL_DEPTH_TEST);


	//TODO: add widgets dynamically in one line
	//#define addMat(x) cv::Mat& x = matMap[#x] = cv::Mat();
	//float dim = 16;
	//static ofxUIToggle* toDrawSideViewports = new ofxUIToggle("toDrawSideViewports", false, dim, dim);
	//if(!gui1->getWidget("toDrawSideViewports"))  gui1->addWidget(toDrawSideViewports);

	if (toDrawSideViewports)
	{
		for(int i = 0; i < N_CAMERAS; i++){
			cameras[i]->begin(viewGrid[i]);
			drawScene(i);
			cameras[i]->end();
		}
	}


	// Draw annotations (text, gui, etc)
	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything

	// draw some labels
	outputStrings.push_back(ofToString(swarm.size()));


	if(showHelpText)
	{
		for (int i = 0; i < outputStrings.size(); i++)
		{
			ofSetColor(0, 0, 0); //shadow effect
			ofDrawBitmapString(outputStrings[i], viewMain.x + 20 - 1, viewMain.y + 30 -1 + 20 * i);
			ofSetColor(255, 255, 255);
			ofDrawBitmapString(outputStrings[i], viewMain.x + 20, viewMain.y + 30 + 20 * i);
		}
	}

	if (toDrawSideViewports)
	{
		ofDrawBitmapString("EasyCam",   viewGrid[0].x + 20, viewGrid[0].y + 30);
		ofDrawBitmapString("Front",     viewGrid[1].x + 20, viewGrid[1].y + 30);
		ofDrawBitmapString("Top",       viewGrid[2].x + 20, viewGrid[2].y + 30);
		ofDrawBitmapString("Left",      viewGrid[3].x + 20, viewGrid[3].y + 30);

		// draw outlines on views
		ofSetLineWidth(5);
		ofNoFill();
		ofSetColor(255, 255, 255);

		for(int i = 0; i < N_CAMERAS; i++)
		{
			ofRect(viewGrid[i]);
		}
		ofRect(viewMain);
	}

	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();
}

void testApp::drawScene(int iCameraDraw){
	if (!toDrawScene)
		return;

	swarm.draw();
	if (toDrawGrid)
	{
		nodeGrid.draw();
	}
	//--
	// Draw frustum preview for ofEasyCam camera

	// This code draws our camera in
	//	the scene (reddy/pink lines)
	//
	// The pyramid-like shape defined
	//	by the cameras view is called
	//	a 'frustum'.
	//
	// Often we refer to the volume
	//	which can be seen by the
	//	camera as 'the view frustum'.


	// First check if we're already drawing the view through the easycam
	// If so, don't draw the frustum preview.
	if(iCameraDraw != 0){

		ofPushStyle();
		ofPushMatrix();

		//--
		// Create transform for box->frustum

		// In 'camera space' the bounds of
		//  the view are defined by a box
		//  with bounds -1->1 in each axis
		//
		// To convert from camera to world
		//  space, we multiply by the inverse
		//  camera matrix of the camera, i.e
		//  inverse of the ViewProjection
		//  matrix.
		//
		// By applying this transformation
		//  our box in camera space is
		//  transformed into a frustum in
		//  world space.
		//

		// The camera's matricies are dependant on
		//  the aspect ratio of the viewport.
		//  (Which is why we use the viewport as
		//  an argument when we begin the camera.
		//
		// If this camera is fullscreen we'll use
		//   viewMain, else we'll use viewGrid[0]
		ofRectangle boundsToUse;
		if(iMainCamera == 0){
			boundsToUse = viewMain;
		}
		else{
			boundsToUse = viewGrid[0];
		}

		// Now lets get the inverse ViewProjection
		//  for the camera
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf(camEasyCam.getModelViewProjectionMatrix(boundsToUse));

		// By default, we can say
		//	'we are drawing in world space'
		//
		// The camera matrix performs
		//	world->camera
		//
		// The inverse camera matrix performs
		//	camera->world
		//
		// Our box is in camera space, if we
		//	want to draw that into world space
		//	we have to apply the camera->world
		//	transformation.
		//

		// This syntax is a little messy.
		// What it's saying is, send the data
		//  from the inverseCameraMatrix object
		//  to OpenGL, and apply that matrix to
		//  the current OpenGL transform
		glMultMatrixf(inverseCameraMatrix.getPtr());

		//
		//--



		//--
		// Draw box in camera space
		// (i.e. frustum in world space)

		ofNoFill();
		// i.e. a box -1, -1, -1 to +1, +1, +1
		ofBox(0, 0, 0, 2.0f);
		//
		//--

		ofPopStyle();
		ofPopMatrix();
	}

	//--
	// Draw mouse ray

	// Draw the ray if ofEasyCam is in main view,
	//  and we're not currently drawing in that view
	if(iMainCamera == 0 && iCameraDraw != 0){
		ofPushStyle();
		ofSetColor(100, 100, 255);
		ofLine(ray[0], ray[1]);
		ofPopStyle();
	}

	//drawShadow();

}

//--------------------------------------------------------------
void testApp::updateMouseRay(){
	// Define ray in screen space
	ray[0] = ofVec3f(ofGetMouseX(), ofGetMouseY(), -1);
	ray[1] = ofVec3f(ofGetMouseX(), ofGetMouseY(), 1);

	// Transform ray into world space
	ray[0] = cameras[iMainCamera]->screenToWorld(ray[0], viewMain);
	ray[1] = cameras[iMainCamera]->screenToWorld(ray[1], viewMain);
}

ofPoint getOrigin() 
{
	return ofPoint(ofGetWindowWidth() / 2 - 320, ofGetWindowHeight() - 1024, 5);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch(key)
	{
	case '1':
	case '2':
	case '3':
	case '4':
		iMainCamera = key - '1';
		break;

	case 'f': ofToggleFullscreen(); setupViewports(toDrawSideViewports); break;
	case 'g': gui1->toggleVisible(); break;
	case 'd': toDrawVideo = !toDrawVideo; break; //TODO: update gui
	case 'v': toDrawSideViewports = !toDrawSideViewports; setupViewports(toDrawSideViewports); break;
	case 'h': showHelpText = !showHelpText; break;
	case 's': toDrawScene = !toDrawScene; break;


	case 'a':
		swarm.addParticle(1);
		break;
	case 'A':	
		swarm.addParticle(10);
		break;

	case 'b':
		{
			ofPoint p(320, 512);
			ofVec3f v(0, 0, 0);
			swarm.addParticle(getOrigin() + p, v);
			break;
		}

			


	default:
		break;
	}

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	setupViewports(toDrawSideViewports);
}


void testApp::updateMats()
{
	ofPtr<ofPixels> colorPixels = colorStream.getPixels();
	colorMat = ofxCv::toCv(*colorPixels);
	colorTex.loadData(colorMat.ptr(), colorMat.cols, colorMat.rows, GL_LUMINANCE);

	ofPtr<ofShortPixels> depthPixels = depthStream.getPixels();
	depthMat = ofxCv::toCv(*depthPixels);
	cv::Mat m8;	depthMat.convertTo(m8, CV_8UC1, 0.0625);

	depthTex.loadData(m8.ptr(), m8.cols, m8.rows, GL_LUMINANCE);
}

void testApp::allocateTextures()
{
	depthTex.allocate(depthMat.cols, depthMat.rows, GL_LUMINANCE);
	colorTex.allocate(colorMat.cols, colorMat.rows, GL_LUMINANCE);
	shadowTex.allocate(colorMat.cols, colorMat.rows, GL_LUMINANCE);
}

void testApp::update()
{

	outputStrings.clear();
	// help text
	outputStrings.push_back("Press keys 1-4 to select a camera for main view");
	outputStrings.push_back("Camera selected: " + ofToString(iMainCamera + 1));
	outputStrings.push_back("Press 'f' to toggle fullscreen");
	outputStrings.push_back("[d]raw video:" + ofToString(toDrawVideo ? "On" : "Off"));
	outputStrings.push_back("draw [s]cene:" + ofToString(toDrawScene ? "On" : "Off"));
	outputStrings.push_back("side [v]iewports:" + ofToString(toDrawSideViewports ? "On" : "Off"));
	outputStrings.push_back(ofToString(ofGetFrameRate()));
	outputStrings.push_back(ofxProfile::describe());



	ofxProfileSectionPush("updateMats");
	updateMats();
	ofxProfileSectionPop();
	ofxProfileSectionPush("cvProcess");
	cvProcess();
	ofxProfileSectionPop();

}

void testApp::cvProcess()
{
	using namespace cv;

	ofxProfileSectionPush("clone");
	Mat c = colorMat.clone();
	ofxProfileSectionPop();
	
#if 0
	ofxProfileSectionPush("depth ops");
	Mat m = depthMat.clone();
	//	resize(m, m, cv::Size(), 3, 3, CV_INTER_CUBIC);

	ofxUIRangeSlider* r = (ofxUIRangeSlider*)gui1->getWidget("RSLIDER");		

	Mat depthMask = (m > r->getScaledValueLow() &  m < r->getScaledValueHigh());
	Mat invertDepthMask;
	cv::bitwise_not(depthMask, invertDepthMask);

	m.setTo(0, invertDepthMask);
	cv::Mat m8;	m.convertTo(m8, CV_8UC1, 0.25);

	cv::Mat depthMaskHD;
	cv::Mat depthMaskCrop = depthMask(cv::Range::all(), cv::Range(160, 480));

	cv::resize(depthMaskCrop, depthMaskHD, cv::Size(), 2, 2);

	Mat depthMaskHdCropped(cv::Mat(1024, 640, depthMaskHD.type()));
	int vOffset = 32; //HD adds 32 rows on top and bottom
	depthMaskHD.copyTo(depthMaskHdCropped(cv::Range(0 + vOffset, 960 + vOffset), cv::Range::all()));
	cv::morphologyEx(depthMaskHdCropped, depthMaskHdCropped, CV_MOP_DILATE, cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(11, 11)));
	ofxProfileSectionPop();
#endif

	ofxUISlider* s = (ofxUISlider*)gui1->getWidget("colorThreshold");


	ofxProfileSectionPush("gauss blur");
	GaussianBlur(c, c, cv::Size(3,3), 1);
	ofxProfileSectionPop();

	int thresh = s->getScaledValue();
	//	cv::adaptiveThreshold(c, c, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 7, thresh);
	c = c > thresh;

	ofxProfileSectionPush("median blur");
	cv::medianBlur(c, c, 3);
	ofxProfileSectionPop();


	//c.copyTo(c2, depthMaskHdCropped);// fix Res

	c += 64; //25% gray
	//shadowTex.loadData(c2.ptr(), c2.cols, c2.rows, GL_LUMINANCE);

	ofxProfileSectionPush("loadData");
	shadowTex.loadData(c.ptr(), c.cols, c.rows, GL_LUMINANCE);
	ofxProfileSectionPop();

}

void testApp::createBird( BirdData& p )
{
//	printf("TESTAPP: CREATE BIRD \n");
//	cout << p.position << " " << p.velocity << endl;
	swarm.addParticle(p.position, p.velocity);
}

void testApp::exit()
{
	depthStream.exit();
	colorStream.exit();

//! TODO check:
//	Sound3D::closeFmod();

}

			/*
			const nite::Skeleton& skeleton = user.getSkeleton();
			const nite::SkeletonJoint& head = skeleton.getJoint(nite::JOINT_RIGHT_HAND);
			if (head.getPositionConfidence() > .5)
			{
				nite::Point3f nitePos = head.getPosition();
				ofPoint ofPos(nitePos.x, nitePos.y, nitePos.z);

				ofVec2f depthPt = depthStream.worldToCamera(ofPos);
				ofVec2f grayPt = toGrayHD(depthPt);

				ofPoint ofPosTL(ofPos);
				ofPosTL.x -= 100;
				ofPosTL.y -= 100;
				ofPoint ofPosBR(ofPos);
				ofPosBR.x += 100;
				ofPosBR.y += 100;

				ofVec2f depthPtTL = depthStream.worldToCamera(ofPosTL);
				ofVec2f depthPtBR = depthStream.worldToCamera(ofPosBR);

				ofVec2f grayPtTL = toGrayHD(depthPtTL);
				ofVec2f grayPtBR = toGrayHD(depthPtBR);

				cv::Point grayPtTLCV(grayPtTL.x, grayPtTL.y);
				cv::Point grayPtBRCV(grayPtBR.x, grayPtBR.y);
				//cv::rectangle(c, grayPtTLCV , grayPtBRCV , 3);

				//cv::circle(c, cv::Point(grayPt.x, grayPt.y), 4, CV_RGB(0, 0, 0));
				//cv::circle(c, cv::Point(grayPt.x, grayPt.y), 3, CV_RGB(255, 255, 255));
			}
		}
		*/

ofVec2f toGrayHD( ofVec2f depthPt )
{
	ofVec2f grayPt = depthPt * 2;
	grayPt.x -= 320;
	grayPt.y += 32;
	return grayPt;
}


cv::Scalar white(255, 255, 255);
cv::Scalar blue(255,0,0);
cv::Scalar green(0,255,0);
cv::Scalar red(0,0,255);
cv::Scalar black(0,0,0);

void testApp::updateUserTracker( nite::UserTrackerFrameRef& userTrackerFrame )
{
	nite::UserMap userMap = userTrackerFrame.getUserMap();
	const nite::Array<nite::UserData>& userDataArray = userTrackerFrame.getUsers();
	cv::Mat users(userMap.getHeight(), userMap.getWidth(), CV_16UC1, (void*)userMap.getPixels());
	cv::Mat dst(users.size(), CV_8UC3); //debug drawing stuff

	for (int userIndex = 0; userIndex < userDataArray.getSize(); userIndex++)
	{
		const nite::UserData& userData = userDataArray[userIndex];
		nite::UserId id = userData.getId();

		if (userData.isLost())
		{
			appUserMap.erase(id);
		}
		if (!userData.isVisible())
		{
			continue;
		}

		nite::Point3f niteCom = userData.getCenterOfMass();
		ofPoint com(niteCom.x, niteCom.y, niteCom.z);
		ofVec2f com2d = depthStream.worldToCamera(com);
		cv::circle(dst, cv::Point(com2d.x, com2d.y), 5 , red, 2);
		
		if (userData.getSkeleton().getState() == nite::SKELETON_TRACKED)
		{
			appUserMap[id].updateSkeleton(userData);
		}

		//# Contour processing
		cv::Mat u8 = (users == id);

		//detect peaks
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;

		cv::findContours(u8.clone(), contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

		//approximate with lines
		for( size_t k = 0; k < contours.size(); k++ )
		{
			double epsilon = ofMap(com.z, 750, 2000, 15, 3, true); //higher => smoother. TODO: choose as a function of CoM distance (closer=>smaller)
			approxPolyDP(cv::Mat(contours[k]), contours[k], epsilon, true); 
		}

		if( !contours.empty() && !hierarchy.empty() )
		{
			// iterate through all the top-level contours,
			// draw each connected component with its own random color
			for(int ci = 0; ci < hierarchy.size(); ci++)
			{

				vector<cv::Point>& contour = contours[ci];
				cv::drawContours( dst, contours, ci, white, 1, 8, hierarchy );

				if(cv::contourArea(contour) < 100)
				{
					continue;
				}

				//fingerPoint.push_back(contours[idx][0]);

				vector<int> candidates;
				vector<int> valleys;
				vector<int> peaks;

				for (int i = 1; i < contours[ci].size();i++)
				{
					//check if peak
					cv::Point2f vec1 = contour[i - 1] - contour[i];
					cv::Point2f vec2 = contour[(i + 1) % contour.size()] - contour[i];
					cv::Point2f vec3 = contour[(i + 2) % contour.size()] - contour[(i + 1) % contour.size()];

					double vec1norm = cv::norm(vec1);
					double vec2norm = cv::norm(vec2);
					double vec3norm = cv::norm(vec3);

					vec1.x /= vec1norm;
					vec1.y /= vec1norm;
					vec2.x /= vec2norm;
					vec2.y /= vec2norm;
					vec3.x /= vec3norm;
					vec3.y /= vec3norm;


					if (vec1norm < 50)
					{

						if ( vec1.dot(vec2) > 0.1)
						{
							double cr = vec1.cross(vec2);
							bool isPeak = cr > 0;

							//cv::circle(dst,  contour[i], 3, white, 1 );
							//cv::circle(dst,  contour[i], 2, isPeak ? red : blue, -1 );

							candidates.push_back(i);
							if (isPeak)
								peaks.push_back(i);
							else
								valleys.push_back(i);

						}

						if (vec1.dot(vec3) > 0.1 && vec2norm < 50)
						{
							//cv::circle(dst,  contour[i], 3, white, 1 );
							//cv::circle(dst,  contour[i], 2 , green, -1 );
							//stringstream ss; ss << vec2norm;
							//cv::putText(dst, ss.str(), contour[i], CV_FONT_BLACK, 1, white );

							candidates.push_back(i);
							peaks.push_back(i);

						}
					}
				} // i < contour.size


				vector<cv::Point> goodValleys;
				const int valleysMinSize = 2;
				if (valleys.size() > valleysMinSize)
				{
					for (int i = 1; i < valleys.size(); i++)
					{
						if (valleys[i] - valleys[i - 1] < 4)//! TODO: Real Norm... && cv::norm(contour[valleys[i]] - contour[valleys[i-1]]) < 30)
						{
							cv::Point& p = contour[valleys[i]];
							goodValleys.push_back(p);
							//cv::Point midPt(0.5*(contour[valleys[i]] + contour[valleys[i-1]]));
							cv::circle(dst,  p, 2 , green, -1);
							//cv::circle(dst,  midPt, 5 , white, 2);
							//cv::circle(dst, contour[valleys[i]], 5 , red, 2);
							//cv::circle(dst, contour[valleys[i-1]], 5 , red, 2);

							//cv::line(dst, midPt, contour[valleys[i-1] + 1], red); // TODO: paste wing image on shadow
						}
					}

					if (goodValleys.size() > 0)
					{
						vector<ofPoint> realGoodValleys;
						//convert to real world
						for (int i = 0; i < goodValleys.size(); i++)
						{
							cv::Point& pv = goodValleys[i];
							realGoodValleys.push_back(depthStream.cameraToWorld(ofVec2f(pv.x, pv.y)));
						}

						vector<vector<float> > distances;
						vector<int> distanceUnderThresholdRowCount;
						vector<float> distanceUnderThresholdRowWeight;
						const float distanceThreshold = 150.0; //cm
						for (int i = 0; i < realGoodValleys.size(); i++)
						{
							distances.push_back(vector<float>());
							distanceUnderThresholdRowCount.push_back(0);
							distanceUnderThresholdRowWeight.push_back(0.0f);



							for (int j = 0; j < realGoodValleys.size(); j++)
							{
								
								float d = realGoodValleys[i].distance(realGoodValleys[j]);
								if (d > 0 && d < distanceThreshold)
								{
									distanceUnderThresholdRowCount[i]++;
									distanceUnderThresholdRowWeight[i] += d;

									cv::line(dst, goodValleys[i], goodValleys[j], red, 3); // TODO: paste wing image on shadow
									stringstream ss; ss << d;
									cv::putText(dst, ss.str(), 0.5*(goodValleys[i]+goodValleys[j]), CV_FONT_HERSHEY_PLAIN, 1, white );


								}

								distances[i].push_back(d);
							}
						}

						const int countThreshold = 1;
						vector<int> qualifiedIndices; //those who have passed the count threshold
						for (int i = 0; i < distanceUnderThresholdRowCount.size(); i++)
						{
							if (distanceUnderThresholdRowCount[i] > countThreshold)
							{
								qualifiedIndices.push_back(i);
							}
						}


						vector<int> qualifiedIndexCount;
						for (int i = 0; i < qualifiedIndices.size(); i++)
						{
							int ix = qualifiedIndices[i];
							qualifiedIndexCount.push_back(0);
							for (int j = 0; j < qualifiedIndices.size(); j++)
							{
								int jx = qualifiedIndices[j];

								if (distances[ix][jx] < distanceThreshold)
									qualifiedIndexCount[i]++;

							}
						}


						if (qualifiedIndexCount.size() > 0)
						{
							int maxIdx = 0;
							int maxCount = 0;
							for (int i = 0; i < qualifiedIndexCount.size(); i++)
							{
								if (maxCount < qualifiedIndexCount[i])
								{
									maxCount = qualifiedIndexCount[i];
									maxIdx = i;
								}
							}

							cv::Point& p = goodValleys[qualifiedIndices[maxIdx]];
							ofVec2f ofPt(p.x, p.y);
							appUserMap[id].updateHandCluster(ofPt, maxCount);

							const int maxCountThreshold = 3;
							cv::circle(dst, goodValleys[qualifiedIndices[maxIdx]], (maxCount > maxCountThreshold ? 20 : 10), (maxCount >  maxCountThreshold ? green : red), -1);

						}
						else
						{
							appUserMap[id].updateHandCluster(ofVec2f(), 0);
						}
					}

				}


			} // ci < hierarchy.size()
		} // if contours && hierarchy !empty

		if (appUserMap[id].status.valid)
		{
			appUserMap[id].status.valid = false;
			//! TODO: set timeout

			BirdData bd;
			ofVec2f p;
			if (appUserMap[id].status.realWorld)
			{
				p = depthStream.worldToCamera(appUserMap[id].status.position3d);
			}
			else
			{
				p = appUserMap[id].status.position2dHD;
			}
			ofVec2f pHD = toGrayHD(p);
			bd.position = getOrigin() + pHD;

			
			bd.velocity = pHD - toGrayHD(com2d);
			ofNotifyEvent(getBirdEvents().createBird, bd); //TODO send id
		}



	}//userIndex
 


	imshow("dst", dst);
	//imshow("U", u8);


	cv::waitKey(1);

}
