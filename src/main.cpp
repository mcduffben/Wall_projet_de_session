
#include "ofMain.h"
#include "ofApp.h"


//========================================================================
int main( ){
	
	
	ofGLFWWindowSettings windowSettings;
	windowSettings.resizable = true;
	windowSettings.setGLVersion(3, 2);
	windowSettings.numSamples = 4;
	ofCreateWindow(windowSettings);
	
	ofRunApp(new ofApp());
	
}
