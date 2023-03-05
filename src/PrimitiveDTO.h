#pragma once
#include "ofMain.h"

class PrimitiveDTO
{
public:
	struct CercleDTO
	{
		int rayon;
		ofVec2f position;
		ofColor couleur;

		CercleDTO(int& r, ofVec2f p, ofColor c)
			:rayon(r), position(p), couleur(c) {}
	};

	struct CylindreDTO
	{
		int height, rayon;
		ofVec2f position;
		ofColor couleur;

		CylindreDTO(int& r, int& h, ofVec2f& p, ofColor c)
			:height(h), rayon(r), position(p), couleur(c) {}
	};

	struct FormeVecDTO
	{
		ofColor couleur;

		void draw() {}
	};

	vector<vector<CercleDTO>> cerclesManager;
	vector<vector<CylindreDTO>> cylindresManager;
	vector<vector<FormeVecDTO>> formesManager;

	int navCe,navCy,navF;

	void setup();
	void draw();
	void draw3d();

	void undoCe();
	void undoCy();
	void undoF();
	void redoCe();
	void redoCy();
	void RedoF();
	void saveCe();
	void saveCy();
	void saveF();
};

