#pragma once

#include "ofMain.h"
#include "LineDTO.h"
#include "ofxGui.h"

// énumération des types de kernel de convolution
enum class ConvolutionKernel
{
	identity,
	emboss,
	sharpen,
	edge_detect,
	blur
};

// kernel de convolution (3x3) : identité
const std::array<float, 9> convolution_kernel_identity
{
  0.0f,  0.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  0.0f,  0.0f
};

// kernel de convolution (3x3) : aiguiser
const std::array<float, 9> convolution_kernel_sharpen
{
  0.0f, -1.0f,  0.0f,
 -1.0f,  5.0f, -1.0f,
  0.0f, -1.0f,  0.0f
};

// kernel de convolution (3x3) : détection de bordure
const std::array<float, 9> convolution_kernel_edge_detect
{
  0.0f,  1.0f,  0.0f,
  1.0f, -4.0f,  1.0f,
  0.0f,  1.0f,  0.0f
};

// kernel de convolution (3x3) : bosseler
const std::array<float, 9> convolution_kernel_emboss
{
 -2.0f, -1.0f,  0.0f,
 -1.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  2.0f
};

// kernel de convolution (3x3) : flou
const std::array<float, 9> convolution_kernel_blur
{
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f,
  1.0f / 9.0f,  1.0f / 9.0f,  1.0f / 9.0f
};

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


	//bezier
	ofPolyline courbe;
	int line_resolution;
	float radius;
	void drawBezier(vector<ofVec2f> vec);
	ofVec3f position;
	vector<ofPolyline> allcurvs;
	vector<ofVec2f> sphereBezier;
	void addsphereBezier(ofVec2f point);
	//fin bezier

	ofVec2f po;
	vector<LineDTO> mathPreview;
	//LineDTO preview;
	void drawPreview(int x,int y);

	void importimg(ofImage img);



	//import des shader
	//declaration des menus
	ofxGuiGroup group_tone_mapping;
	ofParameter<float> slider_exposure;
	ofParameter<float> slider_gamma;
	ofParameter<bool> toggle_tone_mapping;
	float tone_mapping_exposure;
	float tone_mapping_gamma;
	bool tone_mapping_toggle;
	ofShader shader;

	ofxGuiGroup filtrage;
	ofParameter<bool> linear;
	ofParameter<bool> nearest;
	ConvolutionKernel kernel_type;
	string kernel_name;
	int image_width;
	int image_height;
	ofImage image_destination;
	ofImage image1;
	ofImage image_source;
	void filter();
	void keyReleased(int key);
	ofImage image;
	ofTexture mTex;
	ofTexture mTex1;
	ofImage image_mapping;

	ofxGuiGroup mapping;
	ofParameter<bool> normal_mapping;
	ofParameter<bool> displa_mapping;
	ofMesh mesh;
	ofMesh mesh1;
	ofEasyCam cam;
	ofPlanePrimitive plane;
	void displacement_map();

	ofShader normalMapShader;
	
};

