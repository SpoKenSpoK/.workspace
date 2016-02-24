#include "fumee.hpp"

Fumee::Fumee(float _xposf, float _yposf, float _zposf, float _intervalle_emission,float  _temps_ecoule, Texture* _texture)
    :xposf(_xposf), yposf(_yposf), zposf(_zposf), intervalle_emission(_intervalle_emission), temps_ecoule(_temps_ecoule), texture(_texture)
{}

void Fumee::anime(float temps){
    temps_ecoule+=temps;

    if(temps_ecoule > intervalle_emission){
        temps_ecoule=0.0f;
        Bouffee newBouffee(xposf, yposf, zposf, 5.0f);
        list_bouffee.push_back( newBouffee );
    }

    list<Bouffee>::iterator i;
    i = list_bouffee.begin();
    while( i!= list_bouffee.end() ){
        if( i->vie <= 0)
            i = list_bouffee.erase(i);
        else
            i->anime(temps);
            ++i;
    }
}

void Fumee::affiche(){
    list<Bouffee>::iterator i;
    i = list_bouffee.begin();
    while( i!= list_bouffee.end() ){
            i->affiche(texture);
            ++i;
    }
}
