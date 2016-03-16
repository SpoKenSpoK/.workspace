//R. RAFFIN, IUT Aix-Marseille, dépt. Informatique - Arles
//TP couleurs, transformations, caméras
//compilation par : g++ -Wall TP2.cpp -lGLEW -lGLU -lGL -lglut -o TP2

#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "tetraedre.hpp"

using namespace std;

GLuint leVBO;
GLuint leBufferCouleur;

float anglex=0.0f;
float angley=0.0f;
float anglez=0.0f;

Tetraedre tetraedre(0.5);

static void RenderSceneCouleurs()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0); //couleur générale

	//Liaison avec le buffer de vertex
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); //description des données pointées

	//Liaison avec le buffer de couleur
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leBufferCouleur);
	glColorPointer(3, GL_FLOAT, 0, 0); //description des données pointées

	glDrawArrays(GL_TRIANGLES, 0, tetraedre.getNbP()); //3 éléments à utiliser pour le dessin

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers();
}

static void RenderSceneRotation()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //matrice de modélisation de la scène
	glLoadIdentity(); //matrice identité -> pas de transformation

	glColor3f(1.0, 0.0, 0.0);

	glRotatef(anglex, 1, 0, 0); //rotation de l'angle en degrés sur l'axe des y (verticale OpenGL)
	glRotatef(angley, 0, 1, 0); //rotation de l'angle en degrés sur l'axe des y (verticale OpenGL)
	glRotatef(anglez, 0, 0, 1); //rotation de l'angle en degrés sur l'axe des y (verticale OpenGL)

	//Liaison avec le buffer de couleur
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leBufferCouleur);
	glColorPointer(3, GL_FLOAT, 0, 0); //description des données pointées

	//Liaison avec le buffer de vertex
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); //Commencer de 0, chaque position sera sur 3 éléments, de type float

	glDrawArrays(GL_TRIANGLES, 0, tetraedre.getNbP()); //3 éléments à utiliser pour le dessin

	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
}

static void RenderSceneCameraOrthographique()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0); //couleur générale

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)

	// Choix au niveau des différentes rotation :

	//glRotatef(90, 0,1,0); // Vue de Face
	glRotatef(90, 1,0,0); // Vue de Dessus
	//glRotatef(0,0,1,0); // Vu de Côté

	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0); //projection orthogonale sur z=0, avec un volume de vue : xmin=-10, xmax=10, ymin=-10, ymax=10, ...

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Liaison avec le buffer de vertex
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); //description des données pointées

	glDrawArrays(GL_TRIANGLES, 0, 3); //3 éléments à utiliser pour le dessin

	glDisableClientState(GL_VERTEX_ARRAY);

	//parce que c'est joli...
	glColor3f(0.2, 0.5, 0.3);
	glutWireTeapot(2.0);

	glutSwapBuffers();
}

static void RenderSceneCameraPerspective()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0); //couleur générale

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	gluPerspective(90.0, 1.0, 0.1, 100); //définition d'une perspective (angle d'ouverture 90°,rapport L/H=1.0, near=0.1, far=100)

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Définition de la position de l'observateur
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//paramètres position(0.0, 0.0, 5.0), point visé(0.0, 0.0, 0.0, upVector - verticale (0.0, 1.0, 0.0)

	//Liaison avec le buffer de vertex
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, leVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0); //description des données pointées

	glDrawArrays(GL_TRIANGLES, 0, 3); //3 éléments à utiliser pour le dessin

	glDisableClientState(GL_VERTEX_ARRAY);

	//parce que c'est joli...
	glColor3f(0.2, 0.5, 0.3);
	glutWireTeapot(2.0);

	glutSwapBuffers();
}

GLvoid callback_keyboard(unsigned char key, int x, int y)
{
	float value = 10.0f;
	switch (key)
	{

		case 'a':					// Fl�che vers le haut :
			anglex += value;					// on d�place la cam�ra selon z-
			break;

		case 'z':					// Fl�che vers le bas :
			anglex -= value;					// on d�place la cam�ra selon z
			break;

		case 'e':					// Fl�che vers la gauche :
			angley += value;					// on d�place la cam�ra selon x-
			break;
		case 'r':					// Fl�che vers la gauche :
			angley -= value;					// on d�place la cam�ra selon x-
			break;
		case 't':					// Fl�che vers la gauche :
			anglez += value;					// on d�place la cam�ra selon x-

			break;
		case 'y':					// Fl�che vers la gauche :
			anglez -= value;					// on d�place la cam�ra selon x-
			break;
		case 'w' :
			glutDisplayFunc(RenderSceneCouleurs);
			break;
		case 'x' :
			glutDisplayFunc(RenderSceneRotation);
			break;
		case 'c' :
			glutDisplayFunc(RenderSceneCameraPerspective);
			break;
		case 'v' :
			glutDisplayFunc(RenderSceneCameraOrthographique);
			break;
	}
	//glutDisplayFunc(RenderSceneRotation);

	std::cerr <<"Angle X : "<<anglex<<std::endl;
	std::cerr <<"Angle Y : "<<angley<<std::endl;
	std::cerr <<"Angle Z : "<<anglez<<std::endl;
	std::cerr <<"--------------------------------"<<std::endl;
}


//A lier à la glutIdleFunc
static void IdleFunc()
{
	glutKeyboardFunc(&callback_keyboard);

	/*
		Modifier automatiquement les valeurs des différents angles dès que le GPU est disponible.
		Nous pouvons commenter ces lignes et modifier les valeurs à la main à l'aide des touches définies
		dans callback_keyboard
	*/

	anglex +=0.01;
	angley +=0.01;
	anglez +=0.01;

	glutPostRedisplay();
}




static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneRotation);
	glutIdleFunc(IdleFunc);
}


static void CreateColorBuffer()
{
 	glGenBuffers(1, &leBufferCouleur); //génération d'une référence de buffer object
	glBindBuffer(GL_ARRAY_BUFFER, leBufferCouleur); //liaison du buffer avec un type tableau de données
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 36 /*9*/, tetraedre.colorisation()/*colors*/, GL_STATIC_DRAW); //création et initialisation du container de données (3 couleurs -> 9 float)
}


static void CreateVertexBuffer()
{
	tetraedre.calculPosition_sommets();

 	glGenBuffers(1, &leVBO); //génération d'une référence de buffer object
	glBindBuffer(GL_ARRAY_BUFFER, leVBO); //liaison du buffer avec un type tableau de données
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36, tetraedre.returnVertices(), GL_STATIC_DRAW); //création et initialisation du container de données (3 sommets -> 9 float)
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TP2 : couleurs, transformations, cameras");

	InitializeGlutCallbacks();

	//toujours après l'initialisation de glut
	GLenum res = glewInit();

	if (res != GLEW_OK) {
		cerr << "Error: " << glewGetErrorString(res) << endl;
		return (EXIT_FAILURE);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//fonction de remplissage du buffer de vertex
	CreateVertexBuffer();

	//fonction de remplissage du buffer de couleurs
	CreateColorBuffer();

	glutMainLoop();

	return (EXIT_SUCCESS);
}
