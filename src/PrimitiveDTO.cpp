#include "PrimitiveDTO.h"

void PrimitiveDTO::setup() {
	navCe = 0;
	navCy = 0;
	navF = 0;
}
void PrimitiveDTO::draw() {
	for (int i = 0; i < cerclesManager[navCe].size(); i++) {
		ofSetColor(cerclesManager[navCe][i].couleur);
		ofDrawCircle(cerclesManager[navCe][i].position, cerclesManager[navCe][i].rayon);
	}

	for (int i = 0; i < cylindresManager[navCy].size(); i++) {
		ofSetColor(cylindresManager[navCy][i].couleur);
		ofDrawCircle(cylindresManager[navCy][i].position, cylindresManager[navCy][i].rayon);
	}

	for (int i = 0; i < formesManager[navF].size(); i++) {
		ofSetColor(formesManager[navF][i].couleur);
		formesManager[navF][i].draw();
	}
}

void PrimitiveDTO::draw3d() {

}

void PrimitiveDTO::undoCe() {

}
void PrimitiveDTO::undoCy() {

}
void PrimitiveDTO::undoF() {

}
void PrimitiveDTO::redoCe() {

}
void PrimitiveDTO::redoCy() {

}
void PrimitiveDTO::RedoF() {

}
void PrimitiveDTO::saveCe() {

}
void PrimitiveDTO::saveCy() {

}
void PrimitiveDTO::saveF() {

}