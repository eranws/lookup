#pragma once
#include <deque>
#include <map>
#include "ofMain.h"
#include "NiTE.h"

class UserAppData
{
public:
	UserAppData() : dRunAvg(0.0f), dTrigger(false), hcRunAvg(0.0), hcTrigger(false)
	{
	}

	//Todo: classify 'd', for different joint types and conditions
	static int dValuesSize;
	static int dValuesLowThreshold;
	static int dValuesHighThreshold;
	std::deque<float> dValues;
	float dRunAvg;
	bool dTrigger;

	//hand cluster
	static int hcValuesMaxSize;
	static int hcValuesLowThreshold;
	static int hcValuesHighThreshold;

	std::deque<float> hcValues;
	float hcRunAvg;
	bool hcTrigger;


	void updateSkeleton( const nite::UserData& user );
	void updateHandCluster( ofVec2f p, int maxCount );
	//std::vector<string> userOutputStrings;
	struct Status
	{
		Status() : valid(false){}

		bool valid;
		bool realWorld;
		ofPoint position3d;
		ofVec2f position2dHD;
		ofPoint velocity;
	} status;
};
