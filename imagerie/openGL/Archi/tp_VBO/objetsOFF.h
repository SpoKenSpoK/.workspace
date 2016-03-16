//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

#ifndef _H_OBJETSOFF
#define _H_OBJETSOFF

#include <errno.h>
#include <unistd.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <cmath>

#include <iostream>

struct indexedface {unsigned int S1,S2,S3;};

struct point3D {
		double x;
		double y;
		double z;
		};

class Objet {

  public :
	char nom[40];

	unsigned int nbsommets;
	unsigned int nbfaces;

	unsigned int id;

	struct point3D *lpoints;
	struct indexedface *lfaces;
	struct point3D * lpointsnormales;
	struct point3D * lfacesnormales;

	struct point3D min;
	struct point3D max;
	struct point3D centroid;

	Objet();
	Objet(const Objet&);
	~Objet();

	void charge_OFF(const char nom_fichier[40]);
	void calcul_normales_points();
	void calcul_normales_faces();

	void affiche();

};

//fonctions sur ces objets
void normalize(const struct point3D*);
float produit_scalaire(const struct point3D * , const struct point3D * );
struct point3D produit_vectoriel(const struct point3D * , const struct point3D * );

#endif


