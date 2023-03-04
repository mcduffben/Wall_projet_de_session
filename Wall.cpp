#pragma once
#include "Wall.h"


Walls::Walls(ofVec3f pi, ofVec3f pf, int epais) {
	pi.z = pi.y;
	pi.y = 0;

	pf.z = pf.y;
	pf.y = 0;

	pinit = pi;
	pfinal = pf;
	ofVec3f mirror ;
	mirror = pi;
	mirror.y = 100;
	mpinit = mirror;	
	mirror = pf;
	mirror.y = 100;
	mpfinal = mirror;
	epaisseur = epais;
			
}
