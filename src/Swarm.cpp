#include "Swarm.h"

vector<Sound3D> sounds;
Sound3D gplayer;

string soundNames[] = {
	"sound/7898__schluppipuppie__bird006.wav",
	"sound/9325__tigersound__bird-tweet-2.wav",
	"sound/9327__tigersound__bird-tweet-4.wav",
	"sound/983__rhumphries__rbh-bird-nightengale-1.wav",
	"sound/bird1.wav",
	"sound/Bird4.wav",
	"sound/birds014.wav",
	"sound/birds026.wav"
};

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

	sounds.reserve(20);

	for (int i = 0; i < sizeof(soundNames) / sizeof(string); i++)
	{

		sounds.push_back(Sound3D());
		Sound3D& player = sounds.back();
		
		// add the image to the vector
		string filePath = ofToDataPath(soundNames[i]);
				
		player.loadSound(filePath);
		player.setVolume(0.75);
		player.setMultiPlay(true);
		player.play();
		
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


	if (ofGetSystemTime() % 1000 < 10)
	{
		Sound3D& player = sounds[rand() % sounds.size()];
		if(!player.getIsPlaying())
			player.play();
	}


	ofPushStyle();
	for(int i = 0; i < particles.size(); i++){
		particles[i]->draw();
	}
	ofPopStyle();
}