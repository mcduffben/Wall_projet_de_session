#include "LineDTO.h"
#include <math.h>


LineDTO::LineDTO(ofVec2f pi, ofVec2f pf) {
	pinit = pi;
	pfinal = pf;
	selected = false;
	hauteur = 100;
	largeur = sqrt(pow(pf.x - pi.x, 2) + pow(pf.y - pi.y, 2));
	epaisseur = 30;
	angle = atan((pf.y-pi.y)/(pf.x-pi.x));
}

LineDTO::LineDTO(ofVec2f pi, ofVec2f pf, bool select) {
	pinit = pi;
	pfinal = pf;
	selected = select;
	hauteur = 500;
	largeur = sqrt(pow(pf.x - pi.x, 2) + pow(pf.y - pi.y, 2));
	epaisseur = 30;
}
