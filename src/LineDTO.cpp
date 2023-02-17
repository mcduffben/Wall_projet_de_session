#include "LineDTO.h"


LineDTO::LineDTO(ofVec2f pi, ofVec2f pf) {
	pinit = pi;
	pfinal = pf;
	selected = false;
}

LineDTO::LineDTO(ofVec2f pi, ofVec2f pf, bool select) {
	pinit = pi;
	pfinal = pf;
	selected = select;
}
