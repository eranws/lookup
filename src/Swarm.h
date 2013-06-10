#pragma once
#include "ofMain.h"
#include "ofFileUtils.h"
#include "Bird.h"

#include "..\Sound3d.h"
#include "..\BirdEvents.h"

typedef vector<ofImage> Animation;
static vector<Animation> animations; 

class Swarm : public ofNode {

public:
	void init();
	void addParticle(int nParticles = 1);
	void addParticle(BirdData& bd);
	int size(){return particles.size();}

	void customDraw();

	static void initImages();
	static void initSounds();


protected:
	
	vector<ofPtr<Bird> > particles;

};