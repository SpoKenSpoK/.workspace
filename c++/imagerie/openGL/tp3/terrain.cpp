///////////////////////////////////////////////////////////////////////////////
// IUT d'Aix-Marseille Universit�, site d'Arles
// D�partement Informatique
// 2�me ann�e
// ----------------------------------------------------------------------------
// Auteur :
// ----------------------------------------------------------------------------
// Classe d�finissant un terrain 3D.
///////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "terrain.h"
#include "pgm.h"



///////////////////////////////////////////////////////////////////////////////
// Constructeur de la classe Terrain.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
//
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Terrain::Terrain()
{
	points_terrain = NULL;
}



///////////////////////////////////////////////////////////////////////////////
// Destructeur de la classe Terrain.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
//
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Terrain::~Terrain()
{



}



///////////////////////////////////////////////////////////////////////////////
// Affichage du terrain.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
//
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void Terrain::affiche()
{
	//    On �tudie les points 4 par 4, en les utilisant pour tracer
	//    deux triangles comme suit :
	//
	//         1---2      1er  triangle : 0 -> 1 -> 3
	//	       |  /|      2�me triangle : 1 -> 2 -> 3
	//         | / |
	//         4/--3
	//
	texture.utiliser();

	glBegin(GL_TRIANGLES);

	for(int z=0; z < nb_pt_z-1; ++z)
		for(int x=0; x < nb_pt_x-1; ++x){
			int indice = x + z * nb_pt_x;
			Point_terrain p1, p2, p3, p4;
			p1 = points_terrain[indice];
			p2 = points_terrain[indice + 1];
			p3 = points_terrain[indice + nb_pt_x + 1];
			p4 = points_terrain[indice + nb_pt_x];

			Vector3f sideOne( p2.x - p1.x, p2.hauteur - p1.hauteur, p2.z - p1.z );
			Vector3f sideTwo( p4.x - p1.x, p4.hauteur - p1.hauteur, p4.z - p1.z );

			Vector3f normal = (sideTwo ^ sideOne);
			normal.normalize();
			glNormal3f(normal.x, normal.y, normal.z);
			glTexCoord2f(p1.s,p1.t);
			glVertex3f(p1.x, p1.hauteur, p1.z);
			glTexCoord2f(p4.s,p4.t);
			glVertex3f(p4.x, p4.hauteur, p4.z);
			glTexCoord2f(p2.s,p2.t);
			glVertex3f(p2.x, p2.hauteur, p2.z);

			Vector3f sideThree( p2.x - p3.x, p2.hauteur - p3.hauteur, p2.z - p3.z );
			Vector3f sideFour( p4.x - p3.x, p4.hauteur - p3.hauteur, p4.z - p3.z );

			normal = (sideThree ^ sideFour);
			normal.normalize();
			glNormal3f(normal.x, normal.y, normal.z);
			glTexCoord2f(p2.s,p2.t);
			glVertex3f(p2.x, p2.hauteur, p2.z);
			glTexCoord2f(p4.s,p4.t);
			glVertex3f(p4.x, p4.hauteur, p4.z);
			glTexCoord2f(p3.s,p3.t);
			glVertex3f(p3.x, p3.hauteur, p3.z);
		}

	glEnd();
}

