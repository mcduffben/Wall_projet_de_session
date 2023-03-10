#include "ofApp.h"
#include "renderer.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//cam.setAutoDistance(2000);
	//camera.setAutoDistance(2000);
	//camera3.setAutoDistance(2000);

	labyrinthe.setup();
	renderer.setup();
	prime.setup();
	ModelDTO ptest ( );
	img.load("texture.png");
	ps = new particleSystem(ofPoint(ofGetWidth() / 2, ofGetHeight() - 75), img);
	player.loadModel("testA.fbx");
	player.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
	
	player.setScale(0.2f, 0.2f, 0.2f);
	ofSetCircleResolution(60);
	ofSetCylinderResolution(60,60);
	player.setPosition(0, 0 - ofGetHeight() / 2, 5);

	//Menu : 0 = menu principal, 1 = jeu, 2 = conception, 3 = options, 
	// 4 = mur basique (Conception), 5 = mur basique par param, 6 = mur basique dessin, 7 = édition 2D, 8 = edition ligne 2d
	menu = 0;
	//View : 1 = vue d'un seul mur, 2 = vue de dessus en 2d, 3 = vue en 3d
	vue = 2;
	//La liste des curseurs pour chaque menu
	listeCurseurs = { 0, 1, 2, 3, 4, 4, 4, 5,0,0,0,0,0 };
	//Setup de variables
	freeDraw, wantsToSelect, hasSelectedSmthing, wantsToSelectMultiple, hasSelectedThings = false;
	modifyingLines, modifyingOneLine, drawSphere, drawCyl, drawMod, wantsimport = false;
	horizontal = true;
	oldfloatsliderx = 0;
	oldfloatslidery = 0;

	
	cam.setPosition(x_index, y_index, z_index);
	//camera.setPosition(ofGetWidth() / 2.0, ofGetHeight() / 2.0, 1000);
	//camera3.setPosition(ofGetWidth() / 4.0, ofGetHeight() / 2.0, 2000);

	//cam.setPosition(0, 0, 500);
	player.stopAllAnimations();
	player.resetAllAnimations();
	player.setPositionForAllAnimations(0);
	player.setRotation(1,1,90,90,0);
	//Setup du UI
	setupUi();
}

