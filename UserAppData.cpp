#include "UserAppData.h"

int UserAppData::dValuesSize = 20;
int UserAppData::dValuesLowThreshold = 140;
int UserAppData::dValuesHighThreshold = 300;

int UserAppData::hcValuesMaxSize = 15;
int UserAppData::hcValuesHighThreshold = 2;
int UserAppData::hcValuesLowThreshold = 1;

void UserAppData::updateSkeleton( const nite::UserData& user )
{
	const nite::Skeleton& skeleton = user.getSkeleton();
	const nite::SkeletonJoint& head = skeleton.getJoint(nite::JOINT_HEAD);
	const nite::SkeletonJoint& leftHand = skeleton.getJoint(nite::JOINT_LEFT_HAND);
	const nite::SkeletonJoint& leftElbow = skeleton.getJoint(nite::JOINT_LEFT_ELBOW);
	const nite::SkeletonJoint& rightHand = skeleton.getJoint(nite::JOINT_RIGHT_HAND);
	const nite::SkeletonJoint& rightElbow = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW);

	if (head.getPositionConfidence() > .5)
	{
		//		printf("H: %d. (%5.2f, %5.2f, %5.2f)\n", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
		//userOutputStrings.,		
		//printf("H: %d. %5.2f", user.getId(), head.getPosition().z);
	}

	if (leftHand.getPositionConfidence() > .5 && leftElbow.getPositionConfidence() > .5){
		//	printf("L: %5.2f \t", leftElbow.getPosition().y - leftHand.getPosition().y);
	}


	const nite::SkeletonJoint& j1 = rightHand;
	const nite::SkeletonJoint& j2 = leftHand;

	if (j1.getPositionConfidence() > .5 && j2.getPositionConfidence() > .5){

		const nite::Point3f& np1 = j1.getPosition(); //np - nitePoint
		const nite::Point3f& np2 = j2.getPosition();

		ofPoint p1(np1.x, np1.y, np1.z); //of Point
		ofPoint p2(np2.x, np2.y, np2.z);

		float d = p1.distance(p2);
		//	printf("R: %5.2f \n", rightElbow.getPosition().y - rightHand.getPosition().y);

		//printf("D: %5.2f\t", d); 

		dRunAvg += d;
		dValues.push_front(d);
		if (dValues.size() > dValuesSize)
		{
			dRunAvg -= dValues.back();
			dValues.pop_back();

			//printf("dRunAvg: %.2f\t", dRunAvg); 
			if (dRunAvg < dValuesLowThreshold * dValuesSize && dTrigger)
			{
				dTrigger = false;
				status.valid = true;
				status.position3d = p1.middle(p2);
				status.realWorld = true;
				status.velocity = ofVec3f(ofRandom(-5, 5), ofRandom(-5, 5), ofRandom(-5, 5));
			}
			if (dRunAvg > dValuesHighThreshold * dValuesSize)
			{
				dTrigger = true;
			}
		}	

	}

	//printf("\n"); 

}

void UserAppData::updateHandCluster( ofVec2f p, int maxCount ) 
{
	hcRunAvg += maxCount;
	hcValues.push_front(maxCount);
	if (hcValues.size() > hcValuesMaxSize)
	{
		hcRunAvg -= hcValues.back();
		hcValues.pop_back();

		if (hcRunAvg > hcValuesHighThreshold * hcValuesMaxSize && hcTrigger)
		{
			hcTrigger = false;
			status.valid = true;
			status.position2dHD = p;
			status.realWorld = false;
			status.velocity = ofVec3f(ofRandom(-5, 5), ofRandom(-5, 5), ofRandom(-5, 5));
		}
		if (hcRunAvg < hcValuesLowThreshold * hcValuesMaxSize)
		{
			hcTrigger = true;
		}
	}	
}
