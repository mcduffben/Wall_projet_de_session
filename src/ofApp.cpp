#include "ofApp.h"
#include "renderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	labyrinthe.setup();
	renderer.setup();

	//Setup du UI
	setupUi();

	//Menu : 0 = menu principal, 1 = jeu, 2 = conception, 3 = options, 
	// 4 = mur basique (Conception), 5 = mur basique par param, 6 = mur basique dessin, 7 = édition 2D
	menu = 0;
	//La liste des curseurs pour chaque menu
	listeCurseurs = { 0, 1, 2, 3, 4, 4, 4, 5 };

	//Setup de variables
	freeDraw,wantsToSelect,hasSelectedSmthing = false;
	horizontal = true;
}

void ofApp::setupUi() {
	//Setup du UI du menu principal
	guiPrincipal.setup("Menu Principal");
	guiPrincipal.add(boutonJeu.setup("Jouer"));
	boutonJeu.addListener(this, &ofApp::button_pressed_jeu);
	guiPrincipal.add(boutonConception.setup("Conception"));
	boutonConception.addListener(this, &ofApp::button_pressed_conception);
	guiPrincipal.add(boutonOptions.setup("Options"));
	boutonOptions.addListener(this, &ofApp::button_pressed_options);

	//Setup du UI Jeu
	guiJeu.setup("Jouer");
	guiJeu.add(boutonExitJeu.setup("Retour"));
	boutonExitJeu.addListener(this, &ofApp::button_pressed_exit);

	//Setup du UI Conception
	guiConception.setup("Conception");

	guiConception.add(draw2dWall.setup("Dessiner Mur Basique"));
	draw2dWall.addListener(this, &ofApp::button_pressed_draw2dwall);
	guiConception.add(edition2d.setup("Editer Murs"));
	edition2d.addListener(this, &ofApp::button_pressed_edition2d);
	guiConception.add(boutonExitConception.setup("Retour"));
	boutonExitConception.addListener(this, &ofApp::button_pressed_exit);

	//Setup du UI Options
	guiOptions.setup("Options");

	boutonExitOptions.setup("Retour");
	boutonExitOptions.addListener(this, &ofApp::button_pressed_exit);
	guiOptions.add(&boutonExitOptions);

	//Setup du UI Conception Mur basique
	guiConceptionMurBasique.setup("Conception de murs basiques");
	guiConceptionMurBasique.setSize(300, 100);
	guiConceptionMurBasique.add(boutonByParameters.setup("Creation Par Coordonnees"));
	guiConceptionMurBasique.add(boutonDessinLibre.setup("Creation Par Dessin Libre"));
	guiConceptionMurBasique.add(boutonRetourConceptionMur.setup("Retour"));
	boutonByParameters.addListener(this, &ofApp::button_pressed_drawByParameters);
	boutonDessinLibre.addListener(this, &ofApp::button_pressed_freeDraw);
	boutonRetourConceptionMur.addListener(this, &ofApp::button_pressed_retourConception);

	//Setup du UI Conception Mur basique par paramètre
	guiCreationMurByParameters.setup("Conception de murs par parametres");
	guiCreationMurByParameters.add(newLine.setup("Ajouter un nouveau mur"));
	guiCreationMurByParameters.add(posLine.setup("Deplacer", { ofGetWidth() * .5, ofGetHeight() * .5 }, { 0, 0 }, { ofGetWidth(), ofGetHeight() }));
	guiCreationMurByParameters.add(lengthLine.setup("Longueur", 100, 20, 1000));
	guiCreationMurByParameters.add(orientationLine.setup("Changer orientation"));
	guiCreationMurByParameters.add(saveNew2dWall.setup("Sauvegarder"));
	guiCreationMurByParameters.add(exitCreationByParameters.setup("Retour"));
	newLine.addListener(this, &ofApp::button_pressed_addNewLine2d);
	orientationLine.addListener(this, &ofApp::button_pressed_orientationLine);
	saveNew2dWall.addListener(this, &ofApp::button_pressed_saveNewLine2d);
	exitCreationByParameters.addListener(this, &ofApp::button_pressed_exitCreationByParameters);
	guiCreationMurByParameters.setSize(350, 100);
	xLength = lengthLine;
	yLength = 0;

	//Setup du UI Conception Mur Dessin Libre
	guiCreationDessinLibre.setup("Conception de murs par dessin libre");
	guiCreationDessinLibre.add(doneFreeDraw.setup("Done"));
	doneFreeDraw.addListener(this, &ofApp::button_pressed_doneFreeDraw);
	guiCreationDessinLibre.setSize(350, 100);

	//Setup du UI Edition en 2d
	guiEdition2d.setup("Fenetre d'edition 2d");
	guiEdition2d.add(selectElement.setup("Selectionner un element"));
	guiEdition2d.add(editEntreeSortieTerrain.setup("Fenetre d'edition de l'entree, de la sortie et du terrain"));
	guiEdition2d.add(retourEdition2d.setup("Retour"));
	selectElement.addListener(this, &ofApp::button_pressed_selectElement);
	editEntreeSortieTerrain.addListener(this, &ofApp::button_pressed_editEntreeSortieTerrain);
	retourEdition2d.addListener(this, &ofApp::button_pressed_retourConception);
	guiEdition2d.setSize(500, 100);
}

