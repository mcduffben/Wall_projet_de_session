#include "ofApp.h"
#include "renderer.h"
#include "../../../../Desktop/OF/of_v0.11.2_vs2017_release/libs/glew/include/GL/glew.h"
#include "../../../../Desktop/OF/of_v0.11.2_vs2017_release/libs/openFrameworks/graphics/of3dGraphics.h"

//--------------------------------------------------------------
void ofApp::setup(){
	myfont.load("times-new-roman.ttf", 32);
	ofSetBackgroundColor(229, 235, 231);
//	ofEnableDepthTest();
	ofDisableArbTex();
	//ofDisableAlphaBlending();
	
	tone_mapping_exposure = 1.0f;
	tone_mapping_gamma = 2.2f;
	tone_mapping_toggle = true;

	offset_vertical = 32;
	offset_horizontal = 32;
	
	// dimensions de l'image source
	image_width = image_source.getWidth();
	image_height = image_source.getHeight();
	shader.load("tone_mapping_330_vs.glsl", "tone_mapping_330_fs.glsl");
	image_destination.allocate(image_source.getWidth(), image_source.getHeight(), OF_IMAGE_COLOR);
		
	kernel_type = ConvolutionKernel::identity;
	kernel_name = "identité";

		// appliquer le filtre de convolution par défaut
	filter();
	slider_exposure.set("exposure", tone_mapping_exposure, 0.0f, 5.0f);
	slider_gamma.set("gamma", tone_mapping_gamma, 0.0f, 5.0f);

	if (tone_mapping_toggle)
		toggle_tone_mapping.set("aces filmic", true);
	else
		toggle_tone_mapping.set("reinhard", false);

	image = image_source;
	
	image.resize(image_source.getWidth()/2, image_source.getHeight() / 3);
	image.update();
	mTex.enableMipmap();
	mTex1.enableMipmap();

	mTex = image.getTextureReference();
	mTex1 = image.getTexture();
	
	mTex.generateMipmap();
	mTex1.generateMipmap();

	mTex1.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	mTex.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
	plane.set(ofGetWidth()*2, ofGetHeight()*2);   ///dimensions for width and height in pixels
	plane.setPosition(0, 0, 0); /// position in x y z
	

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
	//View : 1 = vue d'un seul mur, 2 = vue de dessus en 2d, 3 = vue en 3d
	menu = 0;
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
	
	player.stopAllAnimations();
	player.resetAllAnimations();
	player.setPositionForAllAnimations(0);
	player.setRotation(1,1,90,90,0);
	//Setup du UI
	setupUi();
	
	if (tone)
		this->draw();


	//Illuminations
	ofSetBackgroundColor(0);
	ofSetFrameRate(60);
	//
	ofSetSphereResolution(50);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	// paramètres
	oscillation_amplitude = 32.0f;
	oscillation_frequency = 7500.0f;
	speed_motion = 150.0f;
	initial_x = 0.0f;
	initial_z = -100.0f;
	scale_gouraud = 60.0f;
	scale_phong = 40.0f;
	scale_blinn_phong = 0.618f;
	// initialisation des variables
	offset_x = initial_x;
	offset_z = initial_z;

	delta_x = speed_motion;
	delta_z = speed_motion;

	shader_gouraud.load(
		"shader/gouraud_330_vs.glsl",
		"shader/gouraud_330_fs.glsl");

	shader_phong.load(
		"shader/phong_330_vs.glsl",
		"shader/phong_330_fs.glsl");

	shader_blinn_phong.load(
		"shader/blinn_phong_330_vs.glsl",
		"shader/blinn_phong_330_fs.glsl");

	// initialisation de la scène
	reset();

	// transformer la lumière
	
	light.setGlobalPosition(
		ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
		ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
		-offset_z * 1.5f);

	// mise à jour d'une valeur numérique animée par un oscillateur
	float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;

		
		shader_gouraud.begin();
		shader_gouraud.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_gouraud.setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
		shader_gouraud.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_gouraud.setUniform1f("brightness", 20);
		shader_gouraud.setUniform3f("light_position", light.getGlobalPosition());
		shader_gouraud.end();

		shader_phong.begin();
		shader_phong.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_phong.setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
		shader_phong.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_phong.setUniform1f("brightness", 20);
		shader_phong.setUniform3f("light_position", light.getGlobalPosition());
		shader_phong.end();
		

		shader_blinn_phong.begin();
		shader_blinn_phong.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
		shader_blinn_phong.setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
		shader_blinn_phong.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
		shader_blinn_phong.setUniform1f("brightness", 90);
		shader_blinn_phong.setUniform3f("light_position", light.getGlobalPosition());
		shader_blinn_phong.end();
		
		//ofEnableDepthTest();
}

