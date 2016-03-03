#include <fstream>
#include <stdexcept>
#include <list>
#include <GL/glu.h>
#include <GL/glut.h>
#include "objet3d.hpp"
#include "vector3f.h"

Objet3D::Objet3D(Point* _tabPoints, Face* _tabFaces, unsigned int _nbPoints, unsigned int _nbFaces)
    :tabPoints(_tabPoints), tabFaces(_tabFaces), nbPoints(_nbPoints), nbFaces(_nbFaces)
{}

Objet3D::~Objet3D() {
        delete [] tabFaces;
        delete [] tabPoints;
}

Objet3D::Objet3D(const Objet3D & Source) {
    nbPoints = Source.nbPoints;
    nbFaces = Source.nbFaces;
    tabPoints = new Point[nbPoints];
    for(unsigned int i=0; i< nbPoints; ++i)
        tabPoints[i] = Source.tabPoints[i];

    tabFaces = new Face[nbFaces];
    for(unsigned int i=0; i< nbFaces; ++i)
        tabFaces[i] = Source.tabFaces[i];
}

void Objet3D::charger(const char* filename){
    std::ifstream file(filename, std::ios::in);

    if(file) // Test pour savoir si le fichier est bien présent dans le répertoire donné
    {
        std::string isOFF;
        file >> isOFF;
        if(isOFF != "OFF")
            throw std::runtime_error("Error: Wrong file extension - you need '.OFF'");


        file >> nbPoints >> nbFaces;
        std::cerr<<"Nom du fichier : "<<filename<<" | "<<"nbPoints : "<<nbPoints<<" - "<<"nbFaces : "<<nbFaces<<std::endl;

        tabPoints = new Point[nbPoints];    // Allocation dynamique du tableau de Point ayant pour taile le nombre de coordonnées
        tabFaces = new Face[nbFaces];      // Allocation dynamique du tableau de Face prennant comme taille le nombre de faces

        file.seekg(3, file.cur);                        // On se place à la ligne, derrière le '0'

        // ----- Lecture des coordonnées de chaque sommet ------------
        double pvalue;     // Variable temporaire servant à la lecture des doubles
        for(unsigned int i=0; i< nbPoints; ++i){
            file >> pvalue;
            tabPoints[i].x = pvalue;

            file >> pvalue;
            tabPoints[i].y = pvalue;

            file >> pvalue;
            tabPoints[i].z = pvalue;
        }

        // ----- Lecture des emplacements des coordonnées de chaque sommet pour chaque face
        unsigned int svalue;   // Variable temporaire pour la lecture des entier positifs
        for(unsigned int i=0; i< nbFaces; ++i){
            file >> svalue;
            if(svalue != 3)
                throw std::runtime_error("Error : This .OFF file doesn't contain TRIANGLES only");

            file >> svalue;
            tabFaces[i].a = svalue;

            file >> svalue;
            tabFaces[i].b = svalue;

            file >> svalue;
            tabFaces[i].c = svalue;
        }

        calcul_normal();

        file.close(); // Fermeture du fichier
    }
    else
        throw std::runtime_error("Error: Can not open file");
}


void Objet3D::affiche(){
   glBegin(GL_TRIANGLES);
        for(unsigned int i=0; i<nbFaces; ++i){
            glNormal3f(tabPoints[tabFaces[i].a].n.x, tabPoints[tabFaces[i].a].n.y, tabPoints[tabFaces[i].a].n.z);
    		glVertex3f(tabPoints[tabFaces[i].a].x, tabPoints[tabFaces[i].a].y, tabPoints[tabFaces[i].a].z);
            glNormal3f(tabPoints[tabFaces[i].b].n.x, tabPoints[tabFaces[i].b].n.y, tabPoints[tabFaces[i].b].n.z);
    		glVertex3f(tabPoints[tabFaces[i].b].x, tabPoints[tabFaces[i].b].y, tabPoints[tabFaces[i].b].z);
            glNormal3f(tabPoints[tabFaces[i].c].n.x, tabPoints[tabFaces[i].c].n.y, tabPoints[tabFaces[i].c].n.z);
    		glVertex3f(tabPoints[tabFaces[i].c].x, tabPoints[tabFaces[i].c].y, tabPoints[tabFaces[i].c].z);
        }
   glEnd();
}


void Objet3D::interpolation(Objet3D *debut, Objet3D *fin, float t){

	t -= (int)t; // permet de limiter t de 0 à 1

	nbFaces = debut->get_nbFaces();
	nbPoints = debut->get_nbPoints();

	tabPoints = new Point[nbPoints];
	tabFaces = new Face[nbFaces];

	for(unsigned int i=0; i<nbFaces; ++i)
		tabFaces[i] = debut->tabFaces[i]; //recopie le tableau de faces car les faces possedent les meme points.

	for(unsigned int i=0; i<nbPoints; ++i){ //interpolation des points
		tabPoints[i].x = debut->tabPoints[i].x + t*(fin->tabPoints[i].x - debut->tabPoints[i].x);
		tabPoints[i].y = debut->tabPoints[i].y + t*(fin->tabPoints[i].y - debut->tabPoints[i].y);
		tabPoints[i].z = debut->tabPoints[i].z + t*(fin->tabPoints[i].z - debut->tabPoints[i].z);
        //tabPoints[i].n = debut->tabPoints[i].n;
	}

    calcul_normal();
}

void Objet3D::calcul_normal_faces(){
    for(unsigned int j=0; j<nbFaces; ++j){
        float abx = tabPoints[tabFaces[j].b].x - tabPoints[tabFaces[j].a].x;
        float aby = tabPoints[tabFaces[j].b].y - tabPoints[tabFaces[j].a].y;
        float abz = tabPoints[tabFaces[j].b].z - tabPoints[tabFaces[j].a].z;
        Vector3f v1(abx, aby, abz);
        float acx = tabPoints[tabFaces[j].c].x - tabPoints[tabFaces[j].a].x;
        float acy = tabPoints[tabFaces[j].c].y - tabPoints[tabFaces[j].a].y;
        float acz = tabPoints[tabFaces[j].c].z - tabPoints[tabFaces[j].a].z;
        Vector3f v2(acx, acy, acz);

        Vector3f n = v1 ^ v2;
        tabFaces[j].n = n;
    }
}

void Objet3D::calcul_normal(){

    calcul_normal_faces();

    unsigned int nb_normales_par_sommet[nbPoints];

    for(unsigned int i=0; i<nbPoints; ++i){
        tabPoints[i].n = Vector3f(0,0,0);
        nb_normales_par_sommet[i]=0;
    }

    unsigned int a,b,c;

    for(unsigned int i=0; i<nbFaces; ++i){

        a = tabFaces[i].a;
        nb_normales_par_sommet[a]++;
        tabPoints[a].n += tabFaces[i].n;

        b = tabFaces[i].b;
        nb_normales_par_sommet[b]++;
        tabPoints[b].n += tabFaces[i].n;

        c = tabFaces[i].c;
        nb_normales_par_sommet[c]++;
        tabPoints[c].n += tabFaces[i].n;
    }

    for(unsigned int i=0; i<nbPoints; ++i){
        tabPoints[i].n /= nb_normales_par_sommet[i];
        tabPoints[i].n.normalize();
    }
}
