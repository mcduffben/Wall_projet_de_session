#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	ofGLFWWindowSettings windowSettings;
	windowSettings.resizable = true;
	windowSettings.setGLVersion(3, 2);
	windowSettings.numSamples = 4;
	ofCreateWindow(windowSettings);
	ofRunApp(new ofApp());
	//COMMENTAIRE ESSAI

}
