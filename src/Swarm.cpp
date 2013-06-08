#include "Swarm.h"

string filenames[] = {
	"blue_bird/PNG Sequence60",
	"purple_bird/PNG Sequence60",
	"mustard_bird/PNG Sequence60",
	"yellow_bird/PNG Sequence60"
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

			ofImage& image = images.back();
			image.loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
			images[i].setAnchorPercent(0.5f, 0.5f);
			//images[i].rotate90(2);
		}
	}
}

void Swarm::initImages()
{
	for (int i = 0; i < sizeof(filenames) / sizeof(string); i++)
	{
		initAnimation(filenames[i]);
	}
}


void Swarm::init(){

	// Check if we've already initialised
	if(particles.size() == 0){
		Swarm::initImages();
	}
	else
	{
		ofLogWarning("swarm") <<  "Swarm: Already initialised";
		particles.clear();
	}

}

void Swarm::addParticle(ofPoint p, ofVec3f v)
{
	int idx = rand() % animations.size();
	Bird* b = new Bird(v, animations[idx]);
	b->setPosition(ofVec3f(p));

	b->setup();
	particles.push_back(ofPtr<Bird>(b));
}

void Swarm::addParticle(int nParticles)
{
	for (int i = 0; i < nParticles; i++)
	{
		ofPoint p(320, 512, 5);
		ofVec3f v(1, 0, 0);

		addParticle(p, v);
	}
}

void Swarm::customDraw(){

	ofPushStyle();
	for(int i = 0; i < particles.size(); i++){
		particles[i]->draw();
	}
	ofPopStyle();
}