#include "testApp.h"

#include "ofxCv.h"
#include "Utilities.h"
#include "..\BirdEvents.h"

//--------------------------------------------------------------
void testApp::setup(){

	toDrawVideo = true;
	toDrawScene = false;
	toDrawSideViewports = false;
	showHelpText = true;

	ofSetVerticalSync(true);
	ofSetWindowTitle("LookUp!");
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
	//gui1->loadSettings("settings.xml");

	setupScene();

	gui1->setVisible(false);

	cf.setAutoThreshold(false);

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
	
	shadowTex.drawSubsection(0,0, ofGetWindowWidth(), ofGetWindowHeight(), sx, sy, sw, sh);

	Trees::draw();

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
	//cf.draw();
	//cf.getPolyline(0).draw();
	
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

	gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM));

	gui1->addRangeSlider("GRAY_CROP_W", 0.0, 640, 0.0, 640.0, length-xInit,dim);
	gui1->addRangeSlider("GRAY_CROP_H", 0.0, 1024, 0.0, 1024.0, length-xInit,dim);


	gui1->setDrawBack(true);
	gui1->setColorBack(ofColor::gray);
	
	ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 	

	if(name == "DRAW GRID")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget; 
		//bdrawGrid = button->getValue(); 
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
	}


}

void testApp::setupScene()
{
	iMainCamera = 0;
	bCamParent = false;

	// user camera
	camEasyCam.setTarget(nodeSwarm);
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

	Trees::init("C:/Users/eran.weissenstern/Dropbox/Infected_bird_2/treetop 2/PNG Sequence2_60 9X16");

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	nodeSwarm.init(100, 5);
	nodeSwarm.addParticle(1);
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
	glEnable(GL_DEPTH_TEST);

	// draw main viewport
	cameras[iMainCamera]->begin(viewMain);
	drawScene(iMainCamera);

	// calculate mouse ray whilst this camera is active
	updateMouseRay();

	cameras[iMainCamera]->end();

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
	outputStrings.push_back(ofToString(nodeSwarm.size()));


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

	nodeSwarm.draw();
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

	case 'p':
		if(bCamParent){
			camFront.clearParent();
			camTop.clearParent();
			camLeft.clearParent();

			bCamParent = false;
		}
		else{
			camFront.setParent(nodeSwarm.light);
			camTop.setParent(nodeSwarm.light);
			camLeft.setParent(nodeSwarm.light);

			bCamParent = true;
		}
		break;

	case 'a':
		nodeSwarm.addParticle(1);
		break;
	case 'A':	
		nodeSwarm.addParticle(10);
		break;

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
	cv::Mat fullColorMat = ofxCv::toCv(*colorPixels);
	
//	cout << fullColorMat;
//	int x, y, w, h;
//	colorStream.getStream()->getCropping(&x, &y, &w, &h);
//	colorMat = fullColorMat(cv::Range(y, y+h), cv::Range(x, x+w)).clone();
	colorMat = fullColorMat;
	cv::imshow("colorMat", colorMat);

	colorTex.loadData(colorMat.ptr(), colorMat.cols, colorMat.rows, GL_LUMINANCE);

	ofPtr<ofShortPixels> depthPixels = depthStream.getPixels();
	depthMat = ofxCv::toCv(*depthPixels);
	cv::Mat m8;	depthMat.convertTo(m8, CV_8UC1, 0.25);

	depthTex.loadData(m8.ptr(), m8.cols, m8.rows, GL_LUMINANCE);

}

void testApp::allocateTextures()
{
//	if (depthMat.empty()
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
	outputStrings.push_back("Press 'p' to toggle parents on OrthoCamera's");
	outputStrings.push_back("[d]raw video:" + ofToString(toDrawVideo ? "On" : "Off"));
	outputStrings.push_back("draw [s]cene:" + ofToString(toDrawScene ? "On" : "Off"));
	outputStrings.push_back("side [v]iewports:" + ofToString(toDrawSideViewports ? "On" : "Off"));



	outputStrings.push_back(ofToString(ofGetFrameRate()));
	
	updateMats();
	cvProcess();

}

void testApp::cvProcess()
{
	using namespace cv;

	Mat c = colorMat.clone();
	Mat m = depthMat.clone();
	//	resize(m, m, cv::Size(), 3, 3, CV_INTER_CUBIC);

	ofxUIRangeSlider* r = (ofxUIRangeSlider*)gui1->getWidget("RSLIDER");		

	Mat depthMask = (m > r->getScaledValueLow() &  m < r->getScaledValueHigh());
	Mat invertDepthMask;
	cv::bitwise_not(depthMask, invertDepthMask);

	m.setTo(0, invertDepthMask);
	cv::Mat m8;	m.convertTo(m8, CV_8UC1, 0.25);

	cf.findContours(m8);

	for (int i = 0; i < cf.size(); i++)
	{
		double area = cf.getContourArea(i);
		if (area < 1000) continue;
		cv::Point2f C = cf.getCenter(i);

		ofPolyline pln = cf.getPolyline(i);
		ofPolyline plsm = pln.getSmoothed(10);
		ofPolyline plc = pln.getResampledByCount(30);

		vector<ofPoint> pts = plsm.getVertices();
		for (int pi = 0; pi < pts.size(); pi++)
		{
			ofPoint p = pts[pi];
			ofVec2f va(p.x - C.x, p.y - C.y);
			float a = va.angle(ofVec2f());
			float s = va.length();
			//outputStrings.push_back(ofToString(a) + ofToString(s));
		}


		const ofPolyline& pl = plsm;
		for (int j = 1; j < pl.size(); j++)
		{
			cv::line(m8, cv::Point2d(pl[j-1].x, pl[j-1].y), cv::Point2d(pl[j].x, pl[j].y), cvScalarAll(0), 1);
			cv::line(m8, cv::Point2d(pl[j-1].x, pl[j-1].y), cv::Point2d(pl[j].x, pl[j].y), cvScalarAll(255), 1);
		}


		outputStrings.push_back("Bird #" + ofToString(i));
		outputStrings.push_back(ofToString(area));
		cv::line(m8, C, cf.getCentroid(i), cvScalarAll(0), 3);
		cv::line(m8, C, cf.getCentroid(i), cvScalarAll(255), 2);

	}

	//m8 = 255 - m8; //black shadow
		
	cv::morphologyEx(m8, m8, CV_MOP_DILATE, cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(11, 11)));

	
	ofxUISlider* s = (ofxUISlider*)gui1->getWidget("colorThreshold");
//	cvtColor(c, c, CV_RGB2GRAY);

	c = c > s->getScaledValue();
//	imshow("c",c);

//	Mat c2;
//	cvtColor(c, c2, CV_GRAY2RGB);

//	imshow("c2",c2);
	waitKey(1);

	//c2.setTo(255);
	//c.copyTo(c2, m8); fix Res

	
	shadowTex.loadData(c.ptr(), c.cols, c.rows, GL_LUMINANCE);
	
}

void testApp::createBird( BirdData& p )
{
	printf("TESTAPP: CREATE BIRD");
	cout << p.position << " " << p.velocity << endl;
	nodeSwarm.addParticle(p.position, p.velocity);
}

void testApp::exit()
{
	depthStream.exit();
	colorStream.exit();
}



