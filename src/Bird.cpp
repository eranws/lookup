#include "Bird.h"


Bird::Bird(int positionDispersion, int velocityDispersion)
{
	//
		setGlobalPosition(ofVec3f(ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f), ofRandom(-0.5f, 0.5f)) * positionDispersion); //TODO: make member

		velocity.x = (ofRandom(1.0f) - 0.5f)  * velocityDispersion * 10; //TODO: make member
		velocity.y = (ofRandom(1.0f) - 0.5f)  * velocityDispersion * 10;
		velocity.z = (ofRandom(1.0f) - 0.5f)  * velocityDispersion;

		color.r = ofRandom(255.0f);
		color.g = ofRandom(255.0f);
		color.b = 150.0f;
		color.a = 255.0f;

		animationFrameStart = 11;
		birth = rand();
		glideFrameStart = rand();
}


Bird::~Bird(void)
{
}

void Bird::customDraw()
{
	for (int i = 0; i < 2; i++)
	{
		wings[i].draw();
	}
}

void Bird::setup()
{
	wings[1].setScale(1, -1, 1);
	for (int i = 0; i < 2; i++)
	{
		wings[i].setParent(*this);
		wings[i].move(10, 0, 0);
	}
}
