#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "Labyrinthe.h"
#include "PrimitiveDTO.h"
#include "ofxAssimpModelLoader.h"
#include <particleSystem.h>
#include <array>

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

class ofApp : public ofBaseApp{

	public:
		

		Renderer renderer;
		Labyrinthe labyrinthe;
		PrimitiveDTO prime;
		LineDTO line;
		ofxAssimpModelLoader player;

		int menu;
		int vue;
		vector<int> listeCurseurs;
		ofEasyCam cam;
		ofxAssimpModelLoader transitoryModel;
		
		bool freeDraw, horizontal, wantsToSelect, hasSelectedSmthing, wantsToSelectMultiple, hasSelectedThings, particleEffect;
		bool modifyingLines, modifyingOneLine, drawSphere,drawCyl,drawMod, wantsimport;
		int newLineNumber, xLength, yLength;
		int oldfloatsliderx,oldfloatslidery,timeDeFrame;

		ofxPanel guiPrincipal;
		ofxButton boutonJeu, boutonConception,boutonOptions;

		ofxPanel guiJeu;
		ofxButton boutonExitJeu, boutonCreationObstacle, boutonexport;

		ofxPanel guiConception;
		ofxButton boutonExitConception, draw2dWall, edition2d;

		ofxPanel guiOptions;
		ofxButton boutonExitOptions;

		ofxPanel guiConceptionMurBasique;
		ofxButton boutonByParameters, boutonDessinLibre, boutonRetourConceptionMur;

		ofxPanel guiCreationMurByParameters;
		ofxButton newLine,saveNew2dWall, exitCreationByParameters, orientationLine;
		ofxVec2Slider posLine;
		ofxFloatSlider lengthLine;

		ofxPanel guiCreationDessinLibre;
		ofxButton doneFreeDraw;

		ofxPanel guiEdition2d;
		ofxButton selectElement,selectMultipleElements, editEntreeSortieTerrain, retourEdition2d,modifier1element,modifierplusieurs;

		ofxPanel guiObstacle;
		ofxButton ajouterSphere, ajouterModele, ajouterCylindre, sauvegarderPrime3d, retourajouer, undoprime, redoprime;;
		ofxFloatSlider absObstacle, ordObstacle, zObstacle, radObstacle, heightCylinder,scaleModel, degXmodel,degYmodel;
		ofxColorSlider colObstacle;

		ofxPanel guiEditionLigne;
		ofxFloatSlider posxline, posyline;
		ofxButton retour_a_edition2d, voirMur;

		ofxPanel guiEditionplusieurslignes;
		ofxFloatSlider xlines, ylines;
		ofxButton retour_a_edition2dbis;

		ofxPanel affichageMur;
		ofxButton retouraeditionligne, importerimg;

		//upload Image

		ofxButton button;
		ofxButton button1;
		ofxButton button2;
		ofxButton button3;
		ofxButton button4;
		ofxButton button5;
		ofxButton button6;
		ofxButton button7;
		void image_export(const string name, const string extension) const;
		void button_pressed();

		//couleur
		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofParameter<ofColor> color_dessin;

		ofColor background_color;
		float stroke_weight;
		ofColor stroke_color;
		ofParameter<float> slider_stroke_weight;
		ofxPanel gui;
		ofxGuiGroup group_draw;

		ofxButton button_contour;
		bool actif_button = false;
		void button_pressed_2();
		void draw_app();
		void update_app();
//fin

		void setup();
		void setupUi();
		void update();
		void draw();

		void drawUi();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void button_pressed_jeu();
		void button_pressed_conception();
		void button_pressed_options();
		void button_pressed_exit();
		void button_pressed_draw2dwall();
		void button_pressed_drawByParameters();
		void button_pressed_freeDraw();
		void button_pressed_retourConception();
		void button_pressed_edition2d();
		void button_pressed_saveNewLine2d();
		void button_pressed_addNewLine2d();
		void button_pressed_exitCreationByParameters();
		void button_pressed_orientationLine();
		void button_pressed_doneFreeDraw();
		void button_pressed_selectElement();
		void button_pressed_selectMultipleElement();
		void button_pressed_editEntreeSortieTerrain();
		void button_pressed_ajouterobstacle();
		void button_pressed_ajouterSphere();
		void button_pressed_ajouterCylindre();
		void button_pressed_ajouterModele();
		void button_pressed_saveObj3d();
		void buttonretourajouer();
		void button_pressed_retour_a_edition2d();
		void button_pressed_modifier1ligne();
		void button_pressed_modifierplusieurs();
		void button_pressed_undoprime();
		void button_pressed_redoprime();
		void afficherMur();
		void buttonretouraedition();
		void buttonimportimg();
		void exportimg();

		//camera
		//ofEasyCam cam;
		ofEasyCam camera;
		ofEasyCam camera3;
		int x_index, y_index = 0;
		int z_index = 1000;
		ofxGuiGroup group_draw_camera;
		ofxButton gauche;
		ofxButton droite;
		ofxButton bas;
		ofxButton haut;
		ofxButton zoomer;
		ofxButton dezoomer;
		void button_cam_gauche();
		void button_cam_droite();
		void button_cam_haut();
		void button_cam_bas();
		void button_cam_zoomer();
		void button_cam_dezoomer();

		

		void drawVector(ofPoint, ofPoint, float);


		particleSystem* ps;
		ofImage img;
//tone mapping
		ofImage image;
		ofFbo fbo;
		ofShader shader;

		int offset_vertical;
		int offset_horizontal;

		
		ofxGuiGroup group_tone_mapping;
		ofParameter<float> slider_exposure;
		ofParameter<float> slider_gamma;
		ofParameter<bool> toggle_tone_mapping;

		ofxGuiGroup filtrage;
		ofxButton identique;
		void button_identique();
		ofxButton blur;
		void button_blur();
		ofxButton edge_detect;
		void button_edge_detect();
		ofxButton sharpen;
		void button_sharpen();
		
		ofxButton emboss;
		void button_emboss();

		ofxButton Linear;
		void button_Linear();

		ofxButton Nearest;
		void button_Nearest();

		float tone_mapping_exposure;
		float tone_mapping_gamma;
		bool tone_mapping_toggle;

		ofParameter<bool> linear;
		ofParameter<bool> nearest;
		ofParameter<bool> sphere;
		

		bool blur_var=false;
		bool edge_detect_var = false;
		bool sharpen_var = false;
		bool emboss_var = false;
		bool Linear_var = false;
		bool Nearest_var = false;
		bool identique_var=true;
		ofxGuiGroup illimunation;
		ofParameter<bool> activer;
		bool illum_active = true;
		

//convulsion
		ConvolutionKernel kernel_type;

		string kernel_name;
		int image_width;
		int image_height;
		ofImage image_destination;
		ofImage image1;
		ofImage image_source;
		void filter();
		bool tone = true;
		ofTexture mTex;
		ofTexture mTex1;
		ofTrueTypeFont myfont;
		int screen_h = 0;
		int screen_w = 0;
		//ofEasyCam cam;
		ofPlanePrimitive plane;

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
};
