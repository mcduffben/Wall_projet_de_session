#include "ofApp.h"
#include "renderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	renderer.setup();

	//Setup du UI
	setupUi();

	//Nom du menu
	menu = "principal";
}

void ofApp::setupUi() {
	//Setup du UI du menu principal
	guiPrincipal.setup("Menu Principal");
	
	boutonJeu.setup("Jouer");
	boutonJeu.addListener(this, &ofApp::button_pressed_jeu);
	guiPrincipal.add(&boutonJeu);

	boutonConception.setup("Conception");
	boutonConception.addListener(this, &ofApp::button_pressed_conception);
	guiPrincipal.add(&boutonConception);
	
	boutonOptions.setup("Options");
	boutonOptions.addListener(this, &ofApp::button_pressed_options);
	guiPrincipal.add(&boutonOptions);

	//Setup du UI Jeu
	guiJeu.setup("Jouer");

	boutonExitJeu.setup("Retour");
	boutonExitJeu.addListener(this, &ofApp::button_pressed_exit);
	guiJeu.add(&boutonExitJeu);

	//Setup du UI Conception
	guiConception.setup("Conception");

	boutonExitConception.setup("Retour");
	boutonExitConception.addListener(this, &ofApp::button_pressed_exit);
	guiConception.add(&boutonExitConception);
	//add des option UI
	guiConception.add(murSize.setup("size", 3, 1, 8));
	guiConception.add(murColor.setup("color", ofColor(255, 255, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	guiConception.add(solBackground.setup("background", ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));
	guiConception.add(btnClear.setup("clear"));

	//Setup du UI Options
	guiOptions.setup("Options");

	boutonExitOptions.setup("Retour");
	boutonExitOptions.addListener(this, &ofApp::button_pressed_exit);
	guiOptions.add(&boutonExitOptions);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	renderer.draw();
	drawUi();
	renderer.reset();
	//Le curseur est dessin・・la fin pour qu'il soit devant le UI
	renderer.drawCursor();
}


//Le UI est dessin・ selon le menu d駸ir・
void ofApp::drawUi() {
	if (menu == "principal") {
		guiPrincipal.draw();
	}
	else if (menu == "jeu") {
		guiJeu.draw();
	}
	else if (menu == "conception") {
		guiConception.draw();
	}
	else if (menu == "options") {
		guiOptions.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	renderer.drawApercu(x, y, murSize);
	
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	renderer.firstP = ofPoint(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//Bouton Jouer
void ofApp::button_pressed_jeu() 
{
	menu = "jeu";
}

//Bouton Conception
void ofApp::button_pressed_conception()
{
	menu = "conception";
}

//Bouton Options
void ofApp::button_pressed_options()
{
	menu = "options";
}

//Bouton Exit
void ofApp::button_pressed_exit()
{
	menu = "principal";
}
