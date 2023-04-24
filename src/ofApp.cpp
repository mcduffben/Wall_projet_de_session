#include "ofApp.h"
#include "renderer.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	renderlabyrinthe3d = true;
	camTemporaire.setPosition(0, 0, 0);

	tessRender.setup();


	myfont.load("times-new-roman.ttf", 32);
	ofSetBackgroundColor(229, 235, 231);
	
	
	labyrinthe.setup();
	renderer.setup();
	prime.setup();

	img.load("texture.png");

	//Menu : 0 = menu principal, 1 = jeu, 2 = conception, 3 = options, 
	// 4 = mur basique (Conception), 5 = mur basique par param, 6 = mur basique dessin, 7 = édition 2D, 8 = edition ligne 2d
	//View : 1 = vue d'un seul mur, 2 = vue de dessus en 2d, 3 = vue en 3d
	menu = 0;
	vue = 2;
	//La liste des curseurs pour chaque menu
	listeCurseurs = { 0, 1, 2, 3, 4, 4, 4, 5,0,0,0,0,0,0 };

	//Setup de variables
	freeDraw, wantsToSelect, hasSelectedSmthing, wantsToSelectMultiple, hasSelectedThings = false;
	modifyingLines, modifyingOneLine, drawSphere, drawCyl, drawMod, wantsimport = false;
	horizontal = true;
	oldfloatsliderx = 0;
	oldfloatslidery = 0;

	
	cam.setPosition(x_index, y_index, z_index);
	

	//Setup du UI
	setupUi();

	ofSetVerticalSync(true); // Enable vertical sync to limit the framerate
	//ofEnableDepthTest(); // Enable depth testing for proper rendering of 3D objects
	ofDisableArbTex();

	//LANCER DE RAYON
	boxMaterial.setAmbientColor(ofColor(63, 63, 63));
	boxMaterial.setDiffuseColor(ofColor(127, 0, 0));
	boxMaterial.setEmissiveColor(ofColor(31, 0, 0));
	boxMaterial.setSpecularColor(ofColor(127, 127, 127));
	boxMaterial.setShininess(16.0f);

	// configurer la lumière ambiante
	light_ambient.set(127, 127, 127);

	// configurer la lumière directionnelle
	light_directional.setDiffuseColor(ofColor(31, 255, 31));
	light_directional.setSpecularColor(ofColor(191, 191, 191));
	light_directional.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_directional.setDirectional();

	// configurer la lumière ponctuelle
	light_point.setDiffuseColor(ofColor(255, 255, 255));
	light_point.setSpecularColor(ofColor(191, 191, 191));
	light_point.setPointLight();

	// configurer la lumière projecteur
	light_spot.setDiffuseColor(ofColor(191, 191, 191));
	light_spot.setSpecularColor(ofColor(191, 191, 191));
	light_spot.setOrientation(ofVec3f(0.0f, 0.0f, 0.0f));
	light_spot.setSpotConcentration(2);
	light_spot.setSpotlightCutOff(30);
	light_spot.setSpotlight();

	areaLight.setup();
	areaLight.enable();
	areaLight.setAreaLight(400, 120);
	areaLight.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3));
	areaLight.setAttenuation(1.0, 0.00003, 0.00001);
	areaLight.setDiffuseColor(ofFloatColor(1, 1, 1));
	areaLight.setSpecularColor(ofFloatColor(1, 1, 1));

	ofBackground(0);
	plane.set(20000, 20000, 2, 2);
	plane.rotateDeg(-90, glm::vec3(1, 0, 0));
	plane.move(glm::vec3(0, -300, 0));
	materialPlane.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1, 1.0));
	materialPlane.setDiffuseColor(ofFloatColor(0.8, 0.8, 0.8, 1.0));
	materialPlane.setSpecularColor(ofFloatColor(0.8, 0.8, 0.8, 1.0));
	materialPlane.setShininess(10);


	camera.setFarClip(20000);
	camera.move(0, 0, 1000);

	//logoMesh.load("oflogo.ply");

	materialMesh.setAmbientColor(ofFloatColor(0.85, 0.16, 0.43, 0.4));
	materialMesh.setDiffuseColor(ofFloatColor(0.85, 0.16, 0.43, 1.0));
	materialMesh.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	materialMesh.setShininess(10);

	box.set(100);
	box.move(1, 5, 2);
	
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
	guiPrincipal.add(boutonOptions.setup("Shader de Tesselation"));
	boutonOptions.addListener(this, &ofApp::button_pressed_options);
	guiPrincipal.add(boutonDelaunay.setup("Triangulation de Delaunay"));
	boutonDelaunay.addListener(this, &ofApp::button_pressed_delaunay);
	guiPrincipal.add(button.setup("Upload"));
	button.addListener(this, &ofApp::button_pressed);
	
	group_draw.add(color_picker_background);
	group_draw.add(color_picker_stroke);
	group_draw.add(slider_stroke_weight);
	group_draw.add(color_dessin);
	guiPrincipal.add(&group_draw);
	
	//PBR
	is_key_press_up = false;
	is_key_press_down = false;
	is_key_press_left = false;
	is_key_press_right = false;
	is_key_press_q = false;
	is_key_press_e = false;
	pbr.setup();
	reset();
	pbr_active.setName("PBR");
	guiPrincipal.add(pbr_active);
	gui_material.setup("pbr material");
	// interface pour les couleurs du matériau
	group_material_color.setup("color");
	group_material_color.add(color_picker_ambient);
	group_material_color.add(color_picker_diffuse);
	group_material_color.add(color_picker_specular);
	gui_material.add(&group_material_color);
	// interfullbookace pour les facteurs numériques du matériau
	group_material_factor.setup("factor");
	group_material_factor.add(slider_metallic);
	group_material_factor.add(slider_roughness);
	group_material_factor.add(slider_occlusion);
	group_material_factor.add(slider_brightness);
	group_material_factor.add(slider_fresnel_ior);
	gui_material.add(&group_material_factor);
	// interface pour les paramètres de la lumière
	group_light.setup("light");
	group_light.add(color_picker_light_color);
	group_light.add(slider_light_intensity);
	group_light.add(toggle_light_motion);
	gui_material.add(&group_light);
	// interface pour les paramètres de mappage tonal
	group_tone_mapping.setup("tone mapping");
	group_tone_mapping.add(slider_exposure);
	group_tone_mapping.add(slider_gamma);
	group_tone_mapping.add(toggle_tone_mapping);
	gui_material.add(&group_tone_mapping);

	//button_reset.setup("reset");
	//button_reset.addListener(this, &ofApp::button_reset_pressed);
	
	guiPrincipal.add(&gui_material);
	lancer_rayon.setName("Lancer_rayon");
	guiPrincipal.add(lancer_rayon);
	
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
	guiJeu.add(&labyrinthe.mapping);
	

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
	guiOptions.setup("Shader de tesselation");
	boutonExitOptions.setup("Retour");
	boutonExitOptions.addListener(this, &ofApp::button_pressed_exit);
	guiOptions.add(&boutonExitOptions);

	guiDelaunay.setup("Triangulation de Delaunay");
	guiDelaunay.add(boutonExitDelaunay.setup("Retour"));
	boutonExitDelaunay.addListener(this, &ofApp::button_pressed_exit);
	

	//Setup du UI Conception Mur basique
	guiConceptionMurBasique.setup("Conception de murs basiques");
	guiConceptionMurBasique.setSize(300, 100);
	guiConceptionMurBasique.add(boutonByParameters.setup("Creation Par Coordonnees"));
	guiConceptionMurBasique.add(boutonDessinLibre.setup("Creation Par Dessin Libre"));
	guiConceptionMurBasique.add(boutonBezier.setup("Creation par Bezier"));
	guiConceptionMurBasique.add(button3.setup("Upload"));
	button3.addListener(this, &ofApp::button_pressed);
	guiConceptionMurBasique.add(boutonRetourConceptionMur.setup("Retour"));
	boutonByParameters.addListener(this, &ofApp::button_pressed_drawByParameters);
	boutonDessinLibre.addListener(this, &ofApp::button_pressed_freeDraw);
	boutonBezier.addListener(this, &ofApp::creationparBezier);
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
	guiEditionLigne.add(posyline.setup("Position y",400, 0, 2000));
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

	guiObstacle.add(absDir.setup("Abscisse Lumiere", 0, -1000, 1000));
	guiObstacle.add(ordDir.setup("Ordonnee Lumiere", 0, -1000, 1000));
	guiObstacle.add(hautDir.setup("Hauteur Lumiere", 100, -1000, 1000));
	
	guiObstacle.add(&prime.illimunation);
	
	/*
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
	dezoomer.addListener(this, &ofApp::button_cam_dezoomer);*/

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
	//tone_mapping
	affichageMur.add(&labyrinthe.group_tone_mapping);
	
	affichageMur.add(&labyrinthe.filtrage);
	
}

