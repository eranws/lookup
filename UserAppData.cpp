#include "UserAppData.h"

int UserAppData::dValuesSize = 5;
int UserAppData::dValuesLowThreshold = 140;
int UserAppData::dValuesHighThreshold = 300;

int UserAppData::dValuesSize2 = 60;
int UserAppData::dValuesLowThreshold2 = 10;
int UserAppData::dValuesHighThreshold2 = -20;

int UserAppData::hcValuesMaxSize = 3;
float UserAppData::hcValuesHighThreshold = 1.5;
int UserAppData::hcValuesLowThreshold = 1;

void UserAppData::updateSkeleton( const nite::UserData& user )
{
	const nite::Skeleton& skeleton = user.getSkeleton();
	const nite::SkeletonJoint& head = skeleton.getJoint(nite::JOINT_HEAD);
	const nite::SkeletonJoint& leftHand = skeleton.getJoint(nite::JOINT_LEFT_HAND);
	const nite::SkeletonJoint& leftElbow = skeleton.getJoint(nite::JOINT_LEFT_ELBOW);
	const nite::SkeletonJoint& rightHand = skeleton.getJoint(nite::JOINT_RIGHT_HAND);
	const nite::SkeletonJoint& rightElbow = skeleton.getJoint(nite::JOINT_RIGHT_ELBOW);
	const nite::SkeletonJoint& rightShoulder = skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER);
	const nite::SkeletonJoint& leftShoulder = skeleton.getJoint(nite::JOINT_LEFT_SHOULDER);

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
			}
			if (dRunAvg > dValuesHighThreshold * dValuesSize)
			{
				dTrigger = true;
			}
		}	

	}

	//printf("\n"); 

	//hand above shoulder
	{
		const nite::SkeletonJoint& jH = rightHand;
		const nite::SkeletonJoint& jE = rightShoulder;

		if (jH.getPositionConfidence() > .5 && jE.getPositionConfidence() > .5){

			const nite::Point3f& npH = jH.getPosition(); //np - nitePoint
			const nite::Point3f& npE = jE.getPosition();

			ofPoint pH(npH.x, npH.y, npH.z); //of Point
			ofPoint pE(npE.x, npE.y, npE.z);

			float d = (pH - pE).y;
			//	printf("R: %5.2f \n", rightElbow.getPosition().y - rightHand.getPosition().y);

			//printf("D: %5.2f\t", d); 
			dRunAvg2 += d;
			dValues2.push_front(d);
			if (dValues2.size() > dValuesSize2)
			{
				dRunAvg2 -= dValues2.back();
				dValues2.pop_back();

				//printf("dRunAvg: %.2f\t", dRunAvg); 
				if (dRunAvg2 > dValuesLowThreshold2 * dValuesSize2 && dTrigger2)
				{
					dTrigger2 = false;
					status.valid = true;
					status.position3d = pH;
					status.realWorld = true;
				}
				if (dRunAvg2 < dValuesHighThreshold2 * dValuesSize2 )
				{
					dTrigger2 = true;
				}
			}	
		}
	}



	//hand above shoulder
	{
		const nite::SkeletonJoint& jH = leftHand;
		const nite::SkeletonJoint& jE = leftShoulder;

		if (jH.getPositionConfidence() > .5 && jE.getPositionConfidence() > .5){

			const nite::Point3f& npH = jH.getPosition(); //np - nitePoint
			const nite::Point3f& npE = jE.getPosition();

			ofPoint pH(npH.x, npH.y, npH.z); //of Point
			ofPoint pE(npE.x, npE.y, npE.z);

			float d = (pH - pE).y;
			//	printf("R: %5.2f \n", rightElbow.getPosition().y - rightHand.getPosition().y);

			//printf("D: %5.2f\t", d); 
			dRunAvgL += d;
			dValuesL.push_front(d);
			if (dValuesL.size() > dValuesSize2)
			{
				dRunAvgL -= dValuesL.back();
				dValuesL.pop_back();

				//printf("dRunAvg: %.2f\t", dRunAvg); 
				if (dRunAvgL > dValuesLowThreshold2 * dValuesSize2 && dTriggerL)
				{
					dTriggerL = false;
					status.valid = true;
					status.position3d = pH;
					status.realWorld = true;
				}
				if (dRunAvgL < dValuesHighThreshold2 * dValuesSize2 )
				{
					dTriggerL = true;
				}
			}	
		}
	}



}

void UserAppData::updateHandCluster( ofVec2f p, int maxCount ) 
{
	hcRunAvg += maxCount;
	hcValues.push_front(maxCount);
	if (hcValues.size() > hcValuesMaxSize)
	{
		hcRunAvg -= hcValues.back();
		hcValues.pop_back();

		if (hcRunAvg > (hcValuesHighThreshold * hcValuesMaxSize) && hcTrigger)
		{
			hcTrigger = false;
			status.valid = true;
			status.position2dHD = p;
			status.realWorld = false;
		}
		if (hcRunAvg < hcValuesLowThreshold * hcValuesMaxSize)
		{
			hcTrigger = true;
		}
	}	
}
