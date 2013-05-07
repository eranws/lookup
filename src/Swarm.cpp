#include "Swarm.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'

swarm::swarm(){
	light.setAmbientColor(ofColor(0, 0, 0));
}

void swarm::init(float positionDispersion, float velocityDispersion){

	// Check if we've already initialised
	if(particles.size() == 0){
		Bird::initImages();
	}
	else
	{
		ofLogWarning("swarm") <<  "Swarm: Already initialised";
		particles.clear();
	}

	_velocityDispersion = velocityDispersion;
	_positionDispersion = positionDispersion;
}

void swarm::addParticle(int nParticles)
{
	for (int i = 0; i < nParticles; i++){

		ofVec3f v(
			ofRandom(-5, 5)  * _velocityDispersion,
			ofRandom(-5, 5)  * _velocityDispersion,
			ofRandom(-5, 5)  * _velocityDispersion);

		Bird* b = new Bird(v);
		b->	setPosition(ofVec3f(ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f)) * _positionDispersion); //TODO: make member

		b->setup();
		particles.push_back(ofPtr<Bird>(b));
	}
}

void swarm::customDraw(){

	ofPushStyle();
	light.enable();
	light.setPosition(particles[0]->getGlobalPosition());

	for(int i = 0; i < particles.size(); i++){
		particles[i]->draw();
	}

	light.disable();
	ofDisableLighting();

	// Render light as white sphere
	ofSetColor(255, 255, 255);
	ofSphere(light.getPosition(), 2.0);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(" light", particles[0]->getGlobalPosition());
	ofPopStyle();
}


int swarm::size()
{
	return particles.size();
}
