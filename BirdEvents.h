#pragma once

#include "ofEvents.h"
#include "NiTE.h"

struct UserDataArray
{
	UserDataArray(const nite::Array<nite::UserData>& d):data(d){}
	const nite::Array<nite::UserData>& data;
};

struct BirdData
{
	ofPoint position;
	ofVec3f velocity;
};

class BirdEvents{
public:
	ofEvent<BirdData> createBird;
	ofEvent<UserDataArray> updateUsers;
};

BirdEvents& getBirdEvents();



template<class ListenerClass>
void registerBirdEvents(ListenerClass * listener){
	ofAddListener(getBirdEvents().createBird, listener, &ListenerClass::createBird);
	ofAddListener(getBirdEvents().updateUsers, listener, &ListenerClass::updateUsers);

}