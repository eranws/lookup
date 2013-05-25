#pragma once
#include "ofImage.h"
	class Trees
{
public:
	static void init(string dirString);
	static void draw();

private:
	Trees(void);
	~Trees(void);
	static vector <ofImage> images;


};

