#ifndef BOUFFEE_HPP
#define BOUFFEE_HPP

#include "stb_image.h"
#include <GL/glu.h>

class Bouffee {
public:
	float xpos, ypos, zpos;
    float taille;
    float vitesse_x, vitesse_y, vitesse_z;
    float vie;

    Bouffee(float =0, float =0 , float =0, float =1, float =1 , float =0, float =0, float =10);
    void anime(float );
    void affiche();

};

#endif
