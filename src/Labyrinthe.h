#pragma once

#include "ofMain.h"
#include "LineDTO.h"

class Labyrinthe
{

public:

	int echelle, epaisseur, hauteur;
	int newLineX, newLineY, posEntreeX,posEntreeY,posSortieX,posSortieY, jeu_arbitraire;

	vector<LineDTO> murs2Dbasique;

	void setup();
	void update();
	void draw();

	void setNewLineX(int x);
	void setNewLineY(int y);
	void addNewLine(int x, int y);
	void addNewLine(ofVec2f pi, ofVec2f pf, bool select);
	void unselect_all();
	bool selectCheckerSingle(int x, int y);
};