void Terrain::charger(const char* filename){
	texture.charger(filename,true);
	texture.definir_filtrage(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	texture.definir_bouclage(GL_REPEAT, GL_REPEAT);
	texture.definir_melange(GL_MODULATE);
}

void Terrain::calcule_coordonnees_texture(){
	for(int z=0; z < nb_pt_z-1; ++z)
		for(int x=0; x < nb_pt_x-1; ++x){
			int indice = x + z * nb_pt_x;

			points_terrain[indice].s = points_terrain[indice].x/nb_pt_x;
			points_terrain[indice].t = points_terrain[indice].z/nb_pt_z;

			points_terrain[indice + 1].s = points_terrain[indice + 1].x/nb_pt_x;
			points_terrain[indice + 1].t = points_terrain[indice + 1].z/nb_pt_z;

			points_terrain[indice + nb_pt_x + 1].s = points_terrain[indice + nb_pt_x + 1].x/nb_pt_x;
			points_terrain[indice + nb_pt_x + 1].t = points_terrain[indice + nb_pt_x + 1].z/nb_pt_z;

			points_terrain[indice + nb_pt_x].s = points_terrain[indice + nb_pt_x].x/nb_pt_x;
			points_terrain[indice + nb_pt_x].t = points_terrain[indice + nb_pt_x].z/nb_pt_z;
		}
}

void Terrain::calcule_normales(){
	for(int z=0; z < nb_pt_z-1; ++z)
		for(int x=0; x < nb_pt_x-1; ++x){
			int indice = x + z * nb_pt_x;

			Point_terrain p1, p2, p3, p4;
			p1 = points_terrain[indice];
			p2 = points_terrain[indice + 1];
			p3 = points_terrain[indice + nb_pt_x + 1];
			p4 = points_terrain[indice + nb_pt_x];

			Vector3f sideOne( p2.x - p1.x, p2.hauteur - p1.hauteur, p2.z - p1.z );
			Vector3f sideTwo( p4.x - p1.x, p4.hauteur - p1.hauteur, p4.z - p1.z );
			Vector3f normal = (sideTwo ^ sideOne);







			Vector3f sideThree( p2.x - p3.x, p2.hauteur - p3.hauteur, p2.z - p3.z );
			Vector3f sideFour( p4.x - p3.x, p4.hauteur - p3.hauteur, p4.z - p3.z );
			normal = (sideThree ^ sideFour);


		}
}


///////////////////////////////////////////////////////////////////////////////
// Cr�ation du terrain � partir d'une image.
//-----------------------------------------------------------------------------
// Param�tres :
//    cx, cy, cz     (in) : coefficients d'agrandissement selon les 3 axes.
//    image_hauteurs (in) : image au format pgm dont les niveaux de gris
//                          d�finissent les hauteurs d'un terrain.
//
// Retour :
//    true si le terrain a pu �tre cr�e (l'image a pu �tre charg�e),
//    false sinon.
///////////////////////////////////////////////////////////////////////////////
bool Terrain::creation(	float dx, float dy, float dz, const char *image_hauteurs )
{
	// Tableau de byte servant � stocker les pixels en niveaux de gris de
	// l'image au format pgm "image_hauteurs".
	byte	*img;

	// Chargement d'une image en niveaux de gris. Chaque pixel correspondra
	// � un point du terrain.
	img = load_PGM( image_hauteurs, &nb_pt_x, &nb_pt_z );

	// Si l'image n'a pas pu �tre charg�e, alors on quitte la fonction.
	if( img == false )
		return false;

	dist_x = dx;
	dist_y = dy;
	dist_z = dz;

	if( points_terrain )
		delete[] points_terrain;

	points_terrain = new Point_terrain[nb_pt_x * nb_pt_z];

	int x, z, num = 0;

	for( z=0; z<nb_pt_z; z++ )
	{
		for( x=0; x<nb_pt_x; x++ )
		{
			points_terrain[num].x = x * dist_x;
			points_terrain[num].z = z * dist_z;
			points_terrain[num].hauteur = img[num] * dist_y;

			num++;
		}
	}

	delete[] img;

	terrain_ambient[0]=0.6f;  terrain_ambient[1]=0.6f;  terrain_ambient[2]=0.6f;  terrain_ambient[3]=1.0f;
	terrain_diffuse[0]=0.9f;  terrain_diffuse[1]=0.9f;  terrain_diffuse[2]=0.9f;  terrain_diffuse[3]=1.0f;
	terrain_specular[0]=0.0f; terrain_specular[1]=0.0f; terrain_specular[2]=0.0f; terrain_specular[3]=1.0f;
	terrain_shininess[0]=1.0f;

	return true;
}
