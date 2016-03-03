///////////////////////////////////////////////////////////////////////////////
// IUT d'Aix-Marseille Universit�, site d'Arles
// D�partement Informatique
// 2�me ann�e
// ----------------------------------------------------------------------------
// Auteur : S�bastien Thon
// ----------------------------------------------------------------------------
// Programme permettant d'afficher un terrain 3D.
///////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "terrain.h"
#include "texture.hpp"

int SCREEN_WIDTH  = 800;								// Largeur de la fen�tre OpenGL
int SCREEN_HEIGHT = 600;								// Hauteur de la fen�tre OpenGl

#define KEY_ESC 27										// Code ASCII de la touche Echap


float	xpos = 0, ypos = 20, zpos = 150;				// Position de la cam�ra
int		angle_x = 0, angle_y = 0;						// Orientation de la cam�ra
int		mouse_x = 0, mouse_y = 0;						// Position de la souris

// Param�tres de la source de lumi�re 0 (directionnelle)
GLfloat Light0Position[]= { -4.0f, 4.0f, -4.0f, 0.0f };
GLfloat Light0Ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat Light0Diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat Light0Specular[]= { 0.5f, 0.5f, 0.5f, 1.0f };

Terrain	terrain;										// D�claration d'un objet terrain

///////////////////////////////////////////////////////////////////////////////
// Intitialisation de certains param�tres d'OpenGL.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid initGL()
{
	glClearColor(0.7, 0.9, 1.0, 1);						// Couleur servant � effacer la fen�tre (bleu ciel)
    glShadeModel(GL_SMOOTH);							// Mod�le d'ombrage : lissage de Gouraud
	glEnable(GL_CULL_FACE);								// Ne traite pas les faces cach�es
	glEnable(GL_DEPTH_TEST);							// Active le Z-Buffer
	glDepthFunc(GL_LEQUAL);								// Mode de fonctionnement du Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Ambient);	// Couleur ambiante de la source de lumi�re 0 (directionnelle)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diffuse);	// Couleur diffuse de la source de lumi�re 0
	glLightfv(GL_LIGHT0, GL_SPECULAR,Light0Specular);	// Couleur sp�culaire de la source de lumi�re 0
	glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);	// Direction de la source de lumi�re 0
	glEnable(GL_LIGHT0);								// Activation de la source de lumi�re 0

	glEnable(GL_LIGHTING);								// Activation de l'�clairage g�n�ral
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
	terrain.affiche();							// Affiche le terrain


	glutSwapBuffers();							// Affiche la sc�ne (affichage en double buffer)
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
	glTranslatef(-xpos,-ypos,-zpos);
	glRotatef(angle_x, 1,0,0);
	glRotatef(angle_y, 0,1,0);
	glTranslatef(-terrain.centre_x(),0,-terrain.centre_z());

	// On re-sp�cifie la position des sources de lumi�re avec pour qu'elles
	// soient aussi affect�es par la translation et la rotation qu'on vient
	// d'appliquer � la sc�ne.
	glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);

	// On affiche la sc�ne.
	affiche_scene();

	// On force OpenGL � afficher avant de passer � la suite.
	glFlush();
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : redimensionnement de la fen�tre.
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
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);

	SCREEN_WIDTH  = width;
	SCREEN_HEIGHT = height;

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
			zpos -= speed;					// on d�place la cam�ra selon z-
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_DOWN:					// Fl�che vers le bas :
			zpos += speed;					// on d�place la cam�ra selon z+
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_LEFT:					// Fl�che vers la gauche :
			xpos -= speed;					// on d�place la cam�ra selon x-
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_RIGHT:				// Fl�che vers la droite :
			xpos += speed;					// on d�place la cam�ra selon x+
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_PAGE_UP:				// Page Up :
			ypos += speed;
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_PAGE_DOWN:			// Page Down :
			ypos -= speed;
			glutPostRedisplay();			// et on demande le r�affichage.
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
//    _
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
// Retour :
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
// Initialise la sc�ne.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
void initialise_scene()
{
	// On cr�e le terrain au moyen d'une image en niveaux de gris au format PGM.
	// On espace les points de 1.0 unit� selon x, de 0.1 unit� selon y, de 1.0 unit� selon z
	glEnable(GL_TEXTURE_2D);
	terrain.creation( 1.0, 0.1, 1.0, "terrain_128x128.pgm" );
	terrain.calcule_coordonnees_texture();
	terrain.calcule_normales();
	terrain.charger("terrain_texture.png");
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
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Terrain");

	// Intitialisation de param�tres d'OpenGL
	initGL();

	// Initialisation de la sc�ne
	initialise_scene();

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
