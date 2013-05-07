#include "Trees.h"
#include "ofFileUtils.h"
#include "ofImage.h"

vector <ofImage> Trees::images;


Trees::Trees(void)
{
}


Trees::~Trees(void)
{
}

void Trees::init(/*TODO: input*/)
{
	ofDirectory dir;
	//int nFiles = dir.listDir("plops");
	int nFiles = dir.listDir("PNG_yellow_new"); //\treetop\PNG Sequence
	if(nFiles) {        
		for(int i=0; i<dir.numFiles(); i++) { 
			// add the image to the vector
			string filePath = dir.getPath(i);
			images.push_back(ofImage());
			images.back().loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
			images[i].setAnchorPercent(0.5f, 0.5f);
			images[i].rotate90(2);
		}
	}
}

void Trees::draw()
{
	int i = ofGetFrameNum() % images.size();
	images[i].draw(0,0);
}
