#pragma once
#include "ofMain.h"

class PrimitiveDTO
{
public:
	struct CercleDTO
	{
		int rayon;
		ofVec2f position;

		CercleDTO(int & r, ofVec2f p)
			:rayon(r), position(p){}
	};

	struct CylindreDTO
	{
		int height,rayon;
		ofVec2f position;

		CylindreDTO(int & r, int & h, ofVec2f & p)
			:height(h),rayon(r),position(p){}
	};

	struct FormeVecDTO
	{

	};

	vector<CercleDTO> cercles;
	vector<CylindreDTO> cylindres;
	vector<FormeVecDTO> formes;


};

