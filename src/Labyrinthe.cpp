#include "Labyrinthe.h"

void Labyrinthe::setup() {
	epaisseur = 1000;
	hauteur = 1000;
	echelle = 5;
	posEntreeX = ofGetWindowWidth() / 2;
	posEntreeY = ofGetWindowHeight() / 2 + hauteur/2;
	posSortieX = ofGetWindowWidth() / 2;
	posSortieY = ofGetWindowHeight() / 2 - hauteur/2;
	jeu_arbitraire = 20;
}

void Labyrinthe::update() {
	ofSetLineWidth(10);
	posEntreeX = ofGetWindowWidth() / 2;
	posEntreeY = ofGetWindowHeight() / 2 + hauteur / 2;
	posSortieX = ofGetWindowWidth() / 2;
	posSortieY = ofGetWindowHeight() / 2 - hauteur / 2;
}

void Labyrinthe::draw() {
	ofSetColor(0,0,0);
	ofFill();
	ofDrawPlane(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0,epaisseur,hauteur);
	ofSetColor(255, 255, 255);
	if (murs2Dbasique.size() > 0) {
		for (int i = 0; i < murs2Dbasique.size(); i++) {
			if (murs2Dbasique[i].selected)ofSetColor(0, 0, 175);
			else ofSetColor(255, 255, 255);
			ofDrawLine(murs2Dbasique[i].pinit, murs2Dbasique[i].pfinal);
		}
	}
	ofSetColor(0,175,0);
	ofDrawPlane(posEntreeX,posEntreeY, 0, 20, 20);
	ofSetColor(175, 0, 0);
	ofDrawPlane(posSortieX, posSortieY, 0, 20, 20);
}

void Labyrinthe::setNewLineX(int x) {
	this->newLineX = x;
}
void Labyrinthe::setNewLineY(int y) {
	this->newLineY = y;
}
void Labyrinthe::addNewLine(int x, int y) {
	ofVec2f pi, pf;
	pi.x = newLineX;
	pi.y = newLineY;
	pf.x = x;
	pf.y = y;
	if (pi.x < pf.x) {
		LineDTO newLine(pi, pf);
		murs2Dbasique.push_back(newLine);
	}
	else {
		LineDTO newLine(pf, pi);
		murs2Dbasique.push_back(newLine);
	}
}


void Labyrinthe::addNewLine(ofVec2f pi, ofVec2f pf,bool select) {
	if(pi.x<pf.x){ 
		LineDTO newLine(pi, pf, select);
		murs2Dbasique.push_back(newLine);
	}
	else {
		LineDTO newLine(pf, pi, select);
		murs2Dbasique.push_back(newLine);
	}
}

void Labyrinthe::unselect_all() {
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		murs2Dbasique[i].selected = false;
	}
}

bool Labyrinthe::selectCheckerSingle(int x, int y) {
	unselect_all();
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		if ((murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x) < jeu_arbitraire && (murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x) > -jeu_arbitraire) {
			if (murs2Dbasique[i].pfinal.y > murs2Dbasique[i].pinit.y) {
				if (x < (murs2Dbasique[i].pfinal.x + jeu_arbitraire) && x > (murs2Dbasique[i].pfinal.x - jeu_arbitraire) && y < murs2Dbasique[i].pinit.y
					&& y>murs2Dbasique[i].pfinal.y) {
					murs2Dbasique[i].selected = true;
					return true;
				}
			}
			else {
				if (x < (murs2Dbasique[i].pfinal.x + jeu_arbitraire) && x > (murs2Dbasique[i].pinit.x - jeu_arbitraire) && y < murs2Dbasique[i].pinit.y
					&& y>murs2Dbasique[i].pfinal.y) {
					murs2Dbasique[i].selected = true;
					return true;
				}
			}
		}
		float pente, borigine;
		pente = (murs2Dbasique[i].pfinal.y - murs2Dbasique[i].pinit.y) / (murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x);
		borigine = murs2Dbasique[i].pinit.y - pente * murs2Dbasique[i].pinit.x;
		if ((y - jeu_arbitraire) < (pente * x + borigine) && (y + jeu_arbitraire) > (pente * x + borigine)
			&& x> murs2Dbasique[i].pinit.x && x< murs2Dbasique[i].pfinal.x) {
			murs2Dbasique[i].selected = true;
			return true;
		}
	}
	return false;
}