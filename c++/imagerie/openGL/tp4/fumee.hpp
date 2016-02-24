#ifndef FUMEE_HPP
#define FUMEE_HPP

#include <list>
#include "stb_image.h"
#include <GL/glu.h>
#include "bouffee.hpp"
#include "texture.hpp"

using namespace std;

class Fumee {
public:
	float xposf, yposf, zposf;
	float intervalle_emission;
	float temps_ecoule;
	Texture* texture;
	list<Bouffee> list_bouffee;

    Fumee(float =0.0f, float =0.0f , float =0.0f, float =1.0f , float =0.0f, Texture* = NULL);
    inline ~Fumee() { delete texture; }
	void anime(float );
	void affiche();

};

#endif
