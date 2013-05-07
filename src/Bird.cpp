#include "Bird.h"
#include "ofImage.h"


string filenames[] = {
	"body_mc.png",
	"tail_all_mc.png",
	"wing_1_R_mc.png",
	"wing_2_R_mc.png",
	"wing_1_L_mc.png",
	"wing_2_L_mc.png",
};

float xPos[] = {
//	0, 130.65, 70, 109.25, 70, 109.25
	0, 122, 51 , 84, 51, 84
};
float yPos[] = {
	0, -10, -50, -75, 1, 33
};


float xRot[] = {
	0,	127.4,	67.8,	109.25,	67.8,	000
};

float yRot[] = {
	0,	-25.35,	-43.2,	-71.85,	-5.3,	000
};


Bird::Bird(ofVec3f velocity /*img*/) :
	velocity(velocity)

{

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

const int BodyOffsetX = -138/2 - 30;
void Bird::customDraw()
{
	update();
	ofSetColor(255);
	ofSphere(5.0);
	ofDrawArrow(ofVec3f(), ofVec3f(1, 0, 0) * 10, 1);

	ofPushStyle();
	ofSetColor(color);

	//ofTranslate(-138/2 - 30, -53/2);//png size, -30 shift center of mass towards tail
	ofRotateZ(atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180);
//	setOrientation(ofQuaternion(atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180, ofVec3f(0,0,1)));
	images[0].draw(BodyOffsetX, -53/2);
	//images[0].draw(0, 0);

	for (int i = 0; i < 2; i++)
	{
		wings[i].draw();
	}
	tail.draw();

	//ofDrawArrow(ofPoint(), velocity);


	//int frameIndex = ofGetFrameNum() - animationFrameStart; //fix animation not starting in 0;
	//int animationCounter = (frameIndex + particles[i]->birth) / images.size();
	//ofDrawBitmapString(ofToString(animationCounter), 0, 0, 0);
	//int glidingFactor = 10; //flap wings each X times
	//if ((animationCounter + particles[i]->glideFrameStart) % glidingFactor > 0) frameIndex = particles[i]->animationFrameStart;

	// draw the image sequence at the new frame count

	ofPopStyle();

}


void Bird::update(){

const float SPRING_CONSTANT = 0.1f;
const float MAX_VELOCITY = 30.0f;

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();
	move(velocity * dt);
	velocity += -SPRING_CONSTANT * getPosition() * dt;
	velocity.limit(MAX_VELOCITY);
}


void Bird::setup()
{
//	ofTranslate(-138/3, -53/2);//png size

	wings[0].setPosition(51 + BodyOffsetX, -10, 0);
	wings[0].setImage(images[2]);
	wings[0].setImageOffset(ofPoint(0, 35-100, -1));
	wings[0].setRotationFactor(0.2, 10);

	wings[1].setPosition(51 + BodyOffsetX, 10, 0);
	wings[1].setImage(images[4]);
	wings[1].setImageOffset(ofPoint(0, -35, -1));
	wings[1].setRotationFactor(0.2, -10);


	tail.setPosition(122 + BodyOffsetX, 0, 0);
	tail.setImage(images[1]);
	tail.setImageOffset(ofPoint(-3, -34, -2));
	tail.setRotationFactor(0.1, 30);

}


void Wing::customDraw()
{
	ofBox(5);
	setOrientation(ofQuaternion(sinf(_rotationFreq * float(ofGetFrameNum())) * _rotationMag, getZAxis()));
	_image.draw(_imgOffset);
}

void Wing::setImage( const ofImage& image )
{
	_image = image;
}

void Wing::setImageOffset( const ofPoint offset)
{
	_imgOffset = offset;
}

void Wing::setRotationFactor( float freq ,float mag )
//TODO: change to 3rd order polynom formula
{
	_rotationFreq	= freq;
	_rotationMag	= mag;

}
