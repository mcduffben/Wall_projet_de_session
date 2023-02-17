#include "renderer.h"

void Renderer::setup()
{
	ofSetFrameRate(30);
	ofEnableSmoothing();
	//Le curseur est cachÅE
	ofHideCursor();
	fbo.allocate(width, height);
}

void Renderer::draw()
{
	fbo.draw(0, 0);
	ofNoFill();
	ofDrawRectangle(0, 0, width, height);
	polyline.clear();
	
	
}
void Renderer::reset() {
	fbo.begin();
	ofClear(255);
	fbo.end();
}

void Renderer::drawApercu(int x, int y,float murThick)
{
	ofColor red;

	red.r = 255;
	red.g = 0;
	red.b = 0;

	fbo.begin();
	ofSetLineWidth(murThick);
	ofSetColor(red);
	//ofDrawLine(x,y,firstP.x,firstP.y);
	//polyline.clear();
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

Renderer::~Renderer()
{
}