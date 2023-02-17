#pragma once

#include "ofMain.h"
#include "renderer.h"

class ofApp : public ofBaseApp{

	public:
		
		Renderer renderer;

		string menu;

		ofxPanel guiPrincipal;
		ofxButton boutonJeu;
		ofxButton boutonConception;
		ofxButton boutonOptions;

		ofxPanel guiJeu;
		ofxButton boutonExitJeu;

		ofxPanel guiConception;
		ofxButton boutonExitConception;

		ofxPanel guiOptions;
		ofxButton boutonExitOptions;

		ofxIntSlider murSize;
		ofxColorSlider murColor;
		ofxVec3Slider solBackground;
		ofxButton btnClear;

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
		
};
