#include "Bird.h"


Bird::Bird(int positionDispersion, int velocityDispersion)
{
	//
		position.x = (ofRandom(1.0f) - 0.5f)  * positionDispersion; //TODO: make member
		position.y = (ofRandom(1.0f) - 0.5f)  * positionDispersion;
		position.z = (ofRandom(1.0f) - 0.5f)  * positionDispersion;

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
