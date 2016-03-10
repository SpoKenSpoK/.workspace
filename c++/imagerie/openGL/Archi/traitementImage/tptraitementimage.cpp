//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//tp modification d'une image par un fragment shader

#include <GL/glew.h> //à mettre avant gl.h

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <iostream>

//pour la gestion des sources des shaders et la compilation de chacun
#include "glsl_fs.h"
#include "glsl_vs.h"
#include "glsl_program.h"

//des fonctions utiles (lecture fichier, glewInfo, classe objetOFF)
#include "fonctions.h"

using namespace std;
#define ECHAP 27

#ifndef M_PI
	define M_PI	3.14159265358979323846
#endif

GLSL_Program * mes_shaders;
GLint adresse_mem;

float lpos[4] = {1, 0.5, 1,0};
float angle = 0.0f;
float mytime = 0.0f;
unsigned int imagew, imageh;

GLuint texture_desc=-1;

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;

GLint loc;

float seuil = 0.5f;

struct Vertex
{
	float tu, tv;
	float x, y, z;
};

//unr structure pour stocker les coordonnées des 4 sommets du quad + indice de texture
struct Vertex MonQuadTexture[] =
{
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f }
};



//fonction de call-back pour le redimensionnement de la fenêtre
static GLvoid callback_Window(GLsizei width, GLsizei height)
{
	//au cas où h soit nul et entraîne une division par 0
	if(height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	windowWidth = width;
	windowHeight = height;

	windowRatio = float(windowWidth) / float(windowHeight);

	cout << "callback_Window - " << "new width " << windowWidth << " new height " << windowHeight << endl;
}


//classique OpenGL, redisplay inutile car pas de mouvement
void callback_Idle() {
	//angle += 0.01f;
	glutPostRedisplay();
}

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	glOrtho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, texture_desc);
		glInterleavedArrays(GL_T2F_V3F, 0, MonQuadTexture);
		glDrawArrays( GL_QUADS, 0, 4 );

	glFlush(); // pas de double buffer, flush() suffit
	//glutSwapBuffers();

	glUniform1fARB(loc, seuil);
	loc = glGetUniformLocation(mes_shaders->idprogram, "seuil");
}

//gestion classique du clavier
void callback_Keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case ECHAP:
			delete mes_shaders;
			cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
			glutLeaveMainLoop(); //retour au main()
			break;
		case '+':
			if(seuil <= 0.99)
				seuil+=0.01;
				std::cerr << seuil << std::endl;
			break;
		case '-':
			if(seuil >= 0.01){
				seuil-=0.01;
			}

				std::cerr << seuil << std::endl;
			break;

		default:
			cerr << "La touche " << int(key) << " est non active." << endl;
			break;
		}
}

//mise en place des shaders et du program
void SetShaders(void) {
	GLSL_VS le_vertex_shader;
	GLSL_FS le_fragment_shader;

	le_vertex_shader.ReadSource("tptraitementimage.vert");
	le_vertex_shader.Compile();

	le_fragment_shader.ReadSource("tptraitementimage.frag");
	//le_fragment_shader.ReadSource("grey.frag");
	//le_fragment_shader.ReadSource("bandw.frag");
	le_fragment_shader.Compile();

	PrintShaderInfo(le_vertex_shader.idvs);
	PrintShaderInfo(le_fragment_shader.idfs);
	mes_shaders = new GLSL_Program();

	mes_shaders -> Use_VertexShader(le_vertex_shader);
	mes_shaders -> Use_FragmentShader(le_fragment_shader);

	mes_shaders -> Link_Shaders();
	mes_shaders -> Activate();

	PrintProgramInfo(mes_shaders -> idprogram);

}

//classique OpenGL
//on active le support des textures
void InitializeGL() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
}

static void InitializeGlutCallbacks()
{
	//quelle fonction est appelée au rendu ?
	glutDisplayFunc(RenderScene);

	//quelle fonction est appelée quand le GPU est en attente ?
	//fonction inutile, on utilise un timer pour bloquer le framerate
	glutIdleFunc(callback_Idle);

	//quelle fonction est appelée pour traiter les événements du clavier (classique) ?
	glutKeyboardFunc(callback_Keyboard);

	//quelle fonction est appelée pour traiter les événements souris ?
	//glutMotionFunc(callback_Mouse);

	//quelle fonction est appelée pour traiter les événements sur la fenêtre ?
	glutReshapeFunc(callback_Window);

	//quelle fonction est appelée pour traiter les touches spéciales du clavier ?
	//glutSpecialFunc(&callback_SpecialKeys);

}

//vérification du support d'openGL2
void GlewInit() {
GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		cerr << "Erreur: " << glewGetErrorString(err) << endl;
	}
	cerr << "initGLEW_ok, ref GLEW " << glewGetString(GLEW_VERSION) << endl;

	if (glewIsSupported("GL_VERSION_2_0"))
		cerr << "ok OpenGL 2.0\n";
	else {
		cerr << "OpenGL 2.0 impossible\n";
		exit(EXIT_FAILURE);
	}

	cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
      //cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n";

}


int main(int argc, char **argv) {

	//on crée le contexte OpenGL et la fenêtre
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA); //pas de double buffer (inutile si pas de mouvement)
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("TP traitement d image VS FS");

	InitializeGlutCallbacks();
	InitializeGL();

	GlewInit();
		string fichier("LenaRGB.png");
		LoadTexture(&fichier, &texture_desc, &imagew, &imageh);
	SetShaders();


	glutMainLoop();

	delete mes_shaders;
	return (EXIT_FAILURE);
}
