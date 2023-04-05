#pragma once
#include "ofMain.h"
#include "effecdeParticule.h"

class particleSystem{

public:
    particleSystem(ofPoint, ofImage);
    void update();
    void display();
    void addParticle();
    
    ofPoint origin;
    ofImage img;
    vector<effecdeParticule*> particles;
};