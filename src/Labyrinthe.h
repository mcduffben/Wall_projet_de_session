#pragma once

#include "ofMain.h"
#include <componenet/LineDTO.h>

class Labyrinthe
{

public:

	int echelle, epaisseur, hauteur;
	int newLineX, newLineY, posEntreeX,posEntreeY,posSortieX,posSortieY, jeu_arbitraire;
	int hauteurMur3d;

	vector<LineDTO> murs2Dbasique;
	vector<of3dPrimitive> obstacles;
	ofColor background;
	ofVec3f centre3d;

	void setup();
	void update(ofColor color, ofColor back,float epais,ofColor dessin);
	void draw(ofColor color, ofColor back, float epais, ofColor dessin);
	void draw3d(ofColor color, ofColor back, float epais, ofColor dessin);
	void drawWall();
	void update3d(ofColor color, ofColor back, float epais, ofColor dessin);

	void setNewLineX(int x);
	void setNewLineY(int y);
	void addNewLine(int x, int y);
	void addNewLine(ofVec2f pi, ofVec2f pf, bool select);
	void unselect_all();
	bool selectCheckerSingle(bool single,int x, int y);

	ofVec2f po;
	vector<LineDTO> mathPreview;
	//LineDTO preview;
	void drawPreview(int x,int y);

	void importimg(ofImage img);
};

