//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//TP 1 : prise en main
//compilation par : g++ -Wall ArchiIN_tp1.cpp -lGLEW -lGLU -lGL -lglut -o ArchiIN_tp1

//Premier TP d'ArchINA2, un fichier source simple comme base du tp
//Pour de la documentation, le site Intranet et la réf GLSL
//ne pas oublier LD_LIBRARY_PATH, si vous avez une version compilée par vous de GLEW
//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:glew/lib/
//compilation linux : make
//remarque : on utilise "-lglew" pour spécifier à g++ que l'on lie notre programme avec la bibliothèque glew/lib/libglew.so. Cette bibliothèque permet de développer facilement des shaders en uniformisant l'accès aux fonctions GLSL.
//les shaders sont compilés à l'exécution de ce programme
//Si vous modifiez les shaders (.vert ou .frag), inutile de recompiler ce programme, il n'y a qu'à le relancer, l'exécution du programme entraîne la compilation des shaders par le pilote graphique

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdlib.h>     /* srand, rand */

//pour la gestion des sources des shaders et la compilation de chacun
#include "glsl_fs.h"
#include "glsl_vs.h"
#include "glsl_program.h"

//des fonctions utiles (lecture fichier, glewInfo)
#include "fonctions.h"

using namespace std;
#define ECHAP 27

GLSL_Program * mes_shaders;
GLint loc;
GLint loctwo;

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;

float angle=0.0f;

//Timer qui permettra de fixer le framerate à 33im/s
void action_timer(int value)
{
	angle += 0.1f;
	glutPostRedisplay();
	glutTimerFunc(33, action_timer, value); //rappel dans 33ms
}

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

//le rendu de la scène
void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	gluPerspective(90.0, windowRatio, 0.1, 100); //définition d'une perspective (angle d'ouverture 130°,rapport L/H=1.0, near=0.1, far=100)

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Définition de la position de l'observateur
	gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //paramètres position(5.0, 5.0, 5.0), point visé(0.0, 0.0, 0.0), upVector - verticale (0.0, 1.0, 0.0)

	glRotatef(angle, 1, 1, 0);
			//on dessine une théière ou une sphère
			//rouge (c'est ce qu'on croit mais le fragment shader impose une couleur bleue)
			glColor3f(1.0f, 0.0f, 0.0f);

			/* initialize random seed: */

			float chaosOne = (float)(rand()%800)/1000 -0.4;
			float chaosTwo = (float)(rand()%800)/1000 -0.4;
			std::cerr<< chaosOne <<" _ "<< chaosTwo << std::endl;

			glutWireSphere(1.0f,8,8);
			//glutWireTeapot(1.0);
			glUniform1fARB(loc, chaosOne);
			glUniform1fARB(loctwo, chaosTwo);

	glutSwapBuffers();
}

//gestion du clavier
void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {
	case ECHAP:
		delete mes_shaders;
		cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
		glutLeaveMainLoop ( ); //retour au main()
		break;

	default:
		cerr << "callback_Keyboard - touche " << key << " non active." << endl;
	break;
	}
}

//préparation des shaders
void SetShaders(void) {
	GLSL_VS le_vertex_shader;
	GLSL_FS le_fragment_shader;

	//compilation du fichier source sur vertex shader
	le_vertex_shader.ReadSource("tp11.vert");
	le_vertex_shader.Compile();

	//compilation du fichier source sur fragment shader
	le_fragment_shader.ReadSource("tp11.frag");
	le_fragment_shader.Compile();

	//petites infos au cas où
	PrintShaderInfo(le_vertex_shader.idvs);
	PrintShaderInfo(le_fragment_shader.idfs);

	//on crée le shader program qui contient le vertex et le fragment shader
	mes_shaders = new GLSL_Program();

	mes_shaders -> Use_VertexShader(le_vertex_shader);
	mes_shaders -> Use_FragmentShader(le_fragment_shader);

	mes_shaders -> Link_Shaders();
	mes_shaders -> Activate();

	//glUseProgram( mes_shaders->idprogram);
	//on récupère l'adresse de la variagle GPU qui s'appelle "cputime"
 	loc = glGetUniformLocation(mes_shaders -> idprogram, "chaos");

	//des infos sur le shader program au cas où
	PrintProgramInfo(mes_shaders -> idprogram);

}

//initialisation du moteur OpenGL
void InitializeGL() {
	GLfloat lightPosition [] = { 0.0f, 10.0f, 0.0f, 0.0 };

	//Crée une source de lumière directionnelle
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Définit un modèle d'ombrage
	glShadeModel(GL_SMOOTH);

	//prise en compte des faces avant et arrières
	glEnable(GL_CULL_FACE);

	//Z Buffer pour la suppression des parties cachées
	glEnable(GL_DEPTH_TEST);

	//La variable d'état de couleur GL_AMBIENT_AND_DIFFUSE
	//est définie par des appels à glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutTimerFunc( 33, action_timer, 0); //envoi de la fontion action_timer toutes les 33ms
}

//Glew permet de connaître les capacités de la carte et de gérer les shaders de manière simple (et portable)
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

	srand (time(NULL));

	cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
      //cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n";

}

static void InitializeGlutCallbacks()
{
	//quelle fonction est appelée au rendu ?
	glutDisplayFunc(RenderScene);

	//quelle fonction est appelée quand le GPU est en attente ?
	//fonction inutile, on utilise un timer pour bloquer le framerate
	//glutIdleFunc(callback_Idle);

	//quelle fonction est appelée pour traiter les événements du clavier (classique) ?
	glutKeyboardFunc(callback_Keyboard);

	//quelle fonction est appelée pour traiter les événements souris ?
	//glutMotionFunc(callback_Mouse);

	//quelle fonction est appelée pour traiter les événements sur la fenêtre ?
	glutReshapeFunc(callback_Window);

	//quelle fonction est appelée pour traiter les touches spéciales du clavier ?
	//glutSpecialFunc(&callback_SpecialKeys);

}

//le programme principal
int main(int argc, char **argv) {
	srand(time(NULL));	//pour le random

	//on crée le contexte OpenGL et la fenêtre
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("TP1 premiers pas VS FS");

	//toujours après l'initialisation de glut
	GlewInit();

	InitializeGlutCallbacks();
	InitializeGL();
	SetShaders();


	glutMainLoop();


	delete mes_shaders;
	return (EXIT_SUCCESS);
}
