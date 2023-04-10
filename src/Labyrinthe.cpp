#include "Labyrinthe.h"
#include <math.h>

void Labyrinthe::setup() {
	//mesh.set(ofGetWidth()*0.5, ofGetHeight()*0.5);
	//plane.setPosition(ofGetWidth() * 0.5, ofGetHeight() * 0.5,0);
	epaisseur = 1000;
	hauteur = 1000;
	echelle = 5;
	posEntreeX = ofGetWindowWidth() / 2;
	posEntreeY = ofGetWindowHeight() / 2 + hauteur/2;
	posSortieX = ofGetWindowWidth() / 2;
	posSortieY = ofGetWindowHeight() / 2 - hauteur/2;
	jeu_arbitraire = 20;
	
	background.set(0, 0, 0);
	centre3d.x = 0;
	centre3d.y = 0;
	centre3d.z = 0;

	hauteurMur3d = 100;

	tone_mapping_exposure = 1.0f;
	tone_mapping_gamma = 2.2f;
	tone_mapping_toggle = true;
	shader.load("tone_mapping_330_vs.glsl", "tone_mapping_330_fs.glsl");
	//setup de tes menus
	group_tone_mapping.setup("tone mapping");
	group_tone_mapping.add(slider_exposure);
	group_tone_mapping.add(slider_gamma);
	group_tone_mapping.add(toggle_tone_mapping);
	filtrage.setup("Filtrage");
	linear.set("Linear", false);
	nearest.set("Nearest", false);
	filtrage.add(linear);
	filtrage.add(nearest);

	//mapping
	mapping.setup("Mapping");
	normal_mapping.set("Normal", false);
	displa_mapping.set("Displacement", false);
	mapping.add(normal_mapping);
	
	//mapping.add(displa_mapping);

	slider_exposure.set("exposure", tone_mapping_exposure, 0.0f, 5.0f);
	slider_gamma.set("gamma", tone_mapping_gamma, 0.0f, 5.0f);

	if (tone_mapping_toggle)
		toggle_tone_mapping.set("aces filmic", true);
	else
		toggle_tone_mapping.set("reinhard", false);

	
	//set up de tes shader
	

	kernel_type = ConvolutionKernel::identity;
	kernel_name = "identité";
	ofDisableArbTex();
	image_mapping.load("mur.jpg");
	
	//Fonction height mapping
		int meshWidth = 500; // Number of vertices along the x-axis
		int meshHeight = 500; // Number of vertices along the y-axis
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		for (int y = 0; y < meshHeight; y++) {
			for (int x = 0; x < meshWidth; x++) {
				// Calculate the position of the vertex
				float xPos = ofMap(x, 0, meshWidth - 1, 0, ofGetWidth());
				float yPos = ofMap(y, 0, meshHeight - 1, 0, ofGetHeight());
				mesh.addVertex(ofVec3f(xPos, yPos, 0));
				// Set the texture coordinates of the vertex
				float u = ofMap(x, 0, meshWidth - 1, 0, 1);
				float v = ofMap(y, 0, meshHeight - 1, 0, 1);
				mesh.addTexCoord(ofVec2f(u, v));
			}
		}
		// Add the triangles to the mesh
		for (int y = 0; y < meshHeight - 1; y++) {
			for (int x = 0; x < meshWidth - 1; x++) {
				int index = y * meshWidth + x;
				mesh.addTriangle(index, index + 1, index + meshWidth);
				mesh.addTriangle(index + 1, index + meshWidth + 1, index + meshWidth);
			}
		}

		for (int i = 0; i < mesh.getNumVertices(); i++) {
			ofVec3f vertex = mesh.getVertex(i);
			float u = mesh.getTexCoord(i).x;
			float v = mesh.getTexCoord(i).y;
			// Map the texture coordinates to the range [-1, 1]
			float displacement = ofMap(image_mapping.getColor(u * image_mapping.getWidth() / 2, v * image_mapping.getHeight() / 2).r, 0, 255, -1, 1);
			// Displace the vertex based on the displacement value
			vertex.z = displacement * 50; // Adjust the scale of the displacement
			mesh.setVertex(i, vertex);
		}
		//mesh.setTexCoord();
		
		//plane.mapTexCoordsFromTexture(texture);

		normalMapShader.load("shader.vert", "shader_fs.frag");

}

void Labyrinthe::update(ofColor color, ofColor back, float epais, ofColor dessin) {
	ofSetColor(color);
	background.set(back);
	ofSetLineWidth(epais);
	posEntreeX = ofGetWindowWidth() / 2;
	posEntreeY = ofGetWindowHeight() / 2 + hauteur / 2;
	posSortieX = ofGetWindowWidth() / 2;
	posSortieY = ofGetWindowHeight() / 2 - hauteur / 2;
	if (displa_mapping)
			this->displacement_map();
	//if(displa_mapping){
	/*
		
		}*/

	//}
}

