//R. RAFFIN, IUT Aix-Marseille, dépt. Informatique - Arles
//TP simple, affichages de quelques points
//compilation par : g++ -Wall TP1points.cpp -lGL -lGLU -lglut -lGLEW -o TP1points
// https://ametice.univ-amu.fr/pluginfile.php/827988/mod_resource/content/1/TP.pdf

#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "cercle.hpp"

using namespace std;

GLuint leVBO;
Cercle cercle(9, 0, 0, 1);

static void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0); //glEnableVertexAttribArray - Enable or disable a generic vertex attribute array

	glBindBuffer(GL_ARRAY_BUFFER, leVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //Commencer de 0, chaque position sera sur 3 éléments, de type float

	glDrawArrays(GL_LINE_LOOP, 0, cercle.getNbP() ); //3 éléments à utiliser pour le dessin

	glDisableVertexAttribArray(0); //glDisableVertexAttribArray disables the generic vertex attribute array specified by index

	glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderScene);
}

static void CreateVertexBuffer()
{
	//float vertices[27]; // 3 points à 3 coordonnées x,y,z par point
	cercle.calculPosition_sommets();

/*
	srand (time(NULL));
	for(unsigned int i=0; i<18; i+=2){
		vertices[i] = (float)(rand()% 201) / 100 - 1;
		vertices[i+1] = (float)(rand()% 201) / 100 - 1;
		vertices[i+2] = 0.0;
	}
*/

/*
//1er sommet
	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

//2ème sommet
	vertices[3] = 0.25f;
	vertices[4] = 0.25f;
	vertices[5] = 0.0f;

//3ème sommet
	vertices[6] = 0.0f;
	vertices[7] = 0.9f;
	vertices[8] = 0.0f;

	vertices[9] = -0.25f; //pour tester
	vertices[10] = 0.25f;
	vertices[11] = 0.0f;

	vertices[12] = -.9f;
	vertices[13] = 0.0f;
	vertices[14] = 0.0f;

	vertices[15] = -0.25f;
	vertices[16] = -0.25f;
	vertices[17] = 0.0f;

	vertices[18] = 0.0f;
	vertices[19] = -0.9f;
	vertices[20] = 0.0f;

	vertices[21] = 0.25f;
	vertices[22] = -0.25f;
	vertices[23] = 0.0f;

	vertices[24] = 0.9f;
	vertices[25] = 0.0f;
	vertices[26] = 0.0f;
*/

 	glGenBuffers(1, &leVBO); //génération d'une référence de buffer object
	glBindBuffer(GL_ARRAY_BUFFER, leVBO); //liaison du buffer avec un type tableau de données
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* cercle.getNbP()*3, cercle.returnVertices(), GL_STATIC_DRAW); //création et initialisation du container de données (3 sommets -> 9 float)
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TP1 : quelques points");

	InitializeGlutCallbacks();

	//toujours après l'initialisation de glut
	GLenum res = glewInit();

	if (res != GLEW_OK) {
		cerr << "Error: " << glewGetErrorString(res) << endl;
		return (EXIT_FAILURE);
	}

	//cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();

	glutMainLoop();

	return (EXIT_SUCCESS);
}
