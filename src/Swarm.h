#pragma once
#include "ofMain.h"
#include "ofFileUtils.h"
#include "Bird.h"

typedef vector<ofImage> Animation;
static vector<Animation> animations; 

class Swarm : public ofNode {

public:
	void init();
	void addParticle(int nParticles = 1);
	void addParticle(ofPoint p, ofVec3f v);
	int size(){return particles.size();}

	void customDraw();

	static void initImages();


protected:
	
	vector<ofPtr<Bird> > particles;
};