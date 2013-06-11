#include "Trees.h"
#include "ofFileUtils.h"
#include "ofImage.h"
#include "ofMain.h"

vector <ofImage> Trees::images;

void Trees::init(string dirString)
{
	ofDirectory dir;
	//int nFiles = dir.listDir("plops");
	int nFiles = dir.listDir(dirString);//"/treetop/PNG Sequence");
	if(nFiles) {        
		images.reserve(nFiles);
		for(int i=0; i<dir.numFiles(); i++) { 
			// add the image to the vector
			string filePath = dir.getPath(i);
			images.push_back(ofImage());
			images.back().loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
//			images[i].setAnchorPercent(0.5f, 0.5f);
//			images[i].rotate90(2);
		}
	}
}

void Trees::draw(ofRectangle r)
{
	static int rw = 0;

	int f = ofGetFrameNum();
	if (f % 4 == 0)
	{
		//rw+=(rand()%2)*2-1;
		rw++;
//		rw += (rand() % 2) * 2 - 1;
		//) ? 0 : (rand() % 3) - 1;// ? 1 : (rand() % 4) ? 0 : -1; //random walk
	}
	//rw += (rand() % 4) ? 0 : (rand() % 3) - 1;// ? 1 : (rand() % 4) ? 0 : -1; //random walk
	
	int i = rw % images.size();

	images[i].draw(r);
		//	images[i].draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

}
