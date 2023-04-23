#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "Labyrinthe.h"
#include "PrimitiveDTO.h"
#include "ofxAssimpModelLoader.h"
#include <array>
#include "TesselationRender.h"
#include "pbr.h"

class ofApp : public ofBaseApp{

	public:
		
		ofFbo fboTess;
		ofxPanel guiTesslation;
		ofParameterGroup parameters;
		void setupTesselation();
		void drawTess();
		TesselationRender tessRender;

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
		bool renderlabyrinthe3d;
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
		ofxButton boutonByParameters, boutonDessinLibre,boutonBezier, boutonRetourConceptionMur;

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
		ofxFloatSlider absObstacle, ordObstacle, zObstacle, radObstacle, heightCylinder,scaleModel, degXmodel,degYmodel, hautDir,absDir,ordDir;
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


		void creationparBezier();
		bool creationBezier = false;
		vector<ofVec3f> bezier;

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


		
		ofImage img;
		//tone mapping
		ofImage image;
		ofFbo fbo;
		

		int offset_vertical;
		int offset_horizontal;

		ofTrueTypeFont myfont;
		int screen_h = 0;
		int screen_w = 0;
		
		ofEasyCam camTemporaire;
		ofImage image_mapping;

		//pbr
		Pbr pbr;
		ofxGuiGroup gui_material;
		ofxGuiGroup group_material_color;
		ofxGuiGroup group_material_factor;
		ofxGuiGroup group_light;
		ofxGuiGroup group_tone_mapping;

		ofParameter<ofColor> color_picker_ambient;
		ofParameter<ofColor> color_picker_diffuse;
		ofParameter<ofColor> color_picker_specular;

		ofParameter<float> slider_metallic;
		ofParameter<float> slider_roughness;
		ofParameter<float> slider_occlusion;
		ofParameter<float> slider_brightness;

		ofParameter<glm::vec3> slider_fresnel_ior;

		ofParameter<ofColor> color_picker_light_color;

		ofParameter<float> slider_light_intensity;

		ofParameter<float> slider_exposure;
		ofParameter<float> slider_gamma;
		ofParameter<bool> toggle_tone_mapping;

		ofParameter<bool> pbr_active=false;
		
		
		ofParameter<bool> toggle_light_motion;

		ofxButton button_reset;
		
		float time_current;
		float time_last;
		float time_elapsed;

		bool is_key_press_up;
		bool is_key_press_down;
		bool is_key_press_left;
		bool is_key_press_right;

		bool is_key_press_w;
		bool is_key_press_a;
		bool is_key_press_s;
		bool is_key_press_d;
		bool is_key_press_q;
		bool is_key_press_e;

		void reset();
		void menu_pbr();

		//REFLECTION ET REFRACTION

		ofParameter<bool> lancer_rayon = false;
		
		ofLight areaLight;
		ofPlanePrimitive plane;
		//ofEasyCam camera;
		ofMaterial materialPlane;
		ofMesh logoMesh;
		ofMaterial materialMesh;

		int mode = 0;
		float orbitSpeed = 0.0;
		float highwaySpeed = 0.;

		float bump = 0.;
		float bumpHeight = 50.0;

		ofMaterial boxMaterial;
		ofMaterial bgMaterial;
		ofMaterial sphereMaterial;

		ofColor light_ambient;

		ofLight light_directional;
		ofLight light_point;
		ofLight light_spot;

		ofBoxPrimitive box;

};
