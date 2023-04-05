#pragma once

#include "ofMain.h"

class CylindreDTO
{
public:
	int height, rayon;
	ofVec3f position;
	ofColor couleur;

	CylindreDTO(int r, int h, ofVec3f p, ofColor c);
};


