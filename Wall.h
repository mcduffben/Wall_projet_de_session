#pragma once
#include "ofMain.h"

class Walls
{

public:

	ofVec3f pinit, pfinal,mpinit,mpfinal;

	int epaisseur;
	
	Walls(ofVec3f pi, ofVec3f pf,int epaisseur);

};