#pragma once

#include "ofMain.h"

class CercleDTO {
public:
	int rayon;
	ofVec3f position;
	ofColor couleur;

	CercleDTO(int r, ofVec3f p, ofColor c);
};