#pragma once

#include "ofMain.h"

class Renderer
{
public:

	int m_x;
	int m_y;

	void setup();

	void draw();
	void drawCursor();

	void asgCoordinates(int p_x, int p_y);
	
	void essai();

};
