#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include <stdio.h>

using namespace glm;

class TesselationRender
{

public:
    void setup();
    void draw();
    void update();
    array<ofParameterGroup, 3> parameters;


    ofShader shader;
    ofEasyCam camera;

    ofMesh mesh;
    ofTexture texture;
    ofxAssimpModelLoader model;

    ofParameter<bool> lightingEnabled = false;
    ofParameter<bool> wireframe = false;

    ofParameter<ofColor> lightColor;

    array<ofParameter<float>, 3> sliders;
    array<bool, 3> triggered = { false };
};

