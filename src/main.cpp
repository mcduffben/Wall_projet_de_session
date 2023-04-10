#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

	//COMMENTAIRE ESSAI

}
//this break color why
// todo ?
//bool use_gl_fixed_function_pipeline = false;
////========================================================================
//int main() {
//    ofGLWindowSettings windowSettings;
//    windowSettings.setSize(1024, 512);
//
//    if (use_gl_fixed_function_pipeline)
//        windowSettings.setGLVersion(2, 1);
//    else
//        windowSettings.setGLVersion(3, 3);
//
//    ofCreateWindow(windowSettings);
//    ofRunApp(new ofApp());
//
//    //COMMENTAIRE ESSAI
//
//}