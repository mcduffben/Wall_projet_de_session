#include "renderer.h"

void Renderer::setup()
{
	ofSetFrameRate(30);
	
	//Hiding the normal mouse cursor
	ofHideCursor();
	
	//Mouse coordinates (To draw the cursor)
	m_x = 0;
	m_y = 0;
}

void Renderer::draw()
{
	

	//Drawing the cursor at the end
	drawCursor();
}

void Renderer::drawCursor()
{
	//Pour l'instant, je n'ai programmé qu'un seul curseur. Quand le projet va être plus avancé, je vais en rajouter d'autres. -Vincent
	//Drawing the first cursor
	ofSetLineWidth(5);
	ofSetColor(255);
	ofDrawLine(m_x, m_y, m_x, m_y + 40);
	ofDrawLine(m_x, m_y, m_x + 16, m_y + 25);
}

//Assigning new coordinates when the mouse moves
void Renderer::asgCoordinates(int p_x, int p_y)
{
	m_x = p_x;
	m_y = p_y;
}