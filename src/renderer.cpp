#include "renderer.h"

void Renderer::setup()
{
	ofSetFrameRate(30);
	ofEnableSmoothing();
	//Le curseur est cachÅE
	ofHideCursor();
}

void Renderer::draw()
{
	fbo.draw(0, 0);
	ofNoFill();
	ofDrawRectangle(0, 0, width, height);
	
	
}
void Renderer::drawApercu(int x, int y,float murThick,Color)
{

	fbo.begin();
	ofSetLineWidth(guiConception);
	ofSetColor();
	polyline.clear();
	polyline.addVertex(ofPoint(x, y));
	polyline.lineTo(firstP);
	polyline.draw();
	fbo.end();
}

void Renderer::drawCursor()
{
	//Curseur 1 dessinÅE
		ofSetLineWidth(5);
		ofSetColor(255);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);
		ofSetLineWidth(1);
		ofSetColor(0);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX(), ofGetMouseY() + 40);
		ofDrawLine(ofGetMouseX(), ofGetMouseY(), ofGetMouseX() + 16, ofGetMouseY() + 25);
}