void ofApp::setupUi() {

	//Background
	color_picker_background.set("couleur de fond", ofColor(31), ofColor(0, 0), ofColor(255, 255));
	//Couleur de ligne de contour ou remplissage
	color_picker_stroke.set("Remplissage ou contour", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	//Largeur de la ligne de contour 
	slider_stroke_weight.set("largeur de la ligne", 4.0f, 0.0f, 15.0f);
	color_dessin.set("Couleur de dessin", ofColor(10), ofColor(0, 0), ofColor(255, 255));


	//Setup du UI du menu principal
	guiPrincipal.setup("Menu Principal");
	guiPrincipal.add(boutonJeu.setup("Jouer"));
	boutonJeu.addListener(this, &ofApp::button_pressed_jeu);
	guiPrincipal.add(boutonConception.setup("Conception"));
	boutonConception.addListener(this, &ofApp::button_pressed_conception);
	guiPrincipal.add(boutonOptions.setup("Options"));
	boutonOptions.addListener(this, &ofApp::button_pressed_options);
	guiPrincipal.add(button.setup("Upload"));
	button.addListener(this, &ofApp::button_pressed);
	


	group_draw.add(color_picker_background);
	group_draw.add(color_picker_stroke);
	group_draw.add(slider_stroke_weight);
	group_draw.add(color_dessin);
	guiPrincipal.add(&group_draw);

	//Setup du UI Jeu
	guiJeu.setup("Jouer");
	guiJeu.add(boutonCreationObstacle.setup("Ajouter des obstacles"));
	guiJeu.add(boutonexport.setup("Exporter Labyrinthe en image"));
	guiJeu.add(button1.setup("Upload"));
	guiJeu.add(&group_draw);
	button1.addListener(this, &ofApp::button_pressed);
	guiJeu.add(boutonExitJeu.setup("Retour"));
	boutonexport.addListener(this, &ofApp::exportimg);
	boutonCreationObstacle.addListener(this, &ofApp::button_pressed_ajouterobstacle);
	boutonExitJeu.addListener(this, &ofApp::button_pressed_exit);

	//Setup du UI Conception
	guiConception.setup("Conception");
	guiConception.add(draw2dWall.setup("Dessiner Mur Basique"));
	draw2dWall.addListener(this, &ofApp::button_pressed_draw2dwall);
	guiConception.add(edition2d.setup("Editer Murs"));
	guiConception.add(button2.setup("Upload"));
	button2.addListener(this, &ofApp::button_pressed);
	button_contour.setup("Activer Contour");
	button_contour.addListener(this, &ofApp::button_pressed_2);
	guiConception.add(&button_contour);
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
	guiConceptionMurBasique.add(button3.setup("Upload"));
	button3.addListener(this, &ofApp::button_pressed);
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
	guiCreationMurByParameters.add(button4.setup("Upload"));
	button4.addListener(this, &ofApp::button_pressed);
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
	guiCreationDessinLibre.add(button5.setup("Upload"));
	button5.addListener(this, &ofApp::button_pressed);
	doneFreeDraw.addListener(this, &ofApp::button_pressed_doneFreeDraw);
	guiCreationDessinLibre.setSize(350, 100);

	//Setup du UI Edition en 2d
	guiEdition2d.setup("Fenetre d'edition 2d");
	guiEdition2d.add(selectElement.setup("Selectionner un element"));
	guiEdition2d.add(selectMultipleElements.setup("Selectionner des elements"));
	guiEdition2d.add(editEntreeSortieTerrain.setup("Fenetre d'edition de l'entree, de la sortie et du terrain"));
	guiEdition2d.add(modifier1element.setup("Modifier le mur"));
	guiEdition2d.add(modifierplusieurs.setup("Modifier les murs"));
	guiEdition2d.add(button6.setup("Upload"));
	button6.addListener(this, &ofApp::button_pressed);
	guiEdition2d.add(retourEdition2d.setup("Retour"));
	selectElement.addListener(this, &ofApp::button_pressed_selectElement);
	selectMultipleElements.addListener(this, &ofApp::button_pressed_selectMultipleElement);
	editEntreeSortieTerrain.addListener(this, &ofApp::button_pressed_editEntreeSortieTerrain);
	modifier1element.addListener(this, &ofApp::button_pressed_modifier1ligne);
	modifierplusieurs.addListener(this, &ofApp::button_pressed_modifierplusieurs);
	retourEdition2d.addListener(this, &ofApp::button_pressed_retourConception);
	guiEdition2d.setSize(500, 100);

	//Setup du UI edition ligne 2d
	guiEditionLigne.setup("Edition d'une ligne");
	guiEditionLigne.add(posxline.setup("Position x", 800, 0, 2000));
	guiEditionLigne.add(posyline.setup("Position y", 800, 0, 2000));
	guiEditionLigne.add(voirMur.setup("Affichier Mur"));
	guiEditionLigne.add(button7.setup("Upload"));
	button7.addListener(this, &ofApp::button_pressed);
	guiEditionLigne.add(retour_a_edition2d.setup("Retour"));
	voirMur.addListener(this, &ofApp::afficherMur);
	retour_a_edition2d.addListener(this, &ofApp::button_pressed_retour_a_edition2d);

	//Setup du UI edition de plusieurs lignes
	guiEditionplusieurslignes.setup("Edition de plusieurs lignes");
	guiEditionplusieurslignes.add(xlines.setup("Positions x", 0, -50, 50));
	guiEditionplusieurslignes.add(ylines.setup("Positions y", 0, -50, 50));
	guiEditionplusieurslignes.add(retour_a_edition2dbis.setup("Retour"));
	retour_a_edition2dbis.addListener(this, &ofApp::button_pressed_retour_a_edition2d);

	//Setup du UI ajouterObstacle
	//ofxPanel guiObstacle;
	//ofxButton ajouterSphere, ajouterModele, ajouterCylindre, retourajouer;
	guiObstacle.setup("Ajouter des obstacles");
	guiObstacle.add(ajouterSphere.setup("Ajouter Sphere"));
	guiObstacle.add(ajouterCylindre.setup("Ajouter Cylindre"));
	guiObstacle.add(ajouterModele.setup("Ajouter Modele 3d"));
	guiObstacle.add(absObstacle.setup("Abscisse", x_index, -z_index, z_index));
	guiObstacle.add(ordObstacle.setup("Ordonnees", x_index, -z_index, z_index));
	guiObstacle.add(zObstacle.setup("Profondeur", x_index, -z_index, z_index));
	guiObstacle.add(radObstacle.setup("Rayon", 100, 50, 300));
	guiObstacle.add(heightCylinder.setup("Hauteur cylindre", 100, 50, 300));
	guiObstacle.add(scaleModel.setup("Scale du modele", 0.5, 0.1, 0.9));
	guiObstacle.add(degXmodel.setup("Orientation X du modele", 270, 0, 360));
	guiObstacle.add(degYmodel.setup("Orientation Y du modele", 270, 0, 360));
	guiObstacle.add(colObstacle.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
	guiObstacle.add(sauvegarderPrime3d.setup("Sauvegarder"));
	guiObstacle.add(gauche.setup("Gauche"));
	guiObstacle.add(droite.setup("Droite"));
	guiObstacle.add(haut.setup("Haut"));
	guiObstacle.add(bas.setup("Bas"));
	guiObstacle.add(zoomer.setup("Zoomer"));
	guiObstacle.add(dezoomer.setup("Dezoomer"));
	gauche.addListener(this, &ofApp::button_cam_gauche);
	droite.addListener(this, &ofApp::button_cam_droite);
	haut.addListener(this, &ofApp::button_cam_haut);
	bas.addListener(this, &ofApp::button_cam_bas);
	zoomer.addListener(this, &ofApp::button_cam_zoomer);
	dezoomer.addListener(this, &ofApp::button_cam_dezoomer);

	guiObstacle.add(undoprime.setup("Undo"));
	guiObstacle.add(redoprime.setup("Redo"));
	guiObstacle.add(retourajouer.setup("Retour"));
	ajouterSphere.addListener(this, &ofApp::button_pressed_ajouterSphere);
	ajouterCylindre.addListener(this, &ofApp::button_pressed_ajouterCylindre);
	ajouterModele.addListener(this, &ofApp::button_pressed_ajouterModele);
	sauvegarderPrime3d.addListener(this, &ofApp::button_pressed_saveObj3d);
	undoprime.addListener(this,& ofApp::button_pressed_undoprime);
	redoprime.addListener(this, &ofApp::button_pressed_redoprime);
	retourajouer.addListener(this, &ofApp::buttonretourajouer);

	//Setup UI afficher un mur
	affichageMur.setup("Affichage d'un mur");
	affichageMur.add(importerimg.setup("Importer une image"));
	affichageMur.add(retouraeditionligne.setup("Retour"));
	importerimg.addListener(this, &ofApp::buttonimportimg);
	retouraeditionligne.addListener(this, &ofApp::buttonretouraedition);
}

//--------------------------------------------------------------
void ofApp::update() {

	cam.setPosition(x_index, y_index, z_index);
	if(vue==2)labyrinthe.update(color_picker_stroke, background_color, slider_stroke_weight, color_dessin);
	if (vue == 3)labyrinthe.update3d(color_picker_stroke, background_color, slider_stroke_weight,color_dessin);


	if(vue==2)labyrinthe.update(color_picker_stroke, background_color, stroke_weight, color_dessin);
	if (vue == 3)labyrinthe.update3d(color_picker_stroke, background_color, stroke_weight, color_dessin);

	ofBackground(color_picker_background);
	if (timeDeFrame > 0)
	{
		ps->addParticle();
		ps->update();
	}
	//ofColor();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofNoFill();


	renderer.draw();
	//camera.begin();
	if (vue == 1)labyrinthe.drawWall();
	
	//camera3.begin();
	if(vue==2)labyrinthe.draw(color_picker_stroke, background_color, stroke_weight, color_dessin);
	
	//camera3.end();
	//camera.end();
	if (vue == 3) {
		cam.begin();
		labyrinthe.draw3d(color_picker_stroke, background_color,stroke_weight,color_dessin);
     	prime.draw3d();

		player.enableColors();
		ofSetColor(238, 75, 43);
		
		player.drawFaces();
		if (timeDeFrame > 0)
		{
			timeDeFrame--;
			ps->display();

		}
		//Ajout d'objet 3d
		if (drawSphere) {
			ofSetColor(colObstacle);
			ofDrawSphere(absObstacle, ordObstacle, zObstacle, radObstacle);
		}
		if (drawCyl) {
			ofSetColor(colObstacle);
			ofDrawCylinder(absObstacle, ordObstacle, zObstacle, radObstacle, heightCylinder);
		}
		if (drawMod) {
			ofPushMatrix();
			ofRotateXDeg(degXmodel);
			ofRotateYDeg(degYmodel);
			ofSetColor(colObstacle);
			transitoryModel.setScale(scaleModel, scaleModel, scaleModel);
			transitoryModel.drawFaces();
			ofPopMatrix();
		}
		//Fin objet 3d
		cam.end();
	}
	drawUi();


	//cam.begin();
	//Ajout d'une nouvelle ligne par param�tres

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

	//Edition de lignes
	if (modifyingOneLine) {
		for (int i = 0; i < labyrinthe.murs2Dbasique.size();i++) {
			if (labyrinthe.murs2Dbasique[i].selected) {
				labyrinthe.murs2Dbasique[i].pinit.x = posxline;
				labyrinthe.murs2Dbasique[i].pfinal.x = posxline+ labyrinthe.murs2Dbasique[i].diffx;
				labyrinthe.murs2Dbasique[i].pinit.y = posyline;
				labyrinthe.murs2Dbasique[i].pfinal.y = posyline + labyrinthe.murs2Dbasique[i].diffy;
			}
		}
	}
	if (modifyingLines && xlines!=oldfloatsliderx) {
		for (int i = 0; i < labyrinthe.murs2Dbasique.size(); i++) {
			if (labyrinthe.murs2Dbasique[i].selected) {
				labyrinthe.murs2Dbasique[i].pinit.x += xlines;
				labyrinthe.murs2Dbasique[i].pfinal.x += xlines;
			}
		}
	}
	if (modifyingLines && ylines != oldfloatslidery) {
		for (int i = 0; i < labyrinthe.murs2Dbasique.size(); i++) {
			if (labyrinthe.murs2Dbasique[i].selected) {
				labyrinthe.murs2Dbasique[i].pinit.y += ylines;
				labyrinthe.murs2Dbasique[i].pfinal.y += ylines;
			}
		}
	}
	oldfloatsliderx = xlines;
	oldfloatslidery = ylines;
	//Edition de lignes finie


	//Le curseur est dessin� � la fin pour qu'il soit devant le UI
	

	//Le curseur est dessiné à la fin pour qu'il soit devant le UI
	renderer.drawCursor(listeCurseurs[menu]);

	//ofBackground(stroke_color);

	//cam.end();
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
	else if (menu == 8) {
		guiEditionLigne.draw();
	}
	else if (menu == 9) {
		guiEditionplusieurslignes.draw();
	}
	else if (menu == 10) {
		guiObstacle.draw();
	}
	else if (menu == 11) {
		affichageMur.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key)
	{
		case OF_KEY_LEFT: // touche ←
			player.setPosition(player.getPosition().x-5, player.getPosition().y, player.getPosition().z);
			break;

		case OF_KEY_UP: // touche ↑
			player.setPosition(player.getPosition().x, player.getPosition().y + 5, player.getPosition().z);
			break;

		case OF_KEY_RIGHT: // touche →
			player.setPosition(player.getPosition().x+5, player.getPosition().y , player.getPosition().z);
			break;

		case OF_KEY_DOWN: // touche ↓
			player.setPosition(player.getPosition().x, player.getPosition().y-5 , player.getPosition().z);
			break;

		case 120: // touche x
	
			player.setAnimation(0);
			player.getAnimation(0).play();
			
			player.playAllAnimations();
			ps->origin = player.getPosition();
			timeDeFrame += 630;

			break;
		
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (freeDraw) {
		labyrinthe.drawPreview(x, y);
	}

	}
	

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (freeDraw) {
		labyrinthe.po.x = x;
		labyrinthe.po.y = y;
		labyrinthe.setNewLineX(x);
		labyrinthe.setNewLineY(y);
	}
	if (wantsToSelect) {
		bool l =labyrinthe.selectCheckerSingle(true,x, y);
		if (l)hasSelectedSmthing = true;
	}

	if (wantsToSelectMultiple) {
		bool l = labyrinthe.selectCheckerSingle(false,x, y);
		if (l)hasSelectedThings = true;
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (freeDraw) {
		labyrinthe.addNewLine(x, y);
	}
	labyrinthe.mathPreview.clear();
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
	if (drawMod) {
		transitoryModel.loadModel(dragInfo.files.at(0));
	}
	if (wantsimport) {
		ofImage im;
		im.load(dragInfo.files.at(0));
		labyrinthe.importimg(im);
		wantsimport = false;
	}
}

//Bouton Jouer
void ofApp::button_pressed_jeu()
{
	menu = 1;
	vue = 3;
}

//Bouton Conception
void ofApp::button_pressed_conception()
{
	menu = 2;
	vue = 2;
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
	vue = 2;
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
	labyrinthe.unselect_all();
	wantsToSelectMultiple = false;
	wantsToSelect = false;
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
	wantsToSelectMultiple = false;
	wantsToSelect = true;
}
void ofApp::button_pressed_selectMultipleElement() {
	wantsToSelectMultiple = true;
	wantsToSelect = false;
}
void ofApp::button_pressed_editEntreeSortieTerrain() {
	wantsToSelectMultiple = false;
	wantsToSelect = false;
}

void ofApp::button_pressed_ajouterobstacle() {
	menu = 10;
}

void ofApp::button_pressed_ajouterSphere() {
	drawSphere = true;
	drawCyl - false;
	drawMod = false;
}
void ofApp::button_pressed_ajouterCylindre() {
	drawSphere = false;
	drawMod = false;
	drawCyl = true;
}
void ofApp::button_pressed_ajouterModele() {
	drawSphere = false;
	drawCyl = false;
	drawMod = true;
}

void ofApp::button_pressed_saveObj3d(){
	if(drawSphere){
		ofVec3f r(absObstacle, ordObstacle, zObstacle);
		prime.saveCe(radObstacle, r, colObstacle);
	}
	if (drawCyl) {
		ofVec3f r(absObstacle, ordObstacle, zObstacle);
		prime.saveCy(radObstacle,heightCylinder, r, colObstacle);
	}
	if (drawMod) {
		prime.saveM(degXmodel, degYmodel, scaleModel, colObstacle, transitoryModel);
	}
	drawSphere = false; 
	drawCyl = false;
	drawMod = false;
}
void ofApp::buttonretourajouer() {
	menu = 1;
	drawSphere = false;
	drawCyl = false;
	drawMod = false;
}

void ofApp::button_pressed_retour_a_edition2d() {
	menu = 7;
}

void ofApp::button_pressed_modifier1ligne() {
	if(hasSelectedSmthing)menu = 8;
	hasSelectedSmthing = false;
	wantsToSelect = false;
	wantsToSelectMultiple = false;
	modifyingLines = false;
	modifyingOneLine = true;
}
void ofApp::button_pressed_modifierplusieurs() {
	//if has selected
	if(hasSelectedThings)menu = 9;
	hasSelectedThings = false;
	wantsToSelect = false;
	wantsToSelectMultiple = false;
	modifyingOneLine = false;
	modifyingLines = true;
}

void ofApp::button_pressed_undoprime() {
	prime.undo();
}
void ofApp::button_pressed_redoprime() {
	prime.redo();
}

void ofApp::afficherMur() {
	vue = 1;
	menu = 11;
}

void ofApp::buttonretouraedition() {
	vue = 2;
	menu = 8;
}
void ofApp::buttonimportimg() {
	wantsimport = true;
}

void ofApp::exportimg() {
	ofImage image;
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");
	image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	image.save(time_stamp);
}

//exportation d'image
void ofApp::image_export(const string name, const string extension) const
{
	ofImage image;

	// extraire des donn?es temporelles format?es
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// g?n?rer un nom de fichier unique et ordonn?
	string file_name = name + time_stamp + "." + extension;

	// capturer le contenu du framebuffer actif
	image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	// sauvegarder le fichier image
	image.save(file_name);

	//ofLog() << "<export image: " << file_name << ">";
	ofLog() << stroke_color;
}

void ofApp::button_pressed()
{
	// réinitialiser la zone de texte
	ofApp::image_export("render", "png");

}

//background et contour
void ofApp::button_pressed_2()
{
	// réinitialiser la zone de texte
	if (actif_button) {
		actif_button = false;
		button_contour.setName("Activer Contour");
	}
	else {
		actif_button = true;
		button_contour.setName("Desactiver Contour");
	}
	this->update();

	//ofLog() << "<button pressed_2>";
}

void ofApp::draw_app() {
	ofNoFill();
	if (actif_button) {
		//ofLog() << "<app::actif_btn_draw>";
		//ofNoFill();
	}

	else {
		//ofFill();
		//ofLog() << "<app::No_actif_btn_draw>";
	}

	//this.draw();


}

void ofApp::update_app()
{
	ofNoFill();
	// assigner les états courants de l'interface
	background_color = color_picker_background;
	stroke_color = color_picker_stroke;
	stroke_weight = slider_stroke_weight;

	ofSetColor(stroke_color);
	ofSetLineWidth(stroke_weight);

	labyrinthe.background = background_color;
	line.epaisseur = stroke_weight;

	ofNoFill();
	if (actif_button) {
		//ofLog() << "<app::actif_btn>";
		//ofNoFill();
		//ofLog() << "Hi";
	}

	else {

		ofNoFill();
		//ofLog() << "<app::No_actif_btn>";
	}

	//this->update();
}


void ofApp::button_cam_gauche() {
	x_index = x_index - 100;
	this->update();

}
void ofApp::button_cam_droite() {
	x_index = x_index + 100;
	this->update();
}
void ofApp::button_cam_haut() {
	y_index = y_index - 100;
	this->update();
}
void ofApp::button_cam_bas() {
	y_index = y_index + 100;
	this->update();
}
void ofApp::button_cam_zoomer() {
	z_index = z_index +500;
	this->update();
}

void ofApp::button_cam_dezoomer() {
	z_index = z_index - 500;
	this->update();
}
void ofApp::drawVector(ofPoint v, ofPoint loc, float scayl) {
	ofPushMatrix();

	// Translate to location to render vector
	ofTranslate(loc);
	ofColor(255);
	// Call vector heading function to get direction (note that pointing up is a heading of 0) and rotate
	float angle = (float)atan2(-v.y, v.x);
	float theta = -1.0 * angle;
	float heading2D = ofRadToDeg(theta);

	ofRotateZ(heading2D);

	// Calculate length of vector & scale it to be bigger or smaller if necessary
	float len = v.length() * scayl;
	// Draw three lines to make an arrow (draw pointing up since we've rotate to the proper direction)


	ofPopMatrix();

}