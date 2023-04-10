#include "PrimitiveDTO.h"

void PrimitiveDTO::setup() {
	navCe = 0;
	navCy = 0;
	navF = 0;
	navM = 0;
	navigator = -1;
	vector<CercleDTO> c;
	cerclesManager.push_back(c);
	vector<CylindreDTO> cy;
	cylindresManager.push_back(cy);
	vector<ModelDTO> m;
	modelManager.push_back(m);
	
	//setup des illuminations
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

	// initialisation de la sc�ne
	//reset();

	// transformer la lumi�re

	light.setGlobalPosition(
		ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
		ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
		-offset_z );

	// mise � jour d'une valeur num�rique anim�e par un oscillateur
	float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;


	shader_gouraud.begin();
	shader_gouraud.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shader_gouraud.setUniform3f("color_diffuse", 0.6f, 0.6f, 0.0f);
	shader_gouraud.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
	shader_gouraud.setUniform1f("brightness", 10);
	shader_gouraud.setUniform3f("light_position", light.getGlobalPosition());
	shader_gouraud.end();

	shader_phong.begin();
	shader_phong.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shader_phong.setUniform3f("color_diffuse", 0.6f, 0.0f, 0.6f);
	shader_phong.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
	shader_phong.setUniform1f("brightness", 10);
	shader_phong.setUniform3f("light_position", light.getGlobalPosition());
	shader_phong.end();


	shader_blinn_phong.begin();
	shader_blinn_phong.setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shader_blinn_phong.setUniform3f("color_diffuse", 0.0f, 0.6f, 0.6f);
	shader_blinn_phong.setUniform3f("color_specular", 1.0f, 1.0f, 0.0f);
	shader_blinn_phong.setUniform1f("brightness", 10);
	shader_blinn_phong.setUniform3f("light_position", light.getGlobalPosition());
	shader_blinn_phong.end();

	//setup de ton menu
	//Illumination
	gouraud.set("Gouraud", false);
	phong.set("Phong", false);
	blinn_phong.set("Blinn_Phong", false);
	illimunation.setup("Illumination");
	illimunation.add(gouraud);
	illimunation.add(phong);
	illimunation.add(blinn_phong);
	generer_texture.setName("Generer texture");
	illimunation.add(generer_texture);
	//sphere.mapTexCoordsFromTexture(texture);
	 // Cr�er une texture
	int w = 512;
	int h = 512;
	texture.allocate(w, h, GL_RGBA);

	// G�n�rer une texture proc�durale
	unsigned char* pixels = new unsigned char[w * h * 4];
	float scale = 0.01;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			float noiseValue = ofNoise(x * scale * x, y * 2 * scale);
			int index = (y * w + x) * 4;
			pixels[index] = 255 * noiseValue;
			pixels[index + 1] = 255 * noiseValue;
			pixels[index + 2] = 255 * noiseValue;
			pixels[index + 3] = 255;
		}
	}
	texture.loadData(pixels, w, h, GL_RGBA);
	delete[] pixels;
	sphere.mapTexCoordsFromTexture(texture);
}
float PrimitiveDTO::oscillate(float time, float frequency, float amplitude)
{
	return sinf(time * 2.0f * PI / frequency) * amplitude;
}

void PrimitiveDTO::draw() {
	

	for (int i = 0; i < cerclesManager[navCe].size(); i++) {
		ofSetColor(cerclesManager[navCe][i].couleur);
		sphere.setPosition(cerclesManager[navCe][i].position);
		sphere.set(cerclesManager[navCe][i].rayon, cerclesManager[navCe][i].rayon);
		sphere.draw();
	}
	for (int i = 0; i < cylindresManager[navCy].size(); i++) {
		ofSetColor(cylindresManager[navCy][i].couleur);
		ofDrawCircle(cylindresManager[navCy][i].position, cylindresManager[navCy][i].rayon);
	}

	for (int i = 0; i < formesManager[navF].size(); i++) {
		ofSetColor(formesManager[navF][i].couleur);
		formesManager[navF][i].draw();
	}
}

