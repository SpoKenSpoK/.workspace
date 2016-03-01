#include <iostream>
#include "cercle.hpp"
#include "math.h"

Cercle::Cercle(unsigned int _nbPts, float cx, float cy, float r)
    :objetGeometrique(_nbPts), centreX(cx), centreY(cy), rayon(r)
{}

Cercle::~Cercle(){}

void Cercle::calculPosition_sommets(){
    vertices = new float [this->getNbP()*3];
    float pas = 2*M_PI / this->getNbP();
    float pastemp = pas;

    for(unsigned int i=0; i < unsigned (getNbP()*3); i+=3){
        vertices[i] = centreX + (rayon*cos(pastemp));
        vertices[i+1] = centreY + (rayon*sin(pastemp));
        vertices[i+2] = 0.0f;

        pastemp += pas;

        /*std::cout << vertices[i] << std::endl;
        std::cout << vertices[i+1] << std::endl;
        std::cout << vertices[i+2] << std::endl;
        std::cout << pastemp << std::endl;
        std::cout << " ___  " << std::endl; */
    }
}
