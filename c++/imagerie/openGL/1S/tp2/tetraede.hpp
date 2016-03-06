#ifndef TETRAEDE_HPP
#define TETRAEDE_HPP

#include "../tp1/objetGeometrique.hpp"

class Tetraede : public objetGeometrique {
private:
    float cote;
    float* colors;

public:
    Tetraede(float =0.5);
    ~Tetraede();
    virtual void calculPosition_sommets();
    float* colorisation();

};

#endif
