#include "renderer.h"

void Renderer::setup()
{
	ofSetFrameRate(30);
	
	//Le curseur est cach�
	ofHideCursor();
}

void Renderer::draw()
{
	

	//Le curseur est dessin� � la fin
	drawCursor();
}

void Renderer::drawCursor()
{
	//Curseur 1 dessin�
	ofSetLineWidth(5);
	ofSetColor(255);
	ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
	ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);
	ofSetLineWidth(1);
	ofSetColor(0);
	ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
	ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);
}