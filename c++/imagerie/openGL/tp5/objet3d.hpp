#ifndef OBJET3D_HPP
#define OBJET3D_HPP

struct Point{
    float x; // Coordonnées du sommet (point) dans l'espace
    float y;
    float z;
};

struct Face{
    unsigned int a; // Indice de la ligne à lire pour retrouver les coordonnées du premier sommet de la face
    unsigned int b;
    unsigned int c;
};

class Objet3D {
private:
    Point* tabPoints;
    Face* tabFaces;
    unsigned int nbPoints;
    unsigned int nbFaces;

public:
    Objet3D(Point* =0, Face* =0, unsigned int =0, unsigned int =0);
    ~Objet3D();
    void charger(const char* );
    void affiche();
    void interpolation(Objet3D* , Objet3D* , float );

    inline unsigned int get_nbFaces() const { return nbFaces; }
    inline unsigned int get_nbPoints() const { return nbPoints; }
    inline void set_nbFaces(const unsigned int& nbf) { nbFaces = nbf; }
    inline void set_nbPoints(const unsigned int& nbp) { nbPoints = nbp; }
};

#endif //OBJET3D_HPP
