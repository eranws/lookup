#pragma once
#include "ofMain.h"
#include "ofFileUtils.h"
#include "Bird.h"

#include "..\Sound3d.h"

typedef vector<ofImage> Animation;
static vector<Animation> animations; 
static vector<const Sound3D> sounds;

class Swarm : public ofNode {

public:
	void init();
	void addParticle(int nParticles = 1);
	void addParticle(ofPoint p, ofVec3f v);
	int size(){return particles.size();}

	void customDraw();

	static void initImages();
	static void initSounds();


protected:
	
	vector<ofPtr<Bird> > particles;
};