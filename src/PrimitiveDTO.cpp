#include "PrimitiveDTO.h"

void PrimitiveDTO::setup() {
	navCe = 0;
	navCy = 0;
	navF = 0;
	navM = 0;
	navigator = -1;
	vector<CercleDTO> c;
	cerclesManager.push_back(c);
	vector<CylindreDTO> cy;
	cylindresManager.push_back(cy);
	vector<ModelDTO> m;
	modelManager.push_back(m);

	//setup des illuminations
	//setup de ton menu
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

	//activation des illuminations

	//vecteur des spheres qu on itere en dessinant chaque sphere

	for (int i = 0; i < cerclesManager[navCe].size(); i++) {
		ofSetColor(cerclesManager[navCe][i].couleur);
		ofDrawSphere(cerclesManager[navCe][i].position, cerclesManager[navCe][i].rayon);
	}
	//vecteur des spheres qu on itere en dessinant chaque cylindres

	for (int i = 0; i < cylindresManager[navCy].size(); i++) {
		ofSetColor(cylindresManager[navCy][i].couleur);
		ofDrawCylinder(cylindresManager[navCy][i].position, cylindresManager[navCy][i].rayon, cylindresManager[navCy][i].height);
	}


	//vecteur des spheres qu on itere en dessinant chaque modele

	for (int i = 0; i < modelManager[navM].size(); i++) {
		ofPushMatrix();
		ofRotateXDeg(modelManager[navM][i].degX);
		ofRotateYDeg(modelManager[navM][i].degY);
		ofSetColor(modelManager[navM][i].col);
		modelManager[navM][i].model.drawFaces();
		ofPopMatrix();
	}

	//desactivation des illuminations


	//draw menu illumination
}

void PrimitiveDTO::undoCe() {
	if (navCe > 0) {
		navCe -= 1;
	}
}
void PrimitiveDTO::undoCy() {
	if (navCy > 0) {
		navCy -= 1;
	}
}
void PrimitiveDTO::undoF() {

}
void PrimitiveDTO::undoM() {
	if (navM > 0) {
		navM -=1;
	}
}
void PrimitiveDTO::redoCe() {
	if (navCe != cerclesManager.size() - 1) {
		navCe += 1;
	}
}
void PrimitiveDTO::redoCy() {
	if (navCy != cylindresManager.size() - 1) {
		navCy += 1;
	}
}
void PrimitiveDTO::redoF() {

}

void PrimitiveDTO::redoM() {
	if (navM != modelManager.size() - 1)
	{
		navM += 1;
	}
}
void PrimitiveDTO::saveCe(int r, ofVec3f p, ofColor c) {
	cleanIrrelevantHistorique();
	CercleDTO ce(r, p, c);
	vector < CercleDTO > mod = cerclesManager[navCe];
	cerclesManager.push_back(mod);
	navCe += 1;
	cerclesManager[navCe].push_back(ce);
	historique.push_back(1);
	navigator += 1;
}
void PrimitiveDTO::saveCy(int r, int h, ofVec3f p, ofColor c) {
	cleanIrrelevantHistorique();
	CylindreDTO cy(r, h, p, c);
	vector<CylindreDTO> mod = cylindresManager[navCy];
	cylindresManager.push_back(mod);
	navCy += 1;
	cylindresManager[navCy].push_back(cy);
	historique.push_back(2);
	navigator += 1;
}
void PrimitiveDTO::saveF() {

}

void PrimitiveDTO::saveM(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo) {
	cleanIrrelevantHistorique();
	ModelDTO m(dex, dey, sc, co, mo);
	vector<ModelDTO> mod = modelManager[navM];
	modelManager.push_back(mod);
	navM += 1;
	modelManager[navM].push_back(m);
	historique.push_back(3);
	navigator += 1;
}

void PrimitiveDTO::undo() {
	if(historique.size()>0 && navigator >-1){
		if(historique[navigator]==1)undoCe();
		if (historique[navigator] == 2)undoCy();
		if (historique[navigator] == 3)undoM();
		navigator -= 1;
	}
}

void PrimitiveDTO::redo() {
	if (historique.size() > 0) {
		if(navigator!=historique.size()-1)navigator += 1;
		if (historique[navigator] == 1)redoCe();
		if (historique[navigator] == 2)redoCy();
		if (historique[navigator] == 3)redoM();
	}
}

void PrimitiveDTO::cleanIrrelevantHistorique() {
	for (int i = navCe; i < cerclesManager.size()-1;i++) {
		int l = cerclesManager.size();
		cerclesManager.erase(cerclesManager.begin()+l-1);
	}
	for (int i = navCy; i < cylindresManager.size() - 1; i++) {
		int m = cylindresManager.size();
		cylindresManager.erase(cylindresManager.begin() + m - 1);
	}
	for (int i = navM; i < modelManager.size() - 1; i++) {
		int n = modelManager.size();
		modelManager.erase(modelManager.begin() + n - 1);
	}
	for (int i = navigator; i < historique.size() - 1; i++) {
		int p = historique.size();
		historique.erase(historique.begin() + p - 1);
	}
}