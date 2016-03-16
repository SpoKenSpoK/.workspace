#ifndef CERCLE_HPP
#define CERCLE_HPP

#include "objetGeometrique.hpp"

class Cercle : public objetGeometrique {
private:
	float centreX;
	float centreY;
	float rayon;

public:
	Cercle(unsigned int, float =0, float =0, float =1);
	~Cercle();
	virtual void calculPosition_sommets();
};

#endif
