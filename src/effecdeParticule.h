#pragma once
#include "ofMain.h"

class effecdeParticule{
public:
    effecdeParticule(ofPoint, ofImage);
    void update();
    void display();
    bool isDead();
    
    ofPoint location;
    ofPoint velocite;
    ofPoint acceleration;
    float lifespan;
 
    ofImage img;
    
};