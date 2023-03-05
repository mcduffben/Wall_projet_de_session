#pragma once

#include "ofMain.h"

class LineDTO
{

public:

	int hauteur, largeur, epaisseur,angle;
	int diffx, diffy;
	ofVec2f pinit, pfinal;
	bool selected, hasImage;
	ofImage img;

	LineDTO(ofVec2f pi, ofVec2f pf);
	LineDTO(ofVec2f pi, ofVec2f pf, bool select);
};

