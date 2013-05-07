/*
#include "BirdPart.h"


BirdPart::BirdPart(void)
{
}


BirdPart::~BirdPart(void)
{
}

class Wing : public ofNode
{
	virtual void customDraw();
	ofImage _image;
	ofPoint _imgOffset;
	float _rotationFreq;
	float _rotationMag;

public:
	void setImage( const ofImage& image );
	void setImageOffset( const ofPoint offset);
	void setRotationFactor(float freq ,float mag);
};

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



draw...()
{
	//	setOrientation(ofQuaternion(atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180, ofVec3f(0,0,1)));
	images[0].draw(BodyOffsetX, -53/2);
	//images[0].draw(0, 0);

	for (int i = 0; i < 2; i++)
	{
		wings[i].draw();
	}
	tail.draw();

	//ofDrawArrow(ofPoint(), velocity);


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

*/