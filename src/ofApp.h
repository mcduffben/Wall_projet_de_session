#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "Labyrinthe.h"

class ofApp : public ofBaseApp{

	public:
		
		Renderer renderer;
		Labyrinthe labyrinthe;

		int menu;
		int vue;
		vector<int> listeCurseurs;
		
		bool freeDraw, horizontal, wantsToSelect, hasSelectedSmthing;
		int newLineNumber, xLength, yLength;

		ofxPanel guiPrincipal;
		ofxButton boutonJeu, boutonConception,boutonOptions;

		ofxPanel guiJeu;
		ofxButton boutonExitJeu, boutonCreationObstacle;

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
		ofxButton selectElement, editEntreeSortieTerrain, retourEdition2d,modifier1element,modifierplusieurs;

		ofxPanel guiObstacle;
		ofxButton ajouterSphere, ajouterModele, ajouterCylindre, retourajouer;

		ofxPanel guiEditionLigne;
		ofxFloatSlider xline, yline;
		ofxButton retour_a_edition2d, voirMur;

		ofxPanel guiEditionplusieurslignes;
		ofxFloatSlider xline, yline;

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
		void button_pressed_editEntreeSortieTerrain();
		void button_pressed_ajouterSphere();
		void button_pressed_ajouterCylindre();
		void button_pressed_ajouterModele();
		void button_pressed_retour_a_edition2d();
		void button_pressed_modifier1ligne();
		void button_pressed_modifierplusieurs();
};
