#include "tetraedre.hpp"
#include "math.h"
#include <iostream>

Tetraedre::Tetraedre(float _cote, float _x, float _y, float _z, float* _colors)
    : objetGeometrique(36), cote(_cote), x(_x), y(_y), z(_z), colors(_colors)
{}

Tetraedre::~Tetraedre(){ delete [] colors; }

void Tetraedre::calculPosition_sommets(){
    vertices = new float [36];

// Coordonnées trouvées sur le net
    //Première Face
	vertices[0] = x + sqrt(3)/6*cote;         //Rouge
	vertices[1] = y - 1/4*sqrt(2/3)*cote ;
	vertices[2] = z - cote/2 ;

	vertices[3] = x - sqrt(3)/3*cote;         //Vert
	vertices[4] = y - 1/4*sqrt(2/3)*cote ;
	vertices[5] = z;

	vertices[6] = x;                                //Bleu
	vertices[7] = y + sqrt(6)/4*cote ;
	vertices[8] = z;

    //Deuxième Face
    vertices[9] = x - sqrt(3)/3*cote;         //Vert
	vertices[10] = y - 1/4*sqrt(2/3)*cote;
	vertices[11] = z;

    vertices[12] = x;                                //Bleu
	vertices[13] = y + sqrt(6)/4*cote;
	vertices[14] = z;

    vertices[15] = x + sqrt(3)/6*cote;         //Jaune
	vertices[16] = y - 1/4*sqrt(2/3)*cote;
	vertices[17] = z + cote/2;

    //Troisième Face
    vertices[18] = x + sqrt(3)/6*cote;         //Rouge
	vertices[19] = y - 1/4*sqrt(2/3)*cote;
	vertices[20] = z - cote/2;

    vertices[21] = x;                                //Bleu
	vertices[22] = y + sqrt(6)/4*cote;
	vertices[23] = z;

    vertices[24] = x + sqrt(3)/6*cote;         //Jaune
	vertices[25] = y - 1/4*sqrt(2/3)*cote;
	vertices[26] = z + cote/2;

    //Quatrième Face
    vertices[27] = x + sqrt(3)/6*cote;         //Rouge
	vertices[28] = y - 1/4*sqrt(2/3)*cote;
	vertices[29] = z - cote/2;

    vertices[30] = x - sqrt(3)/3*cote;         //Vert
	vertices[31] = y - 1/4*sqrt(2/3)*cote;
	vertices[32] = z;

    vertices[33] = x + sqrt(3)/6*cote;         //Jaune
	vertices[34] = y - 1/4*sqrt(2/3)*cote;
	vertices[35] = z + cote/2;

   /* for(unsigned int i=0; i<36; i+=3){
        std::cout << "Sommet     :"<< i/3 +1 <<std::endl;
        std::cout << vertices[i] << std::endl;
        std::cout << vertices[i+1] << std::endl;
        std::cout << vertices[i+2] << std::endl << std::endl;
    } */
}

float* Tetraedre::colorisation(){
    colors = new float[36]; // 3 couleurs à 3 valeurs R, G, B par point

    colors[0] = 1.0f;
    colors[1] = 0.0f;
    colors[2] = 0.0f;

    colors[3] = 0.0f;
    colors[4] = 1.0f;
    colors[5] = 0.0f;

    colors[6] = 0.0f;
    colors[7] = 0.0f;
    colors[8] = 1.0f;

    colors[9] = 0.0f; // vert
    colors[10] = 1.0f;
    colors[11] = 0.0f;

    colors[12] = 0.0f; // Bleu
    colors[13] = 0.0f;
    colors[14] = 1.0f;

    colors[15] = 1.0f; // Jaune
    colors[16] = 1.0f;
    colors[17] = 0.0f;

    colors[18] = 1.0f;
    colors[19] = 0.0f;
    colors[20] = 0.0f;

    colors[21] = 0.0f; // Bleu
    colors[22] = 0.0f;
    colors[23] = 1.0f;

    colors[24] = 1.0f; // Jaune
    colors[25] = 1.0f;
    colors[26] = 0.0f;

    colors[27] = 1.0f;
    colors[28] = 0.0f;
    colors[29] = 0.0f;

    colors[30] = 0.0f; // vert
    colors[31] = 1.0f;
    colors[32] = 0.0f;

    colors[33] = 1.0f; // Jaune
    colors[34] = 1.0f;
    colors[35] = 0.0f;

    return colors;
}
