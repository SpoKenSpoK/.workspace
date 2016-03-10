//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//le prog principal pour effectuer une déformation d'un plan
//ouvre un fichier 3D OFF, active un ProgramShader avec une contrainte
//qui déforme le vertex s'il est dans le voisinage de la contrainte

#include <GL/glew.h> //attention, à inclure avant gl.h

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
#include "objetsOFF.h"

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

float angle_x = 0.0f;
float angle_y = 0.0f;

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;

//une structure pour stocker les paramètres de la contrainte
struct Contrainte {
	float centre[3];
	float vecteur[3];
	float rayon;
};

struct Contrainte maContrainte;

//les adresses où ou enverra ces valeurs sur la carte graphique
GLint where_centre;
GLint where_vecteur;
GLint where_rayon;
Objet monObjet;

point3D CentremonObjet;
float RayonTrajContrainte;

//Au cas où, 2 fonctions intéressantes...
float fDegree2Radian(float _Dangle) {
	return (_Dangle * M_PI / 180.0f);
}

float fRadian2Degree(float _Rangle) {
	return (_Rangle * 180.0f / M_PI);
}

//Chargement de l'objet 3D, nom du fichier en dur=pas beau
void GeomInit(void) {
unsigned int nfaces;


monObjet.charge_OFF((const char*) "plan_33x33.off");

/*monObjet.affiche();*/
monObjet.id=glGenLists(1);

glNewList(monObjet.id, GL_COMPILE);
	glBegin(GL_TRIANGLES);
		for (nfaces = 0; nfaces<monObjet.nbfaces; nfaces++) {
			glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S1].x, monObjet.lpoints[monObjet.lfaces[nfaces].S1].y, monObjet.lpoints[monObjet.lfaces[nfaces].S1].z);

			glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S2].x, monObjet.lpoints[monObjet.lfaces[nfaces].S2].y, monObjet.lpoints[monObjet.lfaces[nfaces].S2].z);

			glVertex3f(monObjet.lpoints[monObjet.lfaces[nfaces].S3].x, monObjet.lpoints[monObjet.lfaces[nfaces].S3].y, monObjet.lpoints[monObjet.lfaces[nfaces].S3].z);
		}
	glEnd();
glEndList();

//on calcule le centre de l'objet (si on a besoin de le viser avec la caméra par ex.)
CentremonObjet.x = (monObjet.max.x+monObjet.min.x)/2.0f;
CentremonObjet.y = (monObjet.max.y+monObjet.min.y)/2.0f;
CentremonObjet.z = (monObjet.max.z+monObjet.min.z)/2.0f;

RayonTrajContrainte=0.6f;

maContrainte.centre[0]=0.0f;
maContrainte.centre[1]=0.0f;
maContrainte.centre[2]=0.0f;

maContrainte.vecteur[0]=0.0f;
maContrainte.vecteur[1]=0.0f;
maContrainte.vecteur[2]=2.0f;

maContrainte.rayon=0.2f;

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

//pour l'instant on ne fait rien en cas d'inactivité
//mais cela pourrait servir à modifier la contrainte...
void callback_Idle() {
	angle += 0.001f;
	glutPostRedisplay();
}

//classique OpenGL, l'objet est dans une DisplayList puisqu'il n'est pas modifié
//cela accélère l'envoi de l'objet 3D à la carte graphique
void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Modification de la matrice de projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //remise à 0 (identité)
	gluPerspective(90.0, windowRatio, 0.1, 100); //définition d'une perspective (angle d'ouverture 130°,rapport L/H=1.0, near=0.1, far=100)

	//Modification de la matrice de modélisation de la scène
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

	//cf le callback_clavier
	glRotatef(angle_x,1,0,0);
	glRotatef(angle_y,0,1,0);


	float r = 0.3f;
	maContrainte.centre[0] = r* cos(angle);
	maContrainte.centre[1] = r* sin(angle);

	//transmission des valeurs de la structure Contrainte
	glUniform3fv(where_centre, 1, maContrainte.centre);
	glUniform3fv(where_vecteur, 1, maContrainte.vecteur);
	glUniform1f(where_rayon, maContrainte.rayon);

		//Parce qu'on avait pas vu encore les dsiplay List...
		glCallList(monObjet.id);

	glutSwapBuffers();
}

//classique
//l'appui sur les touche qsdz modifie l'angle de vue
void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {

	case 'z':
		angle_x++;
		break;
	case 's':
		angle_x--;
		break;

	case 'q':
		angle_y--;
		break;
	case 'd':
		angle_y++;
		break;

	case ECHAP:
		delete mes_shaders;
		cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
		glutLeaveMainLoop(); //retour au main()
		break;

	default:
		cerr << "La touche " << int(key) << " est non active." << endl;
		cerr << "\t -> Z S Q D pour faire tourner l'objet" << endl;
		break;
	}
}

//La construction des shaders
void SetShaders(void) {
GLSL_VS le_vertex_shader;
GLSL_FS le_fragment_shader;

	le_vertex_shader.ReadSource("tpdeformation.vert");
	le_vertex_shader.Compile();

	le_fragment_shader.ReadSource("tpdeformation.frag");
	le_fragment_shader.Compile();

PrintShaderInfo(le_vertex_shader.idvs);
PrintShaderInfo(le_fragment_shader.idfs);
mes_shaders = new GLSL_Program();

	mes_shaders -> Use_VertexShader(le_vertex_shader);
	mes_shaders -> Use_FragmentShader(le_fragment_shader);

	mes_shaders -> Link_Shaders();
	mes_shaders -> Activate();

	//on aura besoin de localiser ces données dans la mémoire de la carte graphique
	//(pour pouvoir les modifier)
	where_centre = glGetUniformLocation(mes_shaders -> idprogram, "Uv3centre_deformation");
	where_vecteur = glGetUniformLocation(mes_shaders -> idprogram, "Uv3vecteur_deformation");
	where_rayon = glGetUniformLocation(mes_shaders -> idprogram, "Ufrayon_deformation");

PrintProgramInfo(mes_shaders -> idprogram);

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

//Classique OpenGL
void InitializeGL() {

		GLfloat lightPosition [] = { 0.0f, 10.0f, 0.0f, 0.0 };

	//Crée une source de lumière directionnelle
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Définit un modèle d'ombrage
	glShadeModel(GL_SMOOTH);

	//prise en compte des faces avant et arrières
	//glEnable(GL_CULL_FACE);

	//Z Buffer pour la suppression des parties cachées
	glEnable(GL_DEPTH_TEST);

	//La variable d'état de couleur GL_AMBIENT_AND_DIFFUSE
	//est définie par des appels à glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

}

//on s'assure que la carte graphique peut faire de l'openGL2
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
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("TP2 deformation VS FS");

	//attachement des fonctions de l'OpenGL
	//toujours après l'initialisation de glut
	GlewInit();

	InitializeGlutCallbacks();
	InitializeGL();
	SetShaders();

	GeomInit();


	glutMainLoop();

	delete mes_shaders;

	return (EXIT_SUCCESS);
}
