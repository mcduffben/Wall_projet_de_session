#pragma once

#include "ofMain.h"

class LineDTO
{

public:

	ofVec2f pinit, pfinal;
	bool selected;

	LineDTO(ofVec2f pi, ofVec2f pf);
	LineDTO(ofVec2f pi, ofVec2f pf, bool select);
};

