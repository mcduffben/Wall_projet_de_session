#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "CercleDTO.h"
#include "Cylindre.h"
#include "ModelDTO.h"
#include "ofxGui.h"


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

	//lightning related

	ofColor light_ambient;

	ofLight light_directional;
	ofLight light_point;
	ofLight light_spot;

	ofMaterial material1;
	ofMaterial material2;
	ofMaterial material3;

	ofQuaternion orientation_directional;
	ofQuaternion orientation_spot;

	float oscillation_light;
	float oscillation_frequency_light;
	float oscillation_amplitude_light;
	int materialChooser;

	bool renderSpot, renderPoint, renderDir, renderAmbient;
	float hautDir, absDir, ordDir;

	//end lightning

	//declarer tout ce qui est en lien avec illumination
	ofxGuiGroup illimunation;
	ofParameter<bool> gouraud;
	ofParameter<bool> phong;
	ofParameter<bool> blinn_phong;
	bool illum_active = true;

	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinn_phong;

	ofVec3f position_gouraud;
	ofVec3f position_phong;
	ofVec3f position_blinn_phong;

	float oscillation;

	float scale_gouraud;
	float scale_phong;
	float scale_blinn_phong;

	float oscillation_frequency;
	float oscillation_amplitude;

	float speed_motion;

	float offset_x;
	float offset_z;

	float delta_x;
	float delta_z;

	float initial_x;
	float initial_z;

	float center_x;
	float center_y;
	float oscillate(float time, float frequency, float amplitude);
	void reset();
	ofLight light;
	ofSpherePrimitive sphere;
	//TEXTURE PROCEDURALE
	ofParameter<bool> generer_texture = false;
	ofTexture texture;
	

	void setupLight();
	void update3d();
	float oscillate_light(float time, float frequency, float amplitude);
};
