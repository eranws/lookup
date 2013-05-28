#pragma once

#include "ofEvents.h"

struct BirdData
{
	ofPoint position;
	ofVec3f velocity;
};
class BirdEvents{
public:
	ofEvent<BirdData> createBird;
	//TODO: more events here...
};

BirdEvents& getBirdEvents();



template<class ListenerClass>
void registerBirdEvents(ListenerClass * listener){
	ofAddListener(getBirdEvents().createBird, listener, &ListenerClass::createBird);
}