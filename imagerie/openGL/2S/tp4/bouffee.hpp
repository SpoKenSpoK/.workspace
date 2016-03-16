#ifndef BOUFFEE_HPP
#define BOUFFEE_HPP

#include "stb_image.h"
#include <GL/glu.h>
#include "texture.hpp"

class Bouffee {
public:
	float xpos, ypos, zpos;
	float vie;
    float taille;
    float vitesse_x, vitesse_y, vitesse_z;


    Bouffee(float =0.0f, float =0.0f, float =0.0f, float =5.0f, float =1.0f, float =0.0f, float =5.0f, float =0.0f);
    void anime(float );
    void affiche(Texture* );

};

#endif
