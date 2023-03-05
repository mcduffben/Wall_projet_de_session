#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ModelDTO {
public:
	ofColor col;
	int degX, degY, scale;
	ofxAssimpModelLoader model;

	ModelDTO(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo);
};