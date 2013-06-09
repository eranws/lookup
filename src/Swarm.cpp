#include "Swarm.h"

string animationDirNames[] = {
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
	for (int i = 0; i < sizeof(animationDirNames) / sizeof(string); i++)
	{
		initAnimation(animationDirNames[i]);
	}
}


void Swarm::initSounds()
{

	ofVec3f lposition, lvelocity, lforward, lup;
	ofVec3f sposition, svelocity;

	lvelocity.set(1, 1, 1);
	lup.set(0, 1, 0);
	lforward.set(0, 0, 1);
	lposition.set(0, 0, 0);

	sposition.set(3, 3, 2);
	svelocity.set(1, 1, 1);

	


	ofDirectory dir;
	
	int nFiles = dir.listDir("/sound");
	if(nFiles) {
		
		for(int i=0; i<dir.numFiles(); i++)
		{ 
			Sound3D player;

			// add the image to the vector
			string filePath = dir.getPath(i);

			player.loadSound(filePath);
			player.setVolume(0.75);
			player.setMultiPlay(true);

			player.updateListener(lposition, lvelocity, lforward, lup);
			player.updateSound(sposition, svelocity);
			player.update();

			player.play();  

			sounds.push_back(player);
		}
	}
}


void Swarm::init(){

	// Check if we've already initialised
	if(particles.size() == 0){
		Swarm::initImages();
		Swarm::initSounds();
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


	Sound3D& player = sounds.back();
	if(!player.getIsPlaying())
		player.play();


	ofPushStyle();
	for(int i = 0; i < particles.size(); i++){
		particles[i]->draw();
	}
	ofPopStyle();
}