//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

#include "objetsOFF.h"

void normalize(struct point3D* _v) {
float length = sqrtf(produit_scalaire(_v, _v));
	_v->x /= length;
	_v->y /= length;
	_v->z /= length;
}

float produit_scalaire(const struct point3D * _v1, const struct point3D * _v2) {
	float res;

	res = _v1 -> x * _v2->x + _v1->y * _v2->y + _v1->z * _v2->z;

	return res;
}

struct point3D produit_vectoriel(const struct point3D * _v1 , const struct point3D * _v2) {

	struct point3D res;
	res.x = _v1->y * _v2->z - _v1->z * _v2->y;
	res.y = _v1->z * _v2->x - _v1->x * _v2->z;
	res.z = _v1->x * _v2->y - _v1->y * _v2->x;
	return res;

}


struct point3D VecteurFromPoints(const struct point3D * _p1, const struct point3D * _p2) {
	struct point3D res;

	res.x = _p2->x - _p1->x;
	res.y = _p2->y - _p1->y;
	res.z = _p2->z - _p1->z;

	return res;
}

Objet::Objet() {
	strcpy(nom, "nil");
	nbsommets=0;
	nbfaces=0;
	id=0;
}

Objet::~Objet() {
	delete [] lpoints;
	delete [] lfaces;
}

void Objet::calcul_normales_points() {

lpointsnormales = new struct point3D[nbsommets];

for (unsigned int s = 0; s < nbsommets; s++) {
	std::cerr << "calcul normales points, traitement de " << s << "/" << nbsommets << std::endl;
	//on cherche pour chaque sommets la liste des faces auquelles il appartient
	unsigned int tab_faces[20];
	unsigned int cpt_faces=0;

	for (unsigned int f = 0; f < nbfaces ; f++) {
		if ( (lfaces[f].S1 == s) ||  (lfaces[f].S2 == s) ||  (lfaces[f].S3 == s)) {
			tab_faces[cpt_faces] = f;
			cpt_faces++;
		}
	}

	//on calcule ensuite pour ce sommet la moyenne des normales des faces
	struct point3D moyenne;
	moyenne.x = 0.0f;
	moyenne.y = 0.0f;
	moyenne.z = 0.0f;

	for (unsigned int sf = 0; sf < cpt_faces ; cpt_faces++) {
		moyenne.x += lfacesnormales[tab_faces[sf]].x;
		moyenne.y += lfacesnormales[tab_faces[sf]].y;
		moyenne.z += lfacesnormales[tab_faces[sf]].z;
	}

	//affectation de la normale
	lpointsnormales[s].x = moyenne.x / float(cpt_faces);
	lpointsnormales[s].y = moyenne.y / float(cpt_faces);
	lpointsnormales[s].z = moyenne.z / float(cpt_faces);
	}

}


void Objet::calcul_normales_faces() {
struct point3D v1;
struct point3D v2;

lfacesnormales = new struct point3D[nbfaces];

for (unsigned int i = 0; i < nbfaces; i++) {

	v1 = VecteurFromPoints(&lpoints[lfaces[i].S1], &lpoints[lfaces[i].S2]);
	v2 = VecteurFromPoints(&lpoints[lfaces[i].S1], &lpoints[lfaces[i].S3]);

	lfacesnormales [i] = produit_vectoriel(&v2, &v1);
	normalize(&lfacesnormales [i]);
	}
std::cerr << "Fin calcul normales faces, traitement de " << nbfaces << " faces" << std::endl;
}

void Objet::charge_OFF(const char nom_fichier[40])
{
unsigned int i;
FILE * fichier;
unsigned int n;

char magic[10];

if ((fichier=fopen(nom_fichier,"ro"))==NULL)
        {
		perror("Erreur a l'ouverture du fichier OFF");
        	exit(1);
        }

fscanf(fichier,"%s\n",magic);

if (strcmp(magic, "OFF")!=0) {
	perror("Erreur dans l'en tête du fichier OFF");
        exit(0);
}

fscanf(fichier,"%u %u %u\n",&nbsommets, &nbfaces, &i);

//Allocation des listes de données
lfaces=new struct indexedface[nbfaces];
lpoints=new struct point3D[nbsommets];

//Remplissage de la liste de points
for (i=0;i<nbsommets;i++)
{
    fscanf(fichier,"%lf %lf %lf\n",&lpoints[i].x,&lpoints[i].y,&lpoints[i].z);
	if (min.x>lpoints[i].x)
		min.x=lpoints[i].x;
	if (min.y>lpoints[i].y)
		min.y=lpoints[i].y;
	if (min.z>lpoints[i].z)
		min.z=lpoints[i].z;

	if (max.x<lpoints[i].x)
		max.x=lpoints[i].x;
	if (max.y<lpoints[i].y)
		max.y=lpoints[i].y;
	if (max.z<lpoints[i].z)
		max.z=lpoints[i].z;
}

//Remplissage de la liste de faces
for (i=0;i<nbfaces;i++)
{
   fscanf(fichier,"%u %u %u %u\n",&n, &lfaces[i].S1, &lfaces[i].S2,&lfaces[i].S3);
	if (n!=3) {
		perror("Erreur au chargement des faces, non triangulaires");
		exit(1);
		}
}


//calcul du centroid de l'objet à partir de la boîte min/max
centroid.x = (max.x + min.x) / 2.0f;
centroid.y = (max.y + min.y) / 2.0f;
centroid.z = (max.z + min.z) / 2.0f;

std::cerr << "Chargement du fichier " << nom_fichier << std::endl;
std::cerr << "\t" << nom_fichier << " : " << nbsommets << " points - " << nbfaces << " arêtes" << std::endl;
std::cerr << "\t" << nom_fichier << " MAX : " << max.x << " " << max.y << " " << max.z << std::endl;
std::cerr << "\t" << nom_fichier << " MIN : " << min.x << " " << min.y << " " << min.z << std::endl;
std::cerr << "\t" << nom_fichier << " CENTROID : " << centroid.x << " " << centroid.y << " " << centroid.z << std::endl;

calcul_normales_faces();

//calcul_normales_points();

fclose(fichier);
}

void Objet::affiche()
{
    unsigned int i;
    fprintf(stderr,"Objet %s nbsommets %u nbfaces %u\n",nom, nbsommets, nbfaces);

	fprintf(stderr,"\n SOMMETS\n");
		for (i=0;i<nbsommets;i++)
			fprintf(stderr,"Sommets %u [%.2f %.2f %.2f]\n",i,lpoints[i].x,lpoints[i].y,lpoints[i].z);

	fprintf(stderr,"\n FACES\n");
		for (i=0;i<nbfaces;i++)
    			fprintf(stderr,"Face %u [%u %u %u]\n",i, lfaces[i].S1, lfaces[i].S2, lfaces[i].S3);
}
