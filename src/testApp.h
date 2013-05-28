#pragma once

#include "ofMain.h"
#include "ofxOniDevice.h"
#include "ofxDepthStream.h"
#include "ofxColorStream.h"
#include "ofxRecorder.h"
#include "ofxUI.h"

#include "Swarm.h"
#include "Grid.h"
#include "OrthoCamera.h"
#include "ContourFinder.h"
#include "..\Trees.h"

#define N_CAMERAS 4

#include "NiTE.h"



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void windowResized(int w, int h);

private:
		ofxOniDevice oniDevice;
		ofxDepthStream depthStream;
		ofxColorStream colorStream;
		ofxRecorder recorder;

		ofPtr<ofxUICanvas> gui1;   	
		void setupGui(); 
		void guiEvent(ofxUIEventArgs &e);
		void setupScene();

		void setupViewports(bool sideViews = true);
		void drawScene(int iCameraDraw);
		void updateMouseRay();



		//cameras (all these inherit from ofCamera)
		ofEasyCam camEasyCam;
		orthoCamera camFront;
		orthoCamera camTop;
		orthoCamera camLeft;

		//cameras have parent?
		bool bCamParent;

		//camera pointers
		ofCamera * cameras[N_CAMERAS];
		int iMainCamera;

		//viewports
		ofRectangle viewMain;
		ofRectangle viewGrid[N_CAMERAS];

		//my custom node
		swarm nodeSwarm;
		grid nodeGrid;

		//ray drawn under mouse cursor [start,end]
		ofVec3f ray[2];

		bool toDrawVideo;
		bool toDrawScene;
		bool toDrawGrid;
		bool toDrawSideViewports;
		bool showHelpText;

		ofxCv::ContourFinder cf;
		std::vector<string> outputStrings;

		void drawShadow();
		void drawVideo();
		void drawVideo2D();

		void updateMats();
		cv::Mat colorMat;
		cv::Mat depthMat;

		void allocateTextures();
		void cvProcess();


		ofTexture depthTex;
		ofTexture colorTex;
		ofTexture shadowTex;

};

