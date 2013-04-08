#pragma once
#include "ofMain.h"
#include "ofFileUtils.h"
#include "Bird.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'
class swarm : public ofNode {

public:
	swarm();
	void init(float positionDispersion, float velocityDispersion);
	void addParticle(int nParticles = 1);
	int size();

	void customDraw();
	ofLight light;

protected:
	
	vector<ofPtr<Bird> > particles;

	float _positionDispersion;
	float _velocityDispersion;

};