void Labyrinthe::draw(ofColor color, ofColor back, float epais, ofColor dessin) {
	ofSetLineWidth(epais);
	ofSetColor(dessin);
	ofFill();
	ofDrawPlane(ofGetWindowWidth()/2,ofGetWindowHeight()/2,0,epaisseur,hauteur);
	ofSetColor(color);
	if (murs2Dbasique.size() > 0) {
		for (int i = 0; i < murs2Dbasique.size(); i++) {
			if (murs2Dbasique[i].selected)ofSetColor(0, 0, 175);
			else ofSetColor(color);
			ofDrawLine(murs2Dbasique[i].pinit, murs2Dbasique[i].pfinal);
		}
		if (mathPreview.size() > 0) {
			for (int i = 0; i < mathPreview.size(); i++) {
				ofSetColor(color);
				ofDrawLine(mathPreview[i].pinit, mathPreview[i].pfinal);
			}
		}
	}
	ofSetColor(color);
	ofDrawPlane(posEntreeX,posEntreeY, 0, 20, 20);
	ofSetColor(color);
	ofDrawPlane(posSortieX, posSortieY, 0, 20, 20);
	
}

void Labyrinthe::update3d(ofColor color, ofColor back, float epais, ofColor dessin) {
	centre3d.x = 0;// -epaisseur / 2;
	centre3d.y = 0;// -hauteur / 2;
	centre3d.z = 0;
}
void Labyrinthe::draw3d(ofColor color, ofColor back, float epais, ofColor dessin) {
	
	ofFill();
	if (normal_mapping or displa_mapping) {
		cam.begin();
		image_mapping.getTextureReference().bind();
		
		ofEnableDepthTest();
		
		
		ofDrawPlane(centre3d.x, centre3d.y, centre3d.z, epaisseur, hauteur);
		
		
		
		ofDisableDepthTest();
		
		image_mapping.getTextureReference().unbind();
		cam.end();
	}
	else {
		ofSetColor(0, 0, 0);
		//ofDrawPlane(centre3d.x, centre3d.y, centre3d.z, epaisseur, hauteur);
		mesh.plane(epaisseur, hauteur).draw();
	}
	
	
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		ofSetColor(color);
		ofPushMatrix();
		ofRotate(murs2Dbasique[i].angle);
		ofDrawBox((murs2Dbasique[i].pinit.x+ murs2Dbasique[i].pfinal.x)/2 - ofGetWindowWidth()/2, (murs2Dbasique[i].pfinal.y+murs2Dbasique[i].pinit.y)/2-ofGetWindowHeight()/2, 0, murs2Dbasique[i].largeur, murs2Dbasique[i].epaisseur, murs2Dbasique[i].hauteur);
		ofPopMatrix();
	}
}

void Labyrinthe::drawWall() {


	int largeur;
	LineDTO l({ 0,0 }, { 0, 0 }, false);
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		if (murs2Dbasique[i].selected) {
			l = murs2Dbasique[i];
		}
	}
	ofVec2f piE, pfE;
	piE = {ofGetWindowWidth()/2.0f-l.largeur,ofGetWindowHeight()/2.0f-l.hauteur};
	pfE = {piE.x+l.largeur,piE.y+l.hauteur};
	ofSetColor(255, 255, 255);

	//ici on dessine le mur
	ofDrawLine(piE, { piE.x,pfE.y });
	ofDrawLine(piE, { pfE.x,piE.y });
	ofDrawLine(pfE, { piE.x,pfE.y });
	ofDrawLine(pfE, { pfE.x,piE.y });


	if (l.hasImage) {
		if (linear or nearest) {
			image = l.img;
			image.resize(l.img.getWidth() / 2, l.img.getHeight() / 2);
			image.update();
			mTex.enableMipmap();
			mTex = image.getTexture();
			if(nearest)
				mTex.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
			else
				mTex.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
			mTex.generateMipmap();
		}
		else {

			l.img.resize(512, 512);
			l.img.update();
			image_source = l.img;
			image_width = image_source.getWidth();
			image_height = image_source.getHeight();
			image_destination.allocate(image_source.getWidth(), image_source.getHeight(), OF_IMAGE_COLOR);
			filter();
			l.img = image_destination;
		}
		tone_mapping_exposure = slider_exposure;
		tone_mapping_gamma = slider_gamma;
		tone_mapping_toggle = toggle_tone_mapping;

		if (tone_mapping_toggle)
			toggle_tone_mapping.set("aces filmic", true);
		else
			toggle_tone_mapping.set("reinhard", false);


		shader.begin();
		if(linear or nearest)
			shader.setUniformTexture("image", l.img.getTextureReference(), 1);
		else
			shader.setUniformTexture("image", mTex, 1);

		shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
		shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
		shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);
		if(linear or nearest)
			mTex.draw((piE.x + 5), (piE.y + 5), l.largeur - 10, l.hauteur - 10);
		else {

			l.img.draw((piE.x + 5), (piE.y + 5), l.largeur - 10, l.hauteur - 10);
		}

		shader.end();

	}



	//menu.draw()
}


