#include "renderer.h"

void Renderer::setup()
{
	ofSetFrameRate(30);
	
	//Le curseur est caché
	ofHideCursor();
}

void Renderer::draw()
{
	
}

void Renderer::drawCursor(int curseur)
{
	ofNoFill();

	//curseur "normal"
	if (curseur == 0) {
		ofSetLineWidth(5);
		ofSetColor(255);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);

		ofSetLineWidth(1);
		ofSetColor(0);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);
	}

	//cible
	else if (curseur == 1) {
		ofSetColor(50);
		ofSetLineWidth(3);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 15);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 2);

		ofSetColor(220);
		ofSetLineWidth(1);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 15);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1);
	}

	//lignes vers le bas
	else if (curseur == 2) {
		ofSetLineWidth(2);

		ofSetColor(70);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 30, ofGetMouseY() - 30);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 30, ofGetMouseY() - 30);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 10, ofGetMouseY() - 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 10, ofGetMouseY() - 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 20, ofGetMouseY() - 10);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 20, ofGetMouseY() - 10);

		ofSetColor(185);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 25, ofGetMouseY() - 18);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 25, ofGetMouseY() - 18);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 18, ofGetMouseY() - 33);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 18, ofGetMouseY() - 33);

	}

	//flèche
	else if (curseur == 3) {
		ofSetLineWidth(6);
		ofSetColor(0);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 40, ofGetMouseY());
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() - 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 45, ofGetMouseY() - 45);

		ofSetLineWidth(4);
		ofSetColor(128);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 40, ofGetMouseY());
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() - 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 45, ofGetMouseY() - 45);

		ofSetLineWidth(2);
		ofSetColor(255);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 40, ofGetMouseY());
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() - 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 45, ofGetMouseY() - 45);


	}

	//crayon
	else if (curseur == 4) {
		ofSetColor(230);
		ofSetLineWidth(5);
		ofDrawLine(ofGetMouseX() + 3, ofGetMouseY() + 8, ofGetMouseX() - 3, ofGetMouseY() + 8);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 10, ofGetMouseY() + 30);
		ofSetLineWidth(4);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 10, ofGetMouseY() + 30);
		ofDrawLine(ofGetMouseX() - 10, ofGetMouseY() + 30, ofGetMouseX() - 10, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() - 5, ofGetMouseY() + 30, ofGetMouseX() - 5, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX(), ofGetMouseY() + 30, ofGetMouseX(), ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 5, ofGetMouseY() + 30, ofGetMouseX() + 5, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 10, ofGetMouseY() + 30, ofGetMouseX() + 10, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 10, ofGetMouseY() + 30, ofGetMouseX() - 10, ofGetMouseY() + 30);

		ofSetColor(20);
		ofSetLineWidth(2);
		ofDrawLine(ofGetMouseX() + 3, ofGetMouseY() + 8, ofGetMouseX() - 3, ofGetMouseY() + 8);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() - 10, ofGetMouseY() + 30);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 10, ofGetMouseY() + 30);
		ofDrawLine(ofGetMouseX() - 10, ofGetMouseY() + 30, ofGetMouseX() - 10, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() - 5, ofGetMouseY() + 30, ofGetMouseX() - 5, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX(), ofGetMouseY() + 30, ofGetMouseX(), ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 5, ofGetMouseY() + 30, ofGetMouseX() + 5, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 10, ofGetMouseY() + 30, ofGetMouseX() + 10, ofGetMouseY() + 70);
		ofDrawLine(ofGetMouseX() + 10, ofGetMouseY() + 30, ofGetMouseX() - 10, ofGetMouseY() + 30);
	}

	//8 lignes
	else if (curseur == 5) {
		ofSetLineWidth(3);
		ofSetColor(50);
		ofDrawLine(ofGetMouseX() + 3, ofGetMouseY(), ofGetMouseX() + 13, ofGetMouseY());
		ofDrawLine(ofGetMouseX() - 3, ofGetMouseY(), ofGetMouseX() - 13, ofGetMouseY());
		ofDrawLine(ofGetMouseX(), ofGetMouseY() - 3, ofGetMouseX(), ofGetMouseY() - 13);
		ofDrawLine(ofGetMouseX(), ofGetMouseY() + 3, ofGetMouseX(), ofGetMouseY() + 13);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 2);

		ofSetLineWidth(2);
		ofSetColor(250);
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 1);
		ofDrawLine(ofGetMouseX() + 2, ofGetMouseY() + 2, ofGetMouseX() + 9, ofGetMouseY() + 9);
		ofDrawLine(ofGetMouseX() + 2, ofGetMouseY() - 2, ofGetMouseX() + 9, ofGetMouseY() - 9);
		ofDrawLine(ofGetMouseX() - 2, ofGetMouseY() + 2, ofGetMouseX() - 9, ofGetMouseY() + 9);
		ofDrawLine(ofGetMouseX() - 2, ofGetMouseY() - 2, ofGetMouseX() - 9, ofGetMouseY() - 9);
	}
}