// fonction d'oscillation
float ofApp::oscillate(float time, float frequency, float amplitude)
{
	return sinf(time * 2.0f * PI / frequency) * amplitude;
}
void ofApp::reset() {
	
	// centre du framebuffer
	center_x = ofGetWidth() / 2.0f;
	center_y = ofGetHeight() / 2.0f;

	// caméra à sa position initiale
	offset_x = initial_x;
	offset_z = initial_z;

	// déterminer la position des géométries
	position_gouraud.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	position_phong.set(0.0f, 0.0f, 0.0f);
	position_blinn_phong.set(ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
	
	ofLog() << "<reset>";
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

	group_tone_mapping.setup("tone mapping");

	group_tone_mapping.add(slider_exposure);
	group_tone_mapping.add(slider_gamma);
	group_tone_mapping.add(toggle_tone_mapping);
	guiPrincipal.add(&group_tone_mapping);
	//filtrage
	

	filtrage.setup("Filtrage");
	
	linear.set("Linear", false);
	nearest.set("Nearest", false);
	filtrage.add(linear);
	filtrage.add(nearest);
	//gui.add(&filtrage);
	guiPrincipal.add(&filtrage);
	//Illumination
	activer.set("Activer", false);
	sphere.set("Sphere", false);
	illimunation.setup("Illumination");
	illimunation.add(activer);
	illimunation.add(sphere);
	guiPrincipal.add(&illimunation);


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

	tone_mapping_exposure = slider_exposure;
	tone_mapping_gamma = slider_gamma;
	tone_mapping_toggle = toggle_tone_mapping;



	if (tone_mapping_toggle)
		toggle_tone_mapping.set("aces filmic", true);
	else
		toggle_tone_mapping.set("reinhard", false);
	//filtrage
	
	//Illuminations
	
	if (illum_active)
		this->draw();
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	//ofEnableDepthTest();

	ofNoFill();


	renderer.draw();
	//camera.begin();
	if (vue == 1){
		labyrinthe.drawWall();
		
	}
	//camera3.begin();
	if (vue == 2 and tone) {

		if (linear) {
			ofSetWindowShape(
				image_destination.getWidth(),
				image_destination.getHeight());
			// activer le filtre
			shader.begin();
			shader.setUniformTexture("image", mTex, 1);
			shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
			shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
			shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
			cam.begin();
			mTex.bind();
			plane.draw();
			mTex.unbind();
			cam.end();
			shader.end();
			
		}
		else if (activer) {
			ofDisableAlphaBlending();
			
			ofDisableArbTex();
			//cam.begin();
			//ofDrawSphere(0.0f, 0.0f, 0.0f, 100);
			//Illuminations
			// activer l'éclairage dynamique
			ofEnableLighting();

			// activer la lumière dynamique
			light.enable();

			ofPushMatrix();

			// transformer l'origine de la scène au milieu de la fenêtre d'affichage
			ofTranslate(center_x + offset_x, center_y, offset_z);

			ofPushMatrix();

			// positionnner le cube

			ofTranslate(
				position_gouraud.x,
				position_gouraud.y,
				position_gouraud.z);
			
			// activer le shader
			shader_gouraud.begin();

			// dessiner un cube
			ofEnableDepthTest();
			ofFill();
			//ofSetColor(255, 0, 0);
			if(sphere){
				ofDrawSphere(0.0f, 0.0f, 0.0f, scale_gouraud);
			}else{
			ofDrawCone(scale_gouraud, scale_gouraud*2);
			}
			//ofDrawBox(100);
			ofDisableDepthTest();
			shader_gouraud.end();
			
			ofPopMatrix();
			shader_phong.begin();
			ofPushMatrix();

			// positionner la sphère
			ofTranslate(
				position_phong.x,
				position_phong.y,
				position_phong.z);
			ofEnableDepthTest();
			if (sphere){
				ofDrawSphere(0.0f, 0.0f, 0.0f, scale_gouraud);
			}else{
			ofDrawCone(scale_gouraud, scale_gouraud * 2);
			}
			ofDisableDepthTest();
			ofPopMatrix();

			ofPushMatrix();
			shader_phong.end();
			ofTranslate(
				position_blinn_phong.x,
				position_blinn_phong.y,
				position_blinn_phong.z);

			shader_blinn_phong.begin();
			ofEnableDepthTest();
			if (sphere){
				ofDrawSphere(0.0f, 0.0f, 0.0f, scale_gouraud);
			}else{
			ofDrawCone(scale_gouraud, scale_gouraud * 2);
			}
			ofDisableDepthTest();
			ofPopMatrix();

			ofPopMatrix();

			// désactiver le shader
			shader_blinn_phong.end();
			
			// désactiver la lumière
			light.disable();

			// désactiver l'éclairage dynamique
			ofDisableLighting();
			//cam.end();
		}

		else if (nearest) {
			// activer le filtre
			shader.begin();

			shader.setUniformTexture("image", mTex1, 1);

			shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
			shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
			shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
			cam.begin();
			mTex1.bind();
			plane.draw();
			mTex1.unbind();
			cam.end();
			shader.end();

		}else{
		//labyrinthe.draw(color_picker_stroke, background_color, stroke_weight, color_dessin);
		// activer le filtre
		shader.begin();
		//image.draw(offset_horizontal, offset_vertical, image.getWidth(), image.getHeight());
		// passer les attributs uniformes au shader
		//shader.setUniform1f("fenetre", 1);

		shader.setUniformTexture("image", mTex, 1);

		shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
		shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
		shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);




		// dessiner l'image sur la seconde moitié de la surface de la fenêtre
		//image.draw(image.getWidth() + offset_horizontal * 2, offset_vertical, image.getWidth(), image.getHeight());
		//image.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		//cam.begin();

		//image.getTexture().bind();
		
		// désactiver le filtre
		//image.getTexture().unbind();
		//cam.end();
		
		if(image_destination.getWidth()<=100) {
			image_destination.draw(ofGetWidth()/2, ofGetHeight()/2);
		}
		else {
			ofSetWindowShape(
				image_destination.getWidth(),
				image_destination.getHeight());
			image_destination.draw(0, 0);
		}
		
		shader.end();
		
		}
		
	}
	/*
	else if (vue == 2) {
		labyrinthe.draw(color_picker_stroke, background_color, stroke_weight, color_dessin);
		image_destination.draw(offset_horizontal, offset_vertical, image1.getWidth(), image1.getHeight());
	}*/
		
	
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
	switch (key)
	{
	case 49: // touche 1
		kernel_type = ConvolutionKernel::identity;
		kernel_name = "identité";
		break;

	case 50: // touche 2
		kernel_type = ConvolutionKernel::emboss;
		kernel_name = "bosseler";
		break;

	case 51: // touche 3
		kernel_type = ConvolutionKernel::sharpen;
		kernel_name = "aiguiser";
		break;

	case 52: // touche 4
		kernel_type = ConvolutionKernel::edge_detect;
		kernel_name = "détection de bordure";

		break;

	case 53: // touche 5
		kernel_type = ConvolutionKernel::blur;
		kernel_name = "flou";
		break;
	//case 54:
		//image_export("render", "png");

	default:
		break;
	}

	// appliquer le filtre de convolution
	filter();

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
	if (tone) {
		/*
		ofSetFullscreen(true);
		screen_w = ofGetWidth();
		screen_h = ofGetHeight();
		ofSetFullscreen(false);
		*/
		ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
			<< " at : " << dragInfo.position << ">";

		// importer le premier fichier déposé sur la fenêtre si c'est une image (attention : aucune validation du type de fichier)
		//ofTexture mTex;
		ofLoadImage(mTex, dragInfo.files.at(0));
		image_source.load(dragInfo.files.at(0));
		//image.load(dragInfo.files.at(0));
		//image_destination.load(dragInfo.files.at(0));
		image_width = image_source.getWidth();
		image_height = image_source.getHeight();
		//image_source.resize(1024, 864);
		
		
		if (image_width > image_height ) {
			image_source.resize(image_height, image_height);
			image_source.update();
		}
		else if (image_width < image_height ) {
			image_source.resize(image_width, image_width);
			image_source.update();
		}
		if(image_source.getWidth()>800){
		image_source.resize(1024, 768);
		image_source.update();
		
		}
		this->setup();
			

	}

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

// fonction de filtrage par convolution
void ofApp::filter()
{
	if(image_source.getWidth()>0 and image_source.getHeight()>0){
		// résolution du kernel de convolution
		const int kernel_size = 3;

		// décalage à partir du centre du kernel
		const int kernel_offset = kernel_size / 2;

		// nombre de composantes de couleur (RGB)
		const int color_component_count = 3;

		// indices de l'image
		int x, y;

		// indices du kernel
		int i, j;

		// indices d'échantillonnage;
		int xi, yj;

		// index des composantes de couleur
		int c;

		// index du pixel de l'image source utilisé pour le filtrage
		int pixel_index_img_src;

		// index du pixel de l'image de destination en cours de filtrage
		int pixel_index_img_dst;

		// index du pixel de l'image de destination en cours de filtrage
		int kernel_index;

		// valeur à un des indices du kernel de convolution
		float kernel_value;

		// extraire les pixels de l'image source
		ofPixels pixel_array_src = image_source.getPixels();

		// extraire les pixels de l'image de destination
		ofPixels pixel_array_dst = image_destination.getPixels();

		// couleur du pixel lu dans l'image source
		ofColor pixel_color_src;

		// couleur du pixel à écrire dans l'image de destination
		ofColor pixel_color_dst;

		// somme du kernel appliquée à chaque composante de couleur d'un pixel
		float sum[color_component_count];

		// itération sur les rangées des pixels de l'image source
		for (y = 0; y < image_height; ++y)
		{
			// itération sur les colonnes des pixels de l'image source
			for (x = 0; x < image_width; ++x)
			{
				// initialiser le tableau où les valeurs de filtrage sont accumulées
				for (c = 0; c < color_component_count; ++c)
					sum[c] = 0;

				// déterminer l'index du pixel de l'image de destination
				pixel_index_img_dst = (image_width * y + x) * color_component_count;

				// itération sur les colonnes du kernel de convolution
				for (j = -kernel_offset; j <= kernel_offset; ++j)
				{
					// itération sur les rangées du kernel de convolution
					for (i = -kernel_offset; i <= kernel_offset; ++i)
					{
						// calculer les indices d'échantillonnage
						xi = x - i;
						yj = y - j;

						// traitement de la bordure
						if (xi < 0 || xi > image_width || yj < 0 || yj > image_height)
							continue;

						// déterminer l'index du pixel de l'image source à lire
						pixel_index_img_src = (image_width * yj + xi) * color_component_count;

						// lire la couleur du pixel de l'image source
						pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

						// déterminer l'indice du facteur à lire dans le kernel de convolution
						kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

						// extraction de la valeur à cet index du kernel
						switch (kernel_type)
						{
						case ConvolutionKernel::identity:
							kernel_value = convolution_kernel_identity.at(kernel_index);
							break;

						case ConvolutionKernel::emboss:
							kernel_value = convolution_kernel_emboss.at(kernel_index);
							break;

						case ConvolutionKernel::sharpen:
							kernel_value = convolution_kernel_sharpen.at(kernel_index);
							break;

						case ConvolutionKernel::edge_detect:
							kernel_value = convolution_kernel_edge_detect.at(kernel_index);
							break;

						case ConvolutionKernel::blur:
							kernel_value = convolution_kernel_blur.at(kernel_index);
							break;

						default:
							kernel_value = convolution_kernel_identity.at(kernel_index);
							break;
						}

						// itération sur les composantes de couleur
						for (c = 0; c < color_component_count; ++c)
						{
							// accumuler les valeurs de filtrage en fonction du kernel de convolution
							sum[c] = sum[c] + kernel_value * pixel_color_src[c];
						}
					}
				}

				// déterminer la couleur du pixel à partir des valeurs de filtrage accumulées pour chaque composante
				for (c = 0; c < color_component_count; ++c)
				{
					// conversion vers entier et validation des bornes de l'espace de couleur
					pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
				}

				// écrire la couleur à l'index du pixel en cours de filtrage
				pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
			}
		}

		// écrire les pixels dans l'image de destination
		image_destination.setFromPixels(pixel_array_dst);

		ofLog() << "<filtre de convolution complété (" << kernel_name << ")>";
	}
}
//filtrage
	