void Labyrinthe::setNewLineX(int x) {
	this->newLineX = x;
}
void Labyrinthe::setNewLineY(int y) {
	this->newLineY = y;
}
void Labyrinthe::addNewLine(int x, int y) {
	ofVec2f pi, pf;
	pi.x = newLineX;
	pi.y = newLineY;
	pf.x = x;
	pf.y = y;
	if (pi.x < pf.x) {
		LineDTO newLine(pi, pf);
		murs2Dbasique.push_back(newLine);
	}
	else {
		LineDTO newLine(pf, pi);
		murs2Dbasique.push_back(newLine);
	}
}


void Labyrinthe::addNewLine(ofVec2f pi, ofVec2f pf,bool select) {
	if(pi.x<pf.x){ 
		LineDTO newLine(pi, pf, select);
		murs2Dbasique.push_back(newLine);
	}
	else {
		LineDTO newLine(pf, pi, select);
		murs2Dbasique.push_back(newLine);
	}
}

void Labyrinthe::unselect_all() {
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		murs2Dbasique[i].selected = false;
	}
}

bool Labyrinthe::selectCheckerSingle(bool single,int x, int y) {
	if(single)unselect_all();
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		if ((murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x) < jeu_arbitraire && (murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x) > -jeu_arbitraire) {
			if (murs2Dbasique[i].pfinal.y > murs2Dbasique[i].pinit.y) {
				if (x < (murs2Dbasique[i].pfinal.x + jeu_arbitraire) && x > (murs2Dbasique[i].pfinal.x - jeu_arbitraire) && y > murs2Dbasique[i].pinit.y
					&& y<murs2Dbasique[i].pfinal.y) {
					murs2Dbasique[i].selected = true;
					return true;
				}
			}
			else {
				if (x < (murs2Dbasique[i].pfinal.x + jeu_arbitraire) && x > (murs2Dbasique[i].pinit.x - jeu_arbitraire) && y < murs2Dbasique[i].pinit.y
					&& y>murs2Dbasique[i].pfinal.y) {
					murs2Dbasique[i].selected = true;
					return true;
				}
			}
		}
		float pente, borigine;
		pente = (murs2Dbasique[i].pfinal.y - murs2Dbasique[i].pinit.y) / (murs2Dbasique[i].pfinal.x - murs2Dbasique[i].pinit.x);
		borigine = murs2Dbasique[i].pinit.y - pente * murs2Dbasique[i].pinit.x;
		if ((y - jeu_arbitraire) < (pente * x + borigine) && (y + jeu_arbitraire) > (pente * x + borigine)
			&& x> murs2Dbasique[i].pinit.x && x< murs2Dbasique[i].pfinal.x) {
			murs2Dbasique[i].selected = true;
			return true;
		}
	}
	return false;
}


void Labyrinthe::drawPreview(int x, int y) {
	mathPreview.clear();
	
	ofVec2f po, pf;
	po = this->po;
	pf.x = x;
	pf.y = y;
	LineDTO preview(po, pf);
	mathPreview.push_back(preview);

	//preview.pinit = po;
	//preview.pfinal.x = x;
	//preview.pfinal.y = y;
	//ofDrawLine(preview.pinit,preview.pfinal);
}

void Labyrinthe::importimg(ofImage img) {
	for (int i = 0; i < murs2Dbasique.size(); i++) {
		if (murs2Dbasique[i].selected) {
			murs2Dbasique[i].img = img;
			murs2Dbasique[i].hasImage = true;
		}
	}
}


void Labyrinthe::keyReleased(int key) {
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
	//filter();

}
void Labyrinthe::displacement_map() {
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f vertex = mesh.getVertex(i);
		float u = mesh.getTexCoord(i).x;
		float v = mesh.getTexCoord(i).y;
		// Map the texture coordinates to the range [-1, 1]
		float displacement = ofMap(image_mapping.getColor(u * image_mapping.getWidth() / 2, v * image_mapping.getHeight() / 2).r, 0, 255, -1, 1);
		// Displace the vertex based on the displacement value
		vertex.z = displacement * 50; // Adjust the scale of the displacement
		mesh.setVertex(i, vertex);
	}
}

// fonction de filtrage par convolution
void Labyrinthe::filter()
{
	if (image_source.getWidth() > 0 and image_source.getHeight() > 0) {
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
