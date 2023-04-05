#include "ModelDTO.h"

ModelDTO::ModelDTO(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo)
	:degX(dex), degY(dey), scale(sc), col(co), model(mo) {}

void ModelDTO::moveUP() {
	model.setPosition(model.getPosition().x, model.getPosition().y + 5, model.getPosition().x);
}
void ModelDTO::movedown() {
	model.setPosition(model.getPosition().x, model.getPosition().y- 5, model.getPosition().x);
}
void ModelDTO::moveLeft() {
	model.setPosition(model.getPosition().x + 5, model.getPosition().y + 5, model.getPosition().x);
}
void ModelDTO::moveRight() {
	model.setPosition(model.getPosition().x-5, model.getPosition().y , model.getPosition().x);
}
void ModelDTO::Collision() {

}