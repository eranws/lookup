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


	static int dValuesSize2;
	static int dValuesLowThreshold2;
	static int dValuesHighThreshold2;

	std::deque<float> dValues2;
	float dRunAvg2;
	bool dTrigger2;

	std::deque<float> dValuesL;
	float dRunAvgL;
	bool dTriggerL;

	//hand cluster
	static int hcValuesMaxSize;
	static int hcValuesLowThreshold;
	static float hcValuesHighThreshold;

	std::deque<float> hcValues;
	float hcRunAvg;
	bool hcTrigger;


	void updateSkeleton( const nite::UserData& user );
	void updateHandCluster( ofVec2f p, int maxCount );
	//std::vector<string> userOutputStrings;
	struct Status
	{
		Status() : valid(false), lastTime(0){}

		bool valid;
		bool realWorld;
		ofPoint position3d;
		ofVec2f position2dHD;
		ofPoint velocity;
		unsigned long lastTime;
	} status;
};