//--------------------------------------------------------------
void ofApp::update() {
	labyrinthe.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	renderer.draw();

	labyrinthe.draw();
	drawUi();


	//Ajout d'une nouvelle ligne par paramètres
	if (newLineNumber > 0) {
		if (horizontal) {
			xLength = lengthLine;
			yLength = 0;
		}
		else {
			xLength = 0;
			yLength = lengthLine;
		}
		ofSetColor(0, 0, 175);
		ofDrawLine({ posLine->x - xLength, posLine->y - yLength }, { posLine->x + xLength, posLine->y + yLength });
	}
	ofSetColor(255, 255, 255);
	//fin de la fonction


	//Le curseur est dessiné à la fin pour qu'il soit devant le UI
	renderer.drawCursor(listeCurseurs[menu]);
}

//Le UI est dessiné, selon le menu désiré
void ofApp::drawUi() {
	if (menu == 0) {
		guiPrincipal.draw();
	}
	else if (menu == 1) {
		guiJeu.draw();
	}
	else if (menu == 2) {
		guiConception.draw();
	}
	else if (menu == 3) {
		guiOptions.draw();
	}
	else if (menu == 4) {
		guiConceptionMurBasique.draw();
	}
	else if (menu == 5) {
		guiCreationMurByParameters.draw();
	}
	else if (menu == 6) {
		guiCreationDessinLibre.draw();
	}
	else if (menu == 7) {
		guiEdition2d.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (freeDraw) {
		labyrinthe.setNewLineX(x);
		labyrinthe.setNewLineY(y);
	}
	if (wantsToSelect) {
		labyrinthe.selectCheckerSingle(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (freeDraw) {
		labyrinthe.addNewLine(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//Bouton Jouer
void ofApp::button_pressed_jeu()
{
	menu = 1;
}

//Bouton Conception
void ofApp::button_pressed_conception()
{
	menu = 2;
}

//Bouton Options
void ofApp::button_pressed_options()
{
	menu = 3;
}

//Bouton Exit
void ofApp::button_pressed_exit()
{
	menu = 0;
}

void ofApp::button_pressed_draw2dwall() {
	menu = 4;
}

void ofApp::button_pressed_drawByParameters() {
	menu = 5;
}

void ofApp::button_pressed_freeDraw() {
	menu = 6;
	freeDraw = true;
}

void ofApp::button_pressed_retourConception() {
	menu = 2;
}

void ofApp::button_pressed_edition2d() {
	menu = 7;
}

void ofApp::button_pressed_saveNewLine2d() {
	if (newLineNumber = 1) {
		newLineNumber = 0;
		labyrinthe.addNewLine({ posLine->x - xLength, posLine->y - yLength }, { posLine->x + xLength, posLine->y + yLength }, false);
	}
}

void ofApp::button_pressed_addNewLine2d() {
	if (newLineNumber == 0)newLineNumber = 1;
}

void ofApp::button_pressed_exitCreationByParameters() {
	menu = 4;
	newLineNumber = 0;
}

void ofApp::button_pressed_orientationLine() {
	horizontal = !horizontal;
}

void ofApp::button_pressed_doneFreeDraw() {
	menu = 4;
	freeDraw = false;
}


void ofApp::button_pressed_selectElement() {
	wantsToSelect = true;
}
void ofApp::button_pressed_editEntreeSortieTerrain() {

}