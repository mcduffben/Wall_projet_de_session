#include "ModelDTO.h"

ModelDTO::ModelDTO(int dex, int dey, int sc, ofColor co, ofxAssimpModelLoader mo)
	:degX(dex), degY(dey), scale(sc), col(co), model(mo) {}