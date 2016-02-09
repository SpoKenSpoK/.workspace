///////////////////////////////////////////////////////////////////////////////
// IUT d'Aix-Marseille Universit�, site d'Arles
// D�partement Informatique
// 2�me ann�e
// ----------------------------------------------------------------------------
// IN - Synth�se d'images - Rendu r�aliste
// Auteur : S�bastien Thon
// ----------------------------------------------------------------------------
// Base du TP 1
// Base d'un programme permettant d'afficher une sc�ne compos�e d'un sol, de
// maisons et d'arbres. On placera des sources de lumi�re, on sp�cifiera les
// mat�riaux des diff�rents objets et on utilisera un effet de brouillard.
// ----------------------------------------------------------------------------
// Compilation sous Windows :
//   g++ -Wall maison.cpp -o maison.exe -lglut32 -lglu32 -lopengl32 -L.
// Compilation sous Linux :
//   g++ -Wall maison.cpp -o maison.exe -lglut -lGLU -lGL -L.
// Compilation sous MacOS :
//   g++ -framework GLUT -framework OpenGL maison.cpp -o maison.exe
///////////////////////////////////////////////////////////////////////////////
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include "glut.h"
#include "stb_image.h"

#define WIDTH   800										// Largeur de la fen�tre OpenGL
#define HEIGHT  600										// Hauteur de la fen�tre OpenGl

#define KEY_ESC 27										// Code ASCII de la touche Echap

float	xpos = 0, zpos = -50;							// Position de la cam�ra
int		angle_x = 45, angle_y = 45;						// Orientation de la cam�ra
int		mouse_x = 0, mouse_y = 0;						// Position de la souris
float 	xlight = 5.0, ylight = 20.0, zlight = 15.0;

class Texture {
public:
	unsigned char* img;
	GLuint id;
	int tWidth;
	int tHeight;
	int opp;

	inline ~Texture() { delete [] img; }
	bool charger(const char* );
	void utiliser();
	void definir_filtrage(GLint , GLint);
	void definir_bouclage(GLint , GLint);
	void definir_melange(GLint );
};

// Définition des méthodes de la classe Texture

bool Texture::charger(const char* file_name){
	img = stbi_load(file_name, &tWidth, &tHeight, &opp, 0);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	if( img!= NULL){
		switch (opp){
			case 3:
				glTexImage2D( GL_TEXTURE_2D, 0, opp, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
				return true;
				break;

			case 4:
				glTexImage2D( GL_TEXTURE_2D, 0, opp, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
				return true;
				break;

			default:
				return false;
		}
	}
	else return false;
}

void Texture::utiliser() { glBindTexture(GL_TEXTURE_2D, id); }
void Texture::definir_filtrage(GLint mode_min, GLint mode_mag){
	utiliser();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode_mag);
}

void Texture::definir_bouclage(GLint mode_axe_s, GLint mode_axe_t){
	utiliser();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode_axe_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode_axe_t);
}

void Texture::definir_melange(GLint melange){
	utiliser();

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, melange);
}

//*****************************************************************
//* A FAIRE :
//* D�clarer des tableaux contenant les caract�ristiques (Ambient,
//* diffus, sp�culaire) pour diff�rents mat�riaux :
//*    - sol
//*    - mur de maison
//*    - toit de maison
//*    - tronc d'arbre
//*    - feuillage d'arbre
//*
//* Par exemple, pour le mat�riau de sol:
//*   GLfloat MatAmbient_sol[] = {0.0f, 0.5f, 0.0f, 1.0f};	// On utilise des valeurs faibles pour Ambient (sombre, car � l'ombre)
//*   GLfloat MatDiffuse_sol[] = {0.0f, 0.7f, 0.0f, 1.0f};	// On utilise des valeurs plus �lev�es pour Diffuse (partie �clair�e)
//*   GLfloat MatSpecular_sol[]= {0.1f, 0.1f, 0.1f, 1.0f};	// Plus ces valeurs seront �lev�es, et plus le mat�riau sera luisant
//*   GLfloat MatShininess_sol[1]={5.0f};					// Plus cette valeurs era �lev�e (max=128) et plus le reflet sera petit
//*****************************************************************



///////////////////////////////////////////////////////////////////////////////
// Intitialisation de certains param�tres d'OpenGL.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
Texture texture_sol;
Texture texture_mur;
Texture texture_facade;
Texture texture_toit;

GLvoid initGL()
{
	glClearColor(0, 0.50, 0.80, 1);							// Couleur servant � effacer la fen�tre (noir)
    glShadeModel(GL_SMOOTH);							// Mod�le d'ombrage : lissage de Gouraud
	glEnable(GL_CULL_FACE);								// Ne traite pas les faces cach�es
	glEnable(GL_DEPTH_TEST);							// Active le Z-Buffer
	glDepthFunc(GL_LEQUAL);								// Mode de fonctionnement du Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)

	//*****************************************************************
	//* A FAIRE :
	//*
	//* 1. Cr�er la source de lumi�re 0 (GL_LIGHT0) en tant que source de
	//*    lumi�re directionnelle, en pr�cisant :
	//*      - sa position
	//*      - sa couleur ambiante
	//*      - sa couleur diffuse
	//*      - sa couleur sp�culaire
	//*
	//* 2. Activer la source de lumi�re 0.
	//*
	//* 3. Activer l'�clairage g�n�ral
	//*****************************************************************

	// INITIALISATION DES TEXTURES :

	texture_sol.charger("sol.png");
	texture_sol.definir_filtrage(GL_LINEAR, GL_LINEAR);
	texture_sol.definir_bouclage(GL_REPEAT, GL_REPEAT);
	texture_sol.definir_melange(GL_MODULATE);

	texture_mur.charger("mur.png");
	texture_mur.definir_filtrage(GL_LINEAR, GL_LINEAR);
	texture_mur.definir_bouclage(GL_REPEAT, GL_REPEAT);
	texture_mur.definir_melange(GL_MODULATE);

	texture_facade.charger("facade.png");
	texture_facade.definir_filtrage(GL_LINEAR, GL_LINEAR);
	texture_facade.definir_bouclage(GL_REPEAT, GL_REPEAT);
	texture_facade.definir_melange(GL_MODULATE);

	texture_toit.charger("toit.png");
	texture_toit.definir_filtrage(GL_LINEAR, GL_LINEAR);
	texture_toit.definir_bouclage(GL_REPEAT, GL_REPEAT);
	texture_toit.definir_melange(GL_MODULATE);

   // Lumière
	GLfloat Light0Amb[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat Light0Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light0Spec[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat Light0Pos[4] = {5.0f, 20.0f, 10.0f, 0.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);

	// Fixe la position de la lumière 0
	glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);

	GLfloat Light1Amb[4] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat Light1Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light1Spec[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat Light1Pos[4] = {xlight, ylight, zlight, 1.0f};

	glLightfv(GL_LIGHT1, GL_AMBIENT, Light1Amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Light1Spec);

	// Fixe la position de la lumière 1
	glLightfv(GL_LIGHT1, GL_POSITION, Light1Pos);

	// Active la lumière 0 & 1
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Active l’éclairage
	glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// Affiche le sol de la sc�ne.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_sol()
{
	texture_sol.utiliser();

	glPushMatrix();
	//*****************************************************************
	//* A FAIRE :
	//* Remplacer le glColor3f() suivant par l'utilisation du mat�riau
	//* cr�e pour le sol (utiliser glMaterialfv() pour les param�tres
	//* ambiant, diffus, speculaire du mat�riau du sol).
	//*****************************************************************

	GLfloat MatAmbient[4] = {0.0f, 0.4f, 0.0f, 1.0f};
	GLfloat MatDiffuse[4] = {0.0f, 0.8f, 0.0f, 1.0f};
	GLfloat MatSpecular[4] = {0.2f,0.2f, 0.2f, 1.0f};
	GLfloat MatShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess);

	//glColor3f(0.0f,0.8f,0.0f);							// Couleur courante : vert

	glBegin(GL_QUADS); // Affichage d'un quadrilat�re
		glTexCoord2f(0.0,0.0);
		glVertex3d(-50, 0, -50);
		glTexCoord2f(0.0,1.0);
		glVertex3d(-50, 0,  50);
		glTexCoord2f(1.0,1.0);
		glVertex3d( 50, 0,  50);
		glTexCoord2f(1.0,0.0);
		glVertex3d( 50, 0, -50);
	glEnd();

	glPopMatrix();

}

void affiche_repere(){
	glPushMatrix();
	glLineWidth(3);

	GLfloat XAmbient[4] = {0.5f, 0.0f, 0.0f, 1.0f};
	GLfloat XDiffuse[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat XSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat XShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, XAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, XDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, XSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, XShininess);

	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(100, 0, 0);
	glEnd();


	GLfloat YAmbient[4] = {0.0f, 0.5f, 0.0f, 1.0f};
	GLfloat YDiffuse[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat YSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat YShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, YAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, YDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, YSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, YShininess);

	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0, 100, 0);
	glEnd();


	GLfloat ZAmbient[4] = {0.0f, 0.0f, 0.5f, 1.0f};
	GLfloat ZDiffuse[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	GLfloat ZSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat ZShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ZAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ZDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ZSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, ZShininess);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0, 0, 100);
	glEnd();

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// Affiche une maison.
//-----------------------------------------------------------------------------
// Param�tres :
//    xp,yp,zp (in) : position de la maison.
//    yr       (in) : angle de rotation (en degr�s) de la maison p/r � l'axe y.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_maison( float xp, float yp, float zp, float yr )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glTranslatef(xp, yp, zp);							// Positionne la maison avec une translation
	glRotatef(yr, 0,1,0);								// et une rotation
	//*****************************************************************
	//* A FAIRE :
	//* Associer une normale � chacun des polygones suivants. Pour cela
	//* faire appel � glNormal3f() avant chacun des "glBegin(GL_QUADS)"
	//* et des "glBegin(GL_TRIANGLES)".
	//*****************************************************************

	//*****************************************************************
	//* A FAIRE :
	//* Remplacer le glColor3f() suivant par l'utilisation du mat�riau
	//* cr�e pour les murs (utiliser glMaterialfv() pour les param�tres
	//* ambiant, diffus, speculaire du mat�riau du mur).
	//*****************************************************************

	GLfloat MatAmbient[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat MatDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat MatSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat MatShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess);

	//glColor3f(1.0f,1.0f,1.0f);			// Couleur courante : blanc

	texture_facade.utiliser();

	// Mur de face
	glNormal3f(0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d(-4, 5, 5);
		glTexCoord2f(0.0,1.0);
		glVertex3d(-4, 0, 5);
		glTexCoord2f(1.0,1.0);
		glVertex3d( 4, 0, 5);
		glTexCoord2f(1.0,0.0);
		glVertex3d( 4, 5, 5);
	glEnd();

	texture_mur.utiliser();
	// Mur arri�re
	glNormal3f(0.0f,0.0f,-1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d( 4, 5, -5);
		glTexCoord2f(0.0,1.0);
		glVertex3d( 4, 0, -5);
		glTexCoord2f(1.0,1.0);
		glVertex3d(-4, 0, -5);
		glTexCoord2f(1.0,0.0);
		glVertex3d(-4, 5, -5);
	glEnd();

	// Mur gauche
	glNormal3f(-1.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d(-4, 5, -5);
		glTexCoord2f(0.0,1.0);

		glVertex3d(-4, 0, -5);
		glTexCoord2f(1.0,1.0);
		glVertex3d(-4, 0,  5);
		glTexCoord2f(1.0,0.0);
		glVertex3d(-4, 5,  5);
	glEnd();

	// Mur droit
	glNormal3f(1.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d(4, 5,  5);
		glTexCoord2f(0.0,1.0);
		glVertex3d(4, 0,  5);
		glTexCoord2f(1.0,1.0);
		glVertex3d(4, 0, -5);
		glTexCoord2f(1.0,0.0);
		glVertex3d(4, 5, -5);
	glEnd();

	// Triangle avant
	glNormal3f(0.0f,0.0f,1.0f);
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.5,0.0);
		glVertex3d( 0, 9, 5);
		glTexCoord2f(0.0,0.5);
		glVertex3d(-4, 5, 5);
		glTexCoord2f(1.0,0.5);
		glVertex3d( 4, 5, 5);
	glEnd();

	// Triangle arri�re
	glNormal3f(0.0f,0.0f,-1.0f);
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.5,0.0);
		glVertex3d( 0, 9, -5);
		glTexCoord2f(0.0,0.5);
		glVertex3d( 4, 5, -5);
		glTexCoord2f(1.0,0.5);
		glVertex3d(-4, 5, -5);
	glEnd();

//*****************************************************************
//* A FAIRE :
//* Remplacer le glColor3f() suivant par l'utilisation du mat�riau
//* cr�e pour le toit (utiliser glMaterialfv() pour les param�tres
//* ambiant, diffus, speculaire du mat�riau du toit).
//*****************************************************************

	GLfloat ToitAmbient[4] = {0.5f, 0.0f, 0.0f, 1.0f};
	GLfloat ToitDiffuse[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	GLfloat ToitSpecular[4] = {0.2f,0.2f, 0.2f, 1.0f};
	GLfloat ToitShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ToitAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ToitDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ToitSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, ToitShininess);

	glColor3f(1.0f,0.0f,0.0f);			// Couleur courante : rouge

	texture_toit.utiliser();

	// Toit versant droit
	glNormal3f(0.707f,0.707f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d(0, 9,  5);
		glTexCoord2f(0.0,1.0);
		glVertex3d(4, 5,  5);
		glTexCoord2f(1.0,1.0);
		glVertex3d(4, 5, -5);
		glTexCoord2f(1.0,0.0);
		glVertex3d(0, 9, -5);
	glEnd();

	// Toit versant gauche
	glNormal3f(-0.707f,0.707f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3d( 0, 9, -5);
		glTexCoord2f(0.0,1.0);
		glVertex3d(-4, 5, -5);
		glTexCoord2f(1.0,1.0);
		glVertex3d(-4, 5,  5);
		glTexCoord2f(1.0,0.0);
		glVertex3d( 0, 9,  5);
	glEnd();


	glPopMatrix();										// Restaure la matrice de vue
}

///////////////////////////////////////////////////////////////////////////////
// Affiche un arbre.
//-----------------------------------------------------------------------------
// Param�tres :
//    xp,yp,zp (in) : position de l'arbre.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_arbre( float xp, float yp, float zp )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glRotatef(-90, 1,0,0);
	glTranslatef(xp, yp, zp);							// Positionne l'arbre avec une translation

	//*********************Light1Pos********************************************
	//* A FAIRE :
	//* Afficher un arbre dont le tronc sera un c�ne :
	//*     glutSolidCone()
	//* et le feuillage une sph�re :
	//*     glutSolidSphere()
	//* Pr�ciser le mat�riau pour chacun de ces �l�ments.
	//*****************************************************************

	GLfloat ConeAmbient[4] = {0.15f, 0.04f, 0.015f, 1.0f};
	GLfloat ConeDiffuse[4] = {0.3f, 0.08f, 0.035f, 1.0f};
	GLfloat ConeSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat ConeShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ConeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ConeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ConeSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, ConeShininess);

	glutSolidCone(1,6,30,30);

	glTranslatef(0, 0, 6);
	GLfloat SphereAmbient[4] = {0.15f, 0.04f, 0.015f, 1.0f};
	GLfloat SphereDiffuse[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	GLfloat SphereSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
	GLfloat SphereShininess[] = { 5.0F };

	glMaterialfv(GL_FRONT, GL_AMBIENT, SphereAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, SphereDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SphereSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, SphereShininess);

	glutSolidSphere(3,50,50);

	glPopMatrix();										// Restaure la matrice de vue
}


///////////////////////////////////////////////////////////////////////////////
// Affiche les diff�rents objets de la sc�ne.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void affiche_scene()
{
	//affiche_repere();

	glEnable(GL_TEXTURE_2D);
	affiche_sol();								// On affiche le sol.

	//*****************************************************************
	//* A FAIRE :
	//* Afficher quelques maisons et quelques arbres.
	//*
	//* Par exemple, on affiche ici une maison en (0,0,0),
	//* tourn�e de 20� :
	//*****************************************************************
	affiche_maison( -10, 0, 10, 0 );
	affiche_maison( -10, 0, -10, 0 );
	affiche_maison( 10, 0, 10, 0 );
	affiche_maison( 10, 0, -10, 0 );

	glDisable(GL_TEXTURE_2D);

	affiche_arbre( 0, 10, 0);
	affiche_arbre( 0, -10, 0);
	affiche_arbre( -10, 0, 0);
	affiche_arbre( 10, 0, 0);

	glutSwapBuffers();							// Affiche la sc�ne � l'�cran (affichage en double buffer)
}




///////////////////////////////////////////////////////////////////////////////
// Call-back : affichage dans la fen�tre.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_display()
{
	// On efface les pixels de l'image (color buffer) et le Z-Buffer (depth buffer).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// On initialise la matrice de vue avec la matrice identit�.
	glLoadIdentity();

	// On applique une translation et une rotation � la sc�ne pour simuler
	// un d�placement de cam�ra.
	glTranslatef(xpos,0,zpos);
	glRotatef(angle_x, 1,0,0);
	glRotatef(angle_y, 0,1,0);

	//*****************************************************************
	//* A FAIRE :
	//* Re-sp�cifier la position des sources de lumi�re avec glLightfv()
	//* pour qu'elles soient aussi affect�es par la translation et la
	//* rotation qu'on vient d'appliquer � la sc�ne.
	//*****************************************************************

	GLfloat Light0Pos[4] = {5.0f, 20.0f, 10.0f, 0.0f};
	// Fixe la position de la lumière 0
	glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 5.5f);
	glDisable(GL_LIGHT0);


	GLfloat Light1Pos[4] = {xlight, ylight, zlight, 1.0f};

	// Fixe la position de la lumière 1
	glLightfv(GL_LIGHT1, GL_POSITION, Light1Pos);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	//glDisable(GL_LIGHT1);

	glPushMatrix();
		glTranslatef(Light1Pos[0], Light1Pos[1], Light1Pos[2]);
		GLfloat SphereAmbient[4] = {0.5f, 0.5f, 0.00f, 1.0f};
		GLfloat SphereDiffuse[4] = {1.0f, 1.0f, 0.0f, 1.0f};
		GLfloat SphereSpecular[4] = {0.5f,0.5f, 0.5f, 1.0f};
		GLfloat SphereShininess[] = { 10.0F };

		glMaterialfv(GL_FRONT, GL_AMBIENT, SphereAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, SphereDiffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, SphereSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, SphereShininess);

		glutSolidSphere(1,50,50);
	glPopMatrix();

	GLfloat fogColor[4]= {0.4f,0.4f,0.4f,0.0f};
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 100);
	glFogf(GL_FOG_START, 0);
	glFogf(GL_FOG_END, 70);
	glFogfv(GL_FOG_COLOR, fogColor);
	glEnable(GL_FOG);

	// On affiche la sc�ne.
	affiche_scene();

	// On force OpenGL � afficher avant de passer � la suite.
	glFlush();
}

///////////////////////////////////////////////////////////////////////////////
// Call-back : fonction appel�e lorsqu'on redimensionne la fen�tre.
//-----------------------------------------------------------------------------
// Param�tres :
//    width, height (in) : nouvelles largeur et hauteur de la fen�tre.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des �v�nements clavier.
//-----------------------------------------------------------------------------
// Param�tres :
//    key (in) : code ascii de la touche utilis�e.
//    x,y (in) : coordonn�es de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case KEY_ESC:						// 'ECHAP' :
			exit(1);						// on quitte le programme
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des touches speciales du clavier.
//-----------------------------------------------------------------------------
// Param�tres :
//    key (in) : code ascii de la touche utilis�e.
//    x,y (in) : coordonn�es de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	float speed = 1.0f;						// Vitesse de d�placement de la cam�ra

	switch (key)
	{
		case GLUT_KEY_UP:					// Fl�che vers le haut :
			//zpos += speed;					// on d�place la cam�ra selon z-
			zlight -= speed;
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_DOWN:					// Fl�che vers le bas :
			//zpos -= speed;					// on d�place la cam�ra selon z+
			zlight += speed;
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_LEFT:					// Fl�che vers la gauche :
			//xpos += speed;					// on d�place la cam�ra selon x-
			xlight -= speed;
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_RIGHT:				// Fl�che vers la droite :
			//xpos -= speed;					// on d�place la cam�ra selon x+
			xlight+= speed;
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_F1:
			ylight-= speed;
			glutPostRedisplay();
			break;

		case GLUT_KEY_F2:
			ylight+= speed;
			glutPostRedisplay();
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion de clics souris.
//-----------------------------------------------------------------------------
// Param�tres :
//    button (in) : code du bouton utilis�.
//    state  (in) : �tat du bouton.
//    x,y    (in) : coordonn�es de la souris.
// Retour :
//    _glLightfv(GL_LIGHT1, GL_POSITION, Light1Pos);
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des d�placements de la souris.
//-----------------------------------------------------------------------------
// Param�tres :
//    x,y    (in) : coordonn�es de la souris.
// Retour :// Fixe la position de la lumière 1
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	angle_x += y - mouse_y;						// Modifie la direction de vue de la cam�ra
	angle_y += x - mouse_x;						// en fonction de la position de la souris
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();						// Demande le r�affichage
}





///////////////////////////////////////////////////////////////////////////////
// Fonction principale du programme.
//-----------------------------------------------------------------------------
// Param�tres :
//    argc (in) : nombre d'arguments de la ligne de commande.
//    argv (in) : tableau de cha�nes de caract�res contenant les arguments.
// Retour :
//    Un entier contenant le code de retour du programme.
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// Initialisation de param�tres de Glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Maison");

	// Intitialisation de param�tres d'OpenGL
	initGL();

	// D�claration des fonctions de call-back.
	glutDisplayFunc(&callback_display);
	glutReshapeFunc(&callback_reshape);
	glutKeyboardFunc(&callback_keyboard);
	glutSpecialFunc(&callback_special);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	// Boucle principale de Glut.
	glutMainLoop();

	// Code de retour
	return 1;
}
