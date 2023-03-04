#pragma once
#include <Labyrinthe.h>

#include "Wall.h"
class Labyrinthe3d
{

public:
	void setup();
	void update();
	void draw();
	float epaisseur,default_Z;
	
	void wallGen(Labyrinthe lab);
	vector<ofBoxPrimitive> murMatrice;
	vector<Walls> murs;
};