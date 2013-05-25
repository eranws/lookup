#include "Swarm.h"

string filenames[] = {
	"blue_bird/PNG Sequence60",
//	"mustard_bird/PNG Sequence",
//	"yellow_bird/PNG Sequence"
};

void initAnimation(string s)
{
	ofDirectory dir;
	//int nFiles = dir.listDir("plops");
	int nFiles = dir.listDir(s);
	if(nFiles) {
		animations.push_back(Animation());
		Animation& images = animations.back();

		for(int i=0; i<dir.numFiles(); i++) { 
			// add the image to the vector
			string filePath = dir.getPath(i);
			images.push_back(ofImage());
			images.back().loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
			images[i].setAnchorPercent(0.5f, 0.5f);
			//images[i].rotate90(2);
		}
	}
}

void swarm::initImages()
{
	for (int i = 0; i < sizeof(filenames) / sizeof(string); i++)
	{
		initAnimation(filenames[i]);
	}
}


// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'

swarm::swarm(){
	light.setAmbientColor(ofColor(0, 0, 0));
}

void swarm::init(float positionDispersion, float velocityDispersion){

	// Check if we've already initialised
	if(particles.size() == 0){
		swarm::initImages();
	}
	else
	{
		ofLogWarning("swarm") <<  "Swarm: Already initialised";
		particles.clear();
	}

	_velocityDispersion = velocityDispersion;
	_positionDispersion = positionDispersion;
}

void swarm::addParticle(ofPoint p, ofVec3f v)
{
	int idx = rand() % animations.size();
	Bird* b = new Bird(v, animations[idx]);
	b->setPosition(ofVec3f(p));

	b->setup();
	particles.push_back(ofPtr<Bird>(b));
}

void swarm::addParticle(int nParticles)
{
	for (int i = 0; i < nParticles; i++)
	{
		ofPoint p(ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f), ofRandom(2, 5));
		p *= _positionDispersion;
		ofVec3f v(ofRandom(-5, 5), ofRandom(-5, 5), ofRandom(-5, 5));
		v *= _velocityDispersion;

		addParticle(p, v);
	}
}

void swarm::customDraw(){

	ofPushStyle();
	light.enable();
	light.setPosition(particles[0]->getGlobalPosition());

	for(int i = 0; i < particles.size(); i++){
		ofSetColor(particles[i]->color);
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
