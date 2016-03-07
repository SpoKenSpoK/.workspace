#ifndef TETRAEDRE_HPP
#define TETRAEDRE_HPP

#include "../tp1/objetGeometrique.hpp"

class Tetraedre : public objetGeometrique {
private:
    float cote, x, y, z;
    float* colors;

public:
    Tetraedre(float =0.5f, float =0.0f, float =0.0f, float =0.0f, float* =0 );
    ~Tetraedre();
    virtual void calculPosition_sommets();
    float* colorisation();

};

#endif