//--------------------------------------------------------------
void ofApp::update() {

	if (vue == 3) {
		prime.hautDir = hautDir;
		prime.absDir = absDir;
		prime.ordDir = ordDir;
		prime.update3d();
	}

	if (vue == 4)tessRender.update();

	//cam.setPosition(x_index, y_index, z_index);
	if(vue==2)labyrinthe.update(color_picker_stroke, background_color, slider_stroke_weight, color_dessin);
	if (vue == 3)labyrinthe.update3d(color_picker_stroke, background_color, slider_stroke_weight,color_dessin);


	if(vue==2)labyrinthe.update(color_picker_stroke, background_color, stroke_weight, color_dessin);
	if (vue == 3)labyrinthe.update3d(color_picker_stroke, background_color, stroke_weight, color_dessin);

	ofBackground(color_picker_background);
	

	if (pbr_active) {
		time_current = ofGetElapsedTimef();
		time_elapsed = time_current - time_last;
		time_last = time_current;

		if (is_key_press_up)
			pbr.offset_z += pbr.delta_z * time_elapsed;
		if (is_key_press_down)
			pbr.offset_z -= pbr.delta_z * time_elapsed;
		if (is_key_press_left)
			pbr.offset_x += pbr.delta_x * time_elapsed;
		if (is_key_press_right)
			pbr.offset_x -= pbr.delta_x * time_elapsed;
		if (is_key_press_q)
			pbr.rotation_y += pbr.delta_y * time_elapsed;
		if (is_key_press_e)
			pbr.rotation_y -= pbr.delta_y * time_elapsed;

		pbr.material_color_ambient = color_picker_ambient;
		pbr.material_color_diffuse = color_picker_diffuse;
		pbr.material_color_specular = color_picker_specular;

		pbr.material_metallic = slider_metallic;
		pbr.material_roughness = slider_roughness;
		pbr.material_occlusion = slider_occlusion;
		pbr.material_brightness = slider_brightness;

		pbr.material_fresnel_ior = slider_fresnel_ior;

		pbr.light_color = color_picker_light_color;
		pbr.light_intensity = slider_light_intensity;
		pbr.light_motion = toggle_light_motion;

		pbr.tone_mapping_exposure = slider_exposure;
		pbr.tone_mapping_gamma = slider_gamma;
		pbr.tone_mapping_toggle = toggle_tone_mapping;

		if (pbr.tone_mapping_toggle)
			toggle_tone_mapping.set("aces filmic", true);
		else
			toggle_tone_mapping.set("reinhard", false);

		pbr.update();
		
	}
	//LANCER RAYON
	if (lancer_rayon) {
		float elapsedTime = ofGetElapsedTimef();
		float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f / 10000.0, 1.f / 5.f);

		if (mode == 0) {
			float speed = 0;// ofMap(ofGetMouseX(), 100, ofGetWidth() - 100, 0.1, 24, true);
			orbitSpeed += deltaTime * speed;
			areaLight.setPosition(cos(orbitSpeed) * -600, (sin(elapsedTime * 1.1) * 200), sin(orbitSpeed) * -600);
			areaLight.lookAt(glm::vec3(0, -150, 0));
			areaLight.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3));
		}
		else if (mode == 1) {
			float mousePercent = ofMap(ofGetMouseX(), 100, ofGetWidth() - 100, 0, 1, true);
			highwaySpeed = ofMap(mousePercent, 0, 1, 500, 5000, true);
			areaLight.setAmbientColor(ofFloatColor(0.0, 0.0, 0.0));
			float zPosition = ofClamp(sin(elapsedTime), 0, 1) * 500.0 + 200;
			areaLight.setPosition(areaLight.getPosition().x + deltaTime * highwaySpeed, 350, zPosition);
			if (areaLight.getPosition().x > 1500) {
				areaLight.setPosition(-1500, areaLight.getPosition().y, zPosition);
			}
			areaLight.lookAt(glm::vec3(areaLight.getPosition().x, areaLight.getPosition().y - 300, areaLight.getPosition().z));
			areaLight.rollDeg(90);

			float noiseValue = ofSignedNoise(areaLight.getPosition().y * mousePercent * 0.001, areaLight.getPosition().x * mousePercent * 0.001, mousePercent * elapsedTime);
			if (noiseValue > 1.0 - mousePercent * 0.5 && bump > 1.0) {
				bump = 0.0;
				bumpHeight = ofRandom(10, 80);
			}
			bump += deltaTime * (3.0 + (mousePercent + 0.2));
		}
		else if (mode == 2) {
			float mousePercent = ofMap(ofGetMouseX(), 100, ofGetWidth() - 100, 0, 1, true);
			areaLight.setPosition(cos(elapsedTime * 2.0) * -600, fabs(sin(elapsedTime * 2.0) * 600) - 200, sin(elapsedTime * 3.4f) * 200 + 400);
			areaLight.tiltDeg(deltaTime * (mousePercent * 360.0 * 3.0 + 5.0f));
		}
	}
	

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofFill();

	if (vue == 4)
	{
		pbr_active.set(false);
		drawTess();
	}

	if (vue == 5) {
		ofNoFill();
		triangulation.draw();
		ofFill();
	}
	
	
	//camera.begin();
	else if (vue == 1){
		pbr_active.set(false);
		labyrinthe.drawWall();
		
	}
	else if (vue == 2) {
		
		if(renderlabyrinthe3d)labyrinthe.draw(color_picker_stroke, background_color, stroke_weight, color_dessin);

		ofNoFill();
		renderer.draw();
		if (pbr_active){
			ofSetFullscreen(true);
			pbr.draw();
		}
		else if (lancer_rayon) {
			ofFill();

			ofEnableLighting();
			ofEnableDepthTest();
			camera.begin();
			materialPlane.begin();
			plane.draw();
			ofDrawSphere(0, -300, 0, 10000);
			materialPlane.end();

			materialMesh.begin();
			ofPushMatrix();
			ofTranslate(0, -300, 0);
			if (mode == 1) {
				if (bump < 1.0) {
					ofTranslate(0.0, sin(bump * glm::pi<float>()) * bumpHeight, 0.0);
				}
			}
			ofRotateXDeg(-90);
			ofDrawSphere(0, 0, 0, 0);
			ofPopMatrix();
			materialMesh.end();

			boxMaterial.begin();
			ofDrawSphere(100);
			boxMaterial.end();

			boxMaterial.begin();
			ofRotate(30);

			box.draw();
			boxMaterial.end();

			areaLight.draw();
			camera.end();

			ofDisableDepthTest();
			ofDisableLighting();

			ofDrawBitmapStringHighlight("Mode (m): " + ofToString(mode, 0), 15, 15);


		}

		//camera.begin();
		
		
	}else 
	if (vue == 3) {
		pbr_active.set(false);
		cam.begin();
		ofPushMatrix();

		//image_mapping.getTextureReference().bind();
		//ofEnableDepthTest();
		if(renderlabyrinthe3d)labyrinthe.draw3d(color_picker_stroke, background_color,stroke_weight,color_dessin);
		//ofDisableDepthTest();
		//image_mapping.getTextureReference().unbind();

		//prime se trouve dans PrimivitiveDTO .h/.cpp
		//toutes les objets 3d sont affichées ici
     	prime.draw3d();
		//fin des objets 3d


			//labyrinthe.draw3d(color_picker_stroke, background_color, stroke_weight, color_dessin);
		
		
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
		// 
		//camTemporaire.end();
		ofPopMatrix();
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
			for (int i = 0; i < labyrinthe.murs2Dbasique.size(); i++) {
				if (labyrinthe.murs2Dbasique[i].selected) {
					labyrinthe.murs2Dbasique[i].pinit.x = posxline;
					labyrinthe.murs2Dbasique[i].pfinal.x = posxline + labyrinthe.murs2Dbasique[i].diffx;
					labyrinthe.murs2Dbasique[i].pinit.y = posyline;
					labyrinthe.murs2Dbasique[i].pfinal.y = posyline + labyrinthe.murs2Dbasique[i].diffy;
				}
			}
		}
		if (modifyingLines && xlines != oldfloatsliderx) {
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

		//ofBackground(stroke_color);

	//cam.end();
	renderer.drawCursor(listeCurseurs[menu]);
	//ofDisableDepthTest();
	
	
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
	else if (menu == 12) {
		guiDelaunay.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (pbr_active) {
		switch (key)
		{
		case 97:  // touche a
			is_key_press_left = true;
			break;

		case 100: // touche d
			is_key_press_right = true;
			break;

		case 101: // touche q
			is_key_press_q = true;
			break;

		case 113: // touche e
			is_key_press_e = true;
			break;

		case 115: // touche s
			is_key_press_down = true;
			break;

		case 119: // touche w
			is_key_press_up = true;
			break;

		case OF_KEY_LEFT:  // touche ←
			is_key_press_left = true;
			break;

		case OF_KEY_UP:    // touche ↑
			is_key_press_up = true;
			break;

		case OF_KEY_RIGHT: // touche →
			is_key_press_right = true;
			break;

		case OF_KEY_DOWN:  // touche ↓
			is_key_press_down = true;
			break;

		default:
			break;
		}
	}
	else if (lancer_rayon){
		if (key == 'm') {
			mode++;
			if (mode > 2) {
				mode = 0;
			}
		}
}
	else{
	switch (key)
	{
	case 48: // key 0
		renderlabyrinthe3d = !renderlabyrinthe3d;
		break;
	case 49: // key 1
		prime.materialChooser = 1;
		break;

	case 50: // key 2
		prime.materialChooser = 2;
		break;

	case 51: // key 3
		prime.materialChooser = 3;
		break;

	case 52: // key 4
		prime.materialChooser = 0;
		break;
	case 53: // key 5
		prime.renderAmbient = !prime.renderAmbient;
		break;
	case 54: // key 6
		prime.renderDir=!prime.renderDir;
		break;
	case 55: // key 7
		prime.renderPoint=!prime.renderPoint;
		break;
	case 56: // key 8
		prime.renderSpot=!prime.renderSpot;
		break;

		
	default:
		break;
	}
	}

	if (vue == 5 && key == 'r') {
		triangulation.reset();
	}
}
//pbr

void ofApp::reset()
{
	pbr.reset();

	color_picker_ambient.set("ambient", pbr.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
	color_picker_diffuse.set("diffuse", pbr.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
	color_picker_specular.set("specular", pbr.material_color_specular, ofColor(0, 0), ofColor(255, 255));

	slider_metallic.set("metallic", pbr.material_metallic, 0.0f, 1.0f);
	slider_roughness.set("roughness", pbr.material_roughness, 0.0f, 1.0f);
	slider_occlusion.set("occlusion", pbr.material_occlusion, 0.0f, 5.0f);
	slider_brightness.set("brightness", pbr.material_brightness, 0.0f, 5.0f);

	slider_fresnel_ior.set("fresnel ior", pbr.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f));

	color_picker_light_color.set("color", pbr.light_color, ofColor(0, 0), ofColor(255, 255));
	slider_light_intensity.set("intensity", pbr.light_intensity, 0.0f, 10.0f);

	toggle_light_motion.set("motion", pbr.light_motion);

	slider_exposure.set("exposure", 1.0f, 0.0f, 5.0f);
	slider_gamma.set("gamma", 2.2f, 0.0f, 5.0f);

	if (pbr.tone_mapping_toggle)
		toggle_tone_mapping.set("aces filmic", true);
	else
		toggle_tone_mapping.set("reinhard", false);
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (pbr_active) {
		switch (key)
		{
		case 97:  // touche a
			is_key_press_left = false;
			break;

		case 100: // touche d
			is_key_press_right = false;
			break;

		case 101: // touche q
			is_key_press_q = false;
			break;

		case 109: // touche m
			toggle_light_motion = !toggle_light_motion;
			ofLog() << "<toggle light motion: " << toggle_light_motion << ">";
			break;

		case 113: // touche e
			is_key_press_e = false;
			break;

		case 114: // touche r
			if(vue!=5)reset();
			ofLog() << "<reset renderer>";
			break;

		case 115: // touche s
			is_key_press_down = false;
			break;

		
		
		case 119: // touche w
			is_key_press_up = false;
			break;

		case OF_KEY_LEFT:  // touche ←
			is_key_press_left = false;
			break;

		case OF_KEY_UP:    // touche ↑
			is_key_press_up = false;
			break;

		case OF_KEY_RIGHT: // touche →
			is_key_press_right = false;
			break;

		case OF_KEY_DOWN:  // touche ↓
			is_key_press_down = false;
			break;

		default:
			break;
		}
	}
	labyrinthe.keyReleased(key);
	
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
	if (vue == 5) {
		triangulation.addPoint(ofPoint(x, y));
		triangulation.triangulate();
	}
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

	if (this->creationBezier) {
		labyrinthe.addsphereCatmull({ (float)x,(float)y});
		if(this->bezier.size()<5)this->bezier.push_back({ (float)x,(float)y,0 });
		if (this->bezier.size() == 5) {
			labyrinthe.drawCatmullRom(this->bezier);
			this->bezier.clear();
		}
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
	vue = 4;
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
	this->creationBezier = false;
	menu = 5;
}

void ofApp::button_pressed_freeDraw() {
	menu = 6;
	freeDraw = true;
	this->creationBezier = false;
}

void ofApp::button_pressed_retourConception() {
	this->creationBezier = false;
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
	this->creationBezier = false;
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


void ofApp::creationparBezier() {
	this->creationBezier = true;
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


void ofApp::setupTesselation() {

	ofDisableArbTex();

	ofFboSettings settings;
	settings.internalformat = GL_RGB32F;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.useDepth = true;
	settings.depthStencilAsTexture = true;

	fbo.allocate(settings);

	fboTess.begin();
	ofClear(0, 0, 0);

	// No need to clear the alpha channel since I'm using the GL_RGB32F format
	// ofClearAlpha();
	fboTess.end();

	parameters.setName("Settings");

	parameters.add(tessRender.parameters[0]);

	guiTesslation.setup(parameters);
}
void ofApp::drawTess() {


	fboTess.begin();

	ofClear(0);

	tessRender.draw();
	fboTess.end();

	guiTesslation.draw();


}

void ofApp::button_pressed_delaunay() {
	vue = 5;
	menu = 12;
}

