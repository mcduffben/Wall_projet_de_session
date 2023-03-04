#include "Labyrinthe3d.h"
#include "Wall.h"
void Labyrinthe3d::setup() {
	epaisseur = 5;
	//ofEnableDepthTest();
	default_Z = ofGetWindowHeight() / 2;
}

void Labyrinthe3d::update() {
	
}

void Labyrinthe3d::draw() {
	if(!murMatrice.empty())
		for (size_t i = 0; i < murMatrice.size(); i++)
		{	
			ofFill();
			ofSetColor(10, 10, 10, 1000);
			murMatrice[i].draw();
		}
}
void Labyrinthe3d::wallGen(Labyrinthe lab) {
	if (lab.murs2Dbasique.size() > 0) {
		for (int i = 0; i < lab.murs2Dbasique.size(); i++) { 
			
			Walls ajout(lab.murs2Dbasique[i].pinit, lab.murs2Dbasique[i].pfinal,epaisseur);

			murs.push_back(ajout);
			ofBoxPrimitive newbox;
			newbox.setWidth(ajout.pinit.distance(ajout.pfinal));
			newbox.setHeight(default_Z);

			newbox.setDepth(ajout.mpinit.distance(ajout.mpfinal));
			newbox.setPosition(ajout.pinit);
			murMatrice.push_back(newbox);
		}
	}
}
