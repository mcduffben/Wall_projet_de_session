#include "effecdeParticule.h"

effecdeParticule::effecdeParticule(ofPoint l, ofImage img_) {
    acceleration.set(1,1,1);
    
    velocite.set(ofRandom(-1,1), ofRandom(-2,0), ofRandom(0,3));
    location = l;
    lifespan = 100.0;
   
    img = img_;
}

void effecdeParticule::update() {
    velocite += acceleration;
    location += velocite;
    acceleration *= 0;
    lifespan -= 2.0;
}

void effecdeParticule::display() {
    img.draw(location);
}



bool effecdeParticule::isDead() {
    if (lifespan < 0.0) {
        return true;
    } else {
        return false;
    }
}