#include "testApp.h"

#include "ofxCv.h"
#include "Utilities.h"

//--------------------------------------------------------------
void testApp::setup(){

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

	recorder.setup();
	recorder.addStream(depthStream.getStream());
	recorder.addStream(colorStream.getStream());
	updateMats();
	allocateTextures();
	
	setupGui(); 
	setupScene();

	toDrawVideo = true;
	gui1->setVisible(false);

	cf.setAutoThreshold(false);
}


void testApp::drawVideo(){
	
	//depthTex.draw(0,0, ofGetWindowWidth(), ofGetWindowHeight());
	ofPushMatrix();
	ofTranslate(-depthTex.getWidth() / 2, -depthTex.getHeight() / 2, 0);
	depthTex.draw(0, 0, -0.1);
	cf.draw();
	//cf.getPolyline(0).draw();
	colorTex.draw(0 ,0, 1);
	ofPopMatrix();

	/*

	//grabcut
	Mat mask;
	mask = Mat::zeros(c.size(), CV_8UC1);
	
	mask.setTo(GC_FGD, depthMask);
	mask.setTo(GC_PR_BGD, invertDepthMask);

	Mat bgdModel;
	Mat fgdModel;
	cv::Rect rect(0,0,mask.cols, mask.rows);
	int iterCount = 3;
	int mode = GC_INIT_WITH_MASK;
//	grabCut(c, mask, rect, bgdModel, fgdModel, 1);
	grabCut(c, mask, rect, bgdModel, fgdModel, 5 , mode);

	//imshow("bgd", bgdModel);
	imshow("mask", mask * 70);
	imshow("color", c);
	*/

}

void testApp::setupGui()
{

	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 255-xInit; 

	gui1 = ofPtr<ofxUICanvas>(new ofxUICanvas(0, 0, length+xInit, ofGetHeight())); 

	gui1->addWidgetDown(new ofxUILabel("Look Up", OFX_UI_FONT_LARGE)); 
	gui1->addWidgetDown(new ofxUILabel("Press 'g' to Hide GUIs", OFX_UI_FONT_LARGE)); 

	gui1->addSpacer(length-xInit, 2);

	gui1->addWidgetDown(new ofxUILabel("Depth Cutoff", OFX_UI_FONT_MEDIUM)); 
	gui1->addRangeSlider("RSLIDER", 0.0, 10000.0, 450.0, 2000.0, length-xInit,dim);
	gui1->addSlider("colorThreshold", 1, 255, 10, length-xInit,dim);
	gui1->addSpacer(length-xInit, 2);
	vector<string> depthModes = depthStream.getVideoModesString();
	//gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim); 
	//gui1->addRadio("Depth Modes", depthModes, OFX_UI_ORIENTATION_VERTICAL, dim, dim); 

	gui1->addSpacer(length-xInit, 2);

	gui1->addWidgetDown(new ofxUILabel("V SLIDERS", OFX_UI_FONT_MEDIUM)); 
	gui1->addSlider("0", 0.0, 10255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui1->addSlider("1", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("2", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("3", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("4", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("5", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("6", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("7", 0.0, 255.0, 150, dim, 160);
	gui1->addSlider("8", 0.0, 255.0, 150, dim, 160);
	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

		
	gui1->addSpacer(length-xInit, 2);
	//gui1->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 
	//gui1->addButton("DRAW GRID", false, dim, dim);
	gui1->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM)); 
	gui1->addToggle( "Draw Grid", &toDrawGrid, dim, dim);
	gui1->addToggle( "Draw Video", &toDrawVideo, dim, dim);
	gui1->addToggle( "Draw Side Viewports", &toDrawSideViewports, dim, dim);

	
	
	
	gui1->addSpacer(length-xInit, 2);
	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM)); 
	gui1->add2DPad("PAD", ofPoint(0,length-xInit), ofPoint(0,120), ofPoint((length-xInit)*.5,120*.5), length-xInit,120);

	gui1->setColorBack(ofColor::gray);
	//gui1->setDrawBack(true);

	gui1->setColorFill(ofColor::gray);
	gui1->setDrawFill(true);

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



}

void testApp::setupScene()
{
	iMainCamera = 0;
	bCamParent = false;

	// user camera
	camEasyCam.setTarget(nodeSwarm);
	camEasyCam.setDistance(100);
	cameras[0] = &camEasyCam;

	// front
	camFront.scale = 200;
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

	setupViewports();

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	nodeSwarm.init(100, 5);
	nodeSwarm.addParticle(5);
}

void testApp::setupViewports(){
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



//--------------------------------------------------------------
void testApp::draw(){
	glDisable(GL_DEPTH_TEST);
	ofPushStyle();
	ofSetColor(100, 100, 100);
	ofRect(viewGrid[iMainCamera]);
	ofPopStyle();
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
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
	ofDrawBitmapString("Camera selected: " + ofToString(iMainCamera + 1), viewMain.x + 20, 50);
	ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
	ofDrawBitmapString("Press 'p' to toggle parents on OrthoCamera's", viewMain.x + 20, 90);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), viewMain.x + 20, 110);
	ofDrawBitmapString(ofToString(nodeSwarm.size()), viewMain.x + 20, 130);
	for (int i = 0; i < outputStrings.size(); i++)
	{
		ofDrawBitmapString(outputStrings[i], viewMain.x + 20, 150 + 20 * i);
	}


	ofDrawBitmapString("EasyCam",   viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("Front",     viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Top",       viewGrid[2].x + 20, viewGrid[2].y + 30);
	ofDrawBitmapString("Left",      viewGrid[3].x + 20, viewGrid[3].y + 30);

	// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for(int i = 0; i < N_CAMERAS; i++){
		ofRect(viewGrid[i]);
	}
	//
	ofRect(viewMain);

	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();
}

void testApp::drawScene(int iCameraDraw){

	nodeSwarm.draw();
	if (toDrawGrid) nodeGrid.draw();

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

	if (toDrawVideo)
	{
		drawVideo();
	}

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

	case 'f': ofToggleFullscreen(); break;
	case 'g': gui1->toggleVisible(); break;
	case 'd': toDrawVideo = !toDrawVideo; break; //TODO: update gui

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
	setupViewports();
}


void testApp::updateMats()
{
	ofPtr<ofPixels> colorPixels = colorStream.getPixels();
	colorMat = ofxCv::toCv(*colorPixels);

	ofPtr<ofShortPixels> depthPixels = depthStream.getPixels();
	depthMat = ofxCv::toCv(*depthPixels);
}

void testApp::allocateTextures()
{
	depthTex.allocate(depthMat.cols, depthMat.rows, GL_LUMINANCE);
	colorTex.allocate(colorMat.cols, colorMat.rows, GL_RGB);
}

void testApp::update()
{
	outputStrings.clear();
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
			outputStrings.push_back(ofToString(a) + ofToString(s));


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

//	m8 = 255 - m8; //black shadow
		
	cv::morphologyEx(m8, m8, CV_MOP_DILATE, cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(11, 11)));

	Mat c2 = c.clone();
	c2.setTo(255);

	ofxUISlider* s = (ofxUISlider*)gui1->getWidget("colorThreshold");
	cvtColor(c, c, CV_RGB2GRAY);
	c = c > s->getScaledValue();
	cvtColor(c, c, CV_GRAY2RGB);
	imshow("c2",c);
	waitKey(1);

	c.copyTo(c2, m8);
	depthTex.loadData(m8.ptr(), m8.cols, m8.rows, GL_LUMINANCE);
	colorTex.loadData(c2.ptr(), c2.cols, c2.rows, GL_RGB);
}

