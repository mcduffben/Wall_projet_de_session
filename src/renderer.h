#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Renderer
{
public:
	void setup();

	void draw();
	void drawCursor();
  




    ofImage imgScreenshot;
    int count;
    ofVec3f firstP;
    ofVec3f lastPoint;
    ofFbo fbo;
    int width = 800;
    int height = 600;
    ~Renderer();

    void drawApercu(int x, int y,float epaiseur);
    void reset();
    ofxButton btnClear;
    ofPolyline polyline;
    
    void drawLine(int x, int y);

 
};