void PrimitiveDTO::draw3d() {

	//activation des illuminations
	if(generer_texture){
	texture.bind();
	}
	//vecteur des spheres qu on itere en dessinant chaque sphere
	ofTranslate(ofGetWidth() / 4, ofGetHeight() / 4);
	for (int i = 0; i < cerclesManager[navCe].size(); i++) {
		if (phong or gouraud or blinn_phong) {
			ofDisableAlphaBlending();
			ofDisableArbTex();
			ofEnableLighting();
			// activer la lumi�re dynamique
			light.enable();
			ofFloatColor c = cerclesManager[navCe][i].couleur;
			if (gouraud) {
				shader_gouraud.begin();
				shader_gouraud.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}
			else if (phong) {
				shader_phong.begin();
				shader_phong.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}else{
				shader_blinn_phong.begin();
				shader_blinn_phong.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}
			
			// dessiner un cube
			ofEnableDepthTest();
			//ofSetColor(cerclesManager[navCe][i].couleur);
			ofSetColor(cerclesManager[navCe][i].couleur);
			sphere.setPosition(cerclesManager[navCe][i].position);
			sphere.set(cerclesManager[navCe][i].rayon, cerclesManager[navCe][i].rayon);
			sphere.mapTexCoordsFromTexture(texture);
			
			sphere.draw();
			
			//ofDrawSphere(cerclesManager[navCe][i].position, cerclesManager[navCe][i].rayon);
			ofDisableDepthTest();
			if (gouraud)
				shader_gouraud.end();
			else if (phong)
				shader_phong.end();
			else
				shader_blinn_phong.end();
		}
		else {
			ofSetColor(cerclesManager[navCe][i].couleur);
			
			ofSetColor(cerclesManager[navCe][i].couleur);
			sphere.setPosition(cerclesManager[navCe][i].position);
			sphere.set(cerclesManager[navCe][i].rayon, cerclesManager[navCe][i].rayon);
			sphere.mapTexCoordsFromTexture(texture);
			sphere.draw();
		}

		
	}
	//vecteur des spheres qu on itere en dessinant chaque cylindres

	for (int i = 0; i < cylindresManager[navCy].size(); i++) {
		if (phong or gouraud or blinn_phong) {
			ofDisableAlphaBlending();
			ofDisableArbTex();
			ofEnableLighting();
			// activer la lumi�re dynamique
			light.enable();
			ofFloatColor c = cylindresManager[navCy][i].couleur;
			if (gouraud) {
				shader_gouraud.begin();
				shader_gouraud.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}
			else if (phong) {
				shader_phong.begin();
				shader_phong.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}
			else {
				shader_blinn_phong.begin();
				shader_blinn_phong.setUniform3f("color_diffuse", c.getNormalized().r, c.getNormalized().g, c.getNormalized().b);
			}

			// dessiner un cube
			ofEnableDepthTest();

		//ofSetColor(cylindresManager[navCy][i].couleur);
		ofDrawCylinder(cylindresManager[navCy][i].position, cylindresManager[navCy][i].rayon, cylindresManager[navCy][i].height);
		ofDisableDepthTest();
		if (gouraud)
			shader_gouraud.end();
		else if (phong)
			shader_phong.end();
		else
			shader_blinn_phong.end();
		}
		else {
			ofSetColor(cylindresManager[navCy][i].couleur);
			ofDrawCylinder(cylindresManager[navCy][i].position, cylindresManager[navCy][i].rayon, cylindresManager[navCy][i].height);
		}
	}


	//vecteur des spheres qu on itere en dessinant chaque modele

	for (int i = 0; i < modelManager[navM].size(); i++) {
		ofPushMatrix();
		ofRotateXDeg(modelManager[navM][i].degX);
		ofRotateYDeg(modelManager[navM][i].degY);
		ofSetColor(modelManager[navM][i].col);
		modelManager[navM][i].model.drawFaces();
		ofPopMatrix();


	}

	//desactivation des illuminations
	if (generer_texture) {
		texture.unbind();
	}
	
	//draw menu illumination
}

void PrimitiveDTO::undoCe() {
	if (navCe > 0) {
		navCe -= 1;
	}
}
void PrimitiveDTO::undoCy() {
	if (navCy > 0) {
		navCy -= 1;
	}
}
void PrimitiveDTO::undoF() {

}
void PrimitiveDTO::undoM() {
	if (navM > 0) {
		navM -=1;
	}
}
void PrimitiveDTO::redoCe() {
	if (navCe != cerclesManager.size() - 1) {
		navCe += 1;
	}
}
void PrimitiveDTO::redoCy() {
	if (navCy != cylindresManager.size() - 1) {
		navCy += 1;
	}
}
void PrimitiveDTO::redoF() {

}

void PrimitiveDTO::redoM() {
	if (navM != modelManager.size() - 1)
	{
		navM += 1;
	}
}
void PrimitiveDTO::saveCe(int r, ofVec3f p, ofColor c) {
	cleanIrrelevantHistorique();
	CercleDTO ce(r, p, c);
	vector < CercleDTO > mod = cerclesManager[navCe];
	cerclesManager.push_back(mod);
	navCe += 1;
	cerclesManager[navCe].push_back(ce);
	historique.push_back(1);
	navigator += 1;
}
void PrimitiveDTO::saveCy(int r, int h, ofVec3f p, ofColor c) {
	cleanIrrelevantHistorique();
	CylindreDTO cy(r, h, p, c);
	vector<CylindreDTO> mod = cylindresManager[navCy];
	cylindresManager.push_back(mod);
	navCy += 1;
	cylindresManager[navCy].push_back(cy);
	historique.push_back(2);
	navigator += 1;
}
void PrimitiveDTO::saveF() {

}

void PrimitiveDTO::saveM(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo) {
	cleanIrrelevantHistorique();
	ModelDTO m(dex, dey, sc, co, mo);
	vector<ModelDTO> mod = modelManager[navM];
	modelManager.push_back(mod);
	navM += 1;
	modelManager[navM].push_back(m);
	historique.push_back(3);
	navigator += 1;
}

void PrimitiveDTO::undo() {
	if(historique.size()>0 && navigator >-1){
		if(historique[navigator]==1)undoCe();
		if (historique[navigator] == 2)undoCy();
		if (historique[navigator] == 3)undoM();
		navigator -= 1;
	}
}

void PrimitiveDTO::redo() {
	if (historique.size() > 0) {
		if(navigator!=historique.size()-1)navigator += 1;
		if (historique[navigator] == 1)redoCe();
		if (historique[navigator] == 2)redoCy();
		if (historique[navigator] == 3)redoM();
	}
}

void PrimitiveDTO::cleanIrrelevantHistorique() {
	for (int i = navCe; i < cerclesManager.size()-1;i++) {
		int l = cerclesManager.size();
		cerclesManager.erase(cerclesManager.begin()+l-1);
	}
	for (int i = navCy; i < cylindresManager.size() - 1; i++) {
		int m = cylindresManager.size();
		cylindresManager.erase(cylindresManager.begin() + m - 1);
	}
	for (int i = navM; i < modelManager.size() - 1; i++) {
		int n = modelManager.size();
		modelManager.erase(modelManager.begin() + n - 1);
	}
	for (int i = navigator; i < historique.size() - 1; i++) {
		int p = historique.size();
		historique.erase(historique.begin() + p - 1);
	}
}