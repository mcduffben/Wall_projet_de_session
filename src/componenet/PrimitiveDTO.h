#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "CercleDTO.h"
#include "Cylindre.h"
#include "ModelDTO.h"

class PrimitiveDTO
{
public:

	struct FormeVecDTO
	{
		ofColor couleur;

		void draw() {}
	};

	vector<vector<CercleDTO>> cerclesManager;
	vector<vector<CylindreDTO>> cylindresManager;
	vector<vector<FormeVecDTO>> formesManager;
	vector<vector<ModelDTO>> modelManager;

	int navCe,navCy,navF, navM;
	int navigator; //1=cercle,2=cylindre,3=modele
	vector<int> historique;

	void setup();
	void draw();
	void draw3d();

	void undo();
	void redo();

	void undoCe();
	void undoCy();
	void undoF();
	void undoM();
	void redoCe();
	void redoCy();
	void redoF();
	void redoM();
	void saveCe(int r, ofVec3f p, ofColor c);
	void saveCy(int r, int h, ofVec3f p, ofColor c);
	void saveF();
	void saveM(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo);

	void cleanIrrelevantHistorique();
};
