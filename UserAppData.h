#pragma once
#include <deque>
#include <map>
#include "ofMain.h"
#include "NiTE.h"

class UserAppData
{
public:
	UserAppData() : dRunAvg(0.0f), dTrigger(false)
	{
	}

	//Todo: classify 'd', for different joint types and conditions
	static int dValuesSize;
	static int dValuesLowThreshold;
	static int dValuesHighThreshold;
	std::deque<float> dValues;
	float dRunAvg;
	bool dTrigger;

	void update( const nite::UserData& user );
	//std::vector<string> userOutputStrings;
	struct Status
	{
		bool valid;
		ofPoint position; //realWorld
		ofPoint velocity; //realWorld
	} status;
};
