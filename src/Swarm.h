#pragma once
#include "ofMain.h"
#include "ofFileUtils.h"
#include "Bird.h"

#define SPRING_CONSTANT 0.1f
#define MAX_VELOCITY 30.0f


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
	// we call this update function ourselves
	//  at the beginning of customDraw
	void update();

	// objects
	vector<ofPtr<Bird> > particles;

	float _positionDispersion;
	float _velocityDispersion;

};