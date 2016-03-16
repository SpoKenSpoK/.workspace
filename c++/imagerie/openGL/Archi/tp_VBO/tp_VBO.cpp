//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Display Lists vs VBO vs Vertex Array
#include <GL/glew.h> //attention, à inclure avant gl.h

#include <GL/gl.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

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

//variables pour la gestion des paramètres de la fenêtre
float windowRatio = 1.0f;
int windowHeight = 500;
int windowWidth = 500;

int rendu;
unsigned int nbimages;
float angle = 0.0f;


#define RENDU_DL 0	//displaylist
#define RENDU_VA 1	//VertexArrays
#define RENDU_VBO 2	//VertexBufferObject

//GLSL_Program * mes_shaders;

float MatSpec[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //blanc
float MatDif[4] = {1.0f, 1.0f, 1.0f, 1.0f}; //blanc
float MatAmb[4] = {0.3f, 0.3f, 0.3f, 1.0f}; //gris foncé

float Light1Pos[4] = {0.0f, 0.0f, 20.0f, 1.0f};
float Light1Dif[4] = {0.0f, 0.2f, 0.2f, 1.0f};
float Light1Spec[4] = {0.0f, 0.2f, 0.2f, 1.0f};
float Light1Amb[4] = {0.5f, 0.5f, 0.5f, 1.0f};

//pour la Dsiaply List
GLint madisplaylist;

//pour les VertexArrays
float * ColorArray;
float * NormalArray;
float * VertexArray;

//pour les VBO
GLuint VBOCouleurs;
GLuint VBOSommets;
GLuint VBONormales;

Objet monobjet;

void BeforeReallyQuit() {

cerr << "Nettoyage des ressources\n";

//	delete mes_shaders;

	if (rendu == RENDU_VA) {//pour les VertexArrays
		delete[] VertexArray;
		delete[] NormalArray;
		delete[] ColorArray;
	}

	//pour les VBO
	if (rendu == RENDU_VBO) {
		glDeleteBuffers((GLsizei) 1, &VBOSommets);
		glDeleteBuffers((GLsizei) 1, &VBOCouleurs);
		glDeleteBuffers((GLsizei) 1, &VBONormales);
	}

cerr << "System Clear :-)\n";
}

void callback_Timer(int value){
char message[200];

switch (rendu) {
	case RENDU_DL :
		strcpy(message, "Display Lists ");
	break;
	case RENDU_VA :
		strcpy(message, "Vertex Arrays ");
	break;
	case RENDU_VBO :
		strcpy(message, "VertexBufferObject ");
	break;
}

char nombre[10];
sprintf(nombre, "%u", nbimages);
strcat(message, nombre);
strcat(message, " im/s");

	glutSetWindowTitle(message);

nbimages = 0;

glutTimerFunc(1000, callback_Timer, value); //on repart pour une alarm dans 1 s

}

void LoadOFFObject(Objet * _objet, const char * _fichier_off) {
	_objet -> charge_OFF(_fichier_off);
}

void CreateVBOFromObject(Objet * _monobjet) {

if (glewIsSupported("GL_ARB_vertex_buffer_object") == GL_FALSE) {
		cerr << "VBO impossibles, non supportés\n";
		exit (1);
}

//A priori, les VBO sont supportés

//création des tableaux de normales et points
float * lnormales = new float[_monobjet -> nbfaces * 9];
float * lsommets = new float[_monobjet -> nbfaces * 9];
float * lcolors = new float[_monobjet -> nbfaces * 9];

//remplissage des tableaux
//tableau des couleurs, 1 par point, tirée aléatoirement
unsigned int cpt = 0;
for (unsigned int i = 0; i < _monobjet -> nbfaces; i++) {
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //R
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //G
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //B

	lcolors[cpt++] = tirage_alea(0.0, 0.5); //R
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //G
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //B

	lcolors[cpt++] = tirage_alea(0.0, 0.5); //R
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //G
	lcolors[cpt++] = tirage_alea(0.0, 0.5); //B
}

//tableau des normales
cpt=0;
for (unsigned int i = 0; i < _monobjet -> nbfaces; i++) {
//On recopie UNE normale par sommets
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].x;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].y;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].z;

	lnormales[cpt++] = _monobjet -> lfacesnormales[i].x;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].y;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].z;

	lnormales[cpt++] = _monobjet -> lfacesnormales[i].x;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].y;
	lnormales[cpt++] = _monobjet -> lfacesnormales[i].z;
}

cpt = 0;
//remplissage du tableau des sommets (!! non indexé, 3 points par faces = lourd)
for (unsigned int i = 0; i < _monobjet -> nbfaces; i++) {

	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S1].x;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S1].y;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S1].z;

	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S2].x;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S2].y;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S2].z;

	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S3].x;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S3].y;
	lsommets[cpt++] = _monobjet->lpoints[_monobjet->lfaces[i].S3].z;
}


//VBO pour les sommets
glGenBuffers((GLsizei) 1, &VBOSommets);
	glBindBuffer(GL_ARRAY_BUFFER, VBOSommets);
	glBufferData(GL_ARRAY_BUFFER, _monobjet -> nbfaces * 9 * sizeof(float), lsommets, GL_STATIC_DRAW);

//VBO pour les couleurs
glGenBuffers((GLsizei) 1, &VBOCouleurs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCouleurs);
	glBufferData(GL_ARRAY_BUFFER, _monobjet -> nbfaces * 9 * sizeof(float), lcolors, GL_STATIC_DRAW);

//VBO pour les normales
glGenBuffers((GLsizei) 1, &VBONormales);
	glBindBuffer(GL_ARRAY_BUFFER, VBONormales);
	glBufferData(GL_ARRAY_BUFFER, _monobjet -> nbfaces * 9 * sizeof(float) , lnormales, GL_STATIC_DRAW);

//on peut les jeter, ca a été transféré à la CG
delete []lcolors;
delete []lnormales;
delete []lsommets;

}

void CreateVertexArraysFromObject(Objet* _monObjet) {

//création des tableaux de normales et points
NormalArray = new float[_monObjet -> nbfaces * 9];
VertexArray = new float[_monObjet -> nbfaces * 9];
ColorArray = new float[_monObjet -> nbfaces * 9];

//remplissage des tableaux
//!! attention ces tableaux doivent être accessibles par la boucle de rendu (globaux)
//tableau des couleurs, 1 par point, tirée aléatoirement
unsigned int cpt = 0;
for (unsigned int i = 0; i < _monObjet -> nbfaces; i++) {
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //R
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //G
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //B

	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //R
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //G
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //B

	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //R
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //G
	ColorArray[cpt++] = tirage_alea(0.5, 1.0); //B
}

//tableau des normales
cpt=0;
for (unsigned int i = 0; i < _monObjet -> nbfaces; i++) {
//On recopie UNE normale par sommets
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].x;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].y;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].z;

	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].x;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].y;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].z;

	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].x;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].y;
	NormalArray[cpt++] = _monObjet -> lfacesnormales[i].z;
}

cpt = 0;
//remplissage du tableau des sommets (!! non indexé, 3 points par faces = lourd)
for (unsigned int i = 0; i < _monObjet -> nbfaces; i++) {

	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S1].x;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S1].y;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S1].z;

	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S2].x;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S2].y;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S2].z;

	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S3].x;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S3].y;
	VertexArray[cpt++] = _monObjet->lpoints[_monObjet->lfaces[i].S3].z;
}

}

GLint CreateDisplayListFromObject(Objet* _monObjet) {
unsigned int nfaces;


/*monObjet.affiche();*/
_monObjet -> id = glGenLists(1);

glNewList(_monObjet -> id, GL_COMPILE);

	for (nfaces = 0; nfaces < _monObjet -> nbfaces; nfaces++) {

	glBegin(GL_TRIANGLES);
		glNormal3f(_monObjet -> lfacesnormales[nfaces].x, _monObjet -> lfacesnormales[nfaces].y, _monObjet -> lfacesnormales[nfaces].z);

		glVertex3f(_monObjet->lpoints[_monObjet->lfaces[nfaces].S1].x, _monObjet->lpoints[_monObjet->lfaces[nfaces].S1].y, _monObjet->lpoints[_monObjet->lfaces[nfaces].S1].z);
		glVertex3f(_monObjet->lpoints[_monObjet->lfaces[nfaces].S2].x, _monObjet->lpoints[_monObjet->lfaces[nfaces].S2].y, _monObjet->lpoints[_monObjet->lfaces[nfaces].S2].z);
		glVertex3f(_monObjet->lpoints[_monObjet->lfaces[nfaces].S3].x, _monObjet->lpoints[_monObjet->lfaces[nfaces].S3].y, _monObjet->lpoints[_monObjet->lfaces[nfaces].S3].z);
	glEnd();
	}
glEndList();

return (_monObjet -> id);
}

void GeomInit(void) {

	//on charge l'obbjet OFF
		LoadOFFObject(&monobjet, "data/spaceshuttle.off");

	//LoadOFFObject(&monobjet, "data/dragon_fullres.off");

	switch (rendu) {
		case RENDU_DL :
			//on crée la DisplayList
			madisplaylist = CreateDisplayListFromObject(&monobjet);
		break;
		case RENDU_VA :
			//on crée les tableaux pour le VertexArrays
			CreateVertexArraysFromObject(&monobjet);
		break;
		case RENDU_VBO :
			//on crée les VertexBufferObjets
			CreateVBOFromObject(&monobjet);
		break;
	}
	cout << "Géométrie chargée\n" << endl;
}



void DrawVBO(){
	//On éteint la lumière pour profiter des couleurs...
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);

	glBindBuffer( GL_ARRAY_BUFFER, VBOSommets);
	glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );

	glBindBuffer( GL_ARRAY_BUFFER, VBOCouleurs);
	glColorPointer( 3, GL_FLOAT, 0, (char *) NULL );

	glBindBuffer( GL_ARRAY_BUFFER, VBONormales);
	glNormalPointer(GL_FLOAT, 0, (char *) NULL );

		glDrawArrays(GL_TRIANGLES, 0, monobjet.nbfaces * 3);

glDisableClientState(GL_NORMAL_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);
glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawVertexArray() {
//On éteint la lumière pour profiter des couleurs...
glDisable(GL_LIGHTING);
glDisable(GL_LIGHT0);

glEnableClientState(GL_VERTEX_ARRAY);	//attention, on va se servir de sommets
glEnableClientState(GL_NORMAL_ARRAY);	//attention, on va se servir de normales
glEnableClientState(GL_COLOR_ARRAY);	//attention, on va se servir de couleurs

	glColorPointer(3, GL_FLOAT, 0, ColorArray);	//et voilà le buffer des couleurs, sur 3 float pour chaque couleur

	glNormalPointer(GL_FLOAT, 0, NormalArray);	//le buffer des normales, sur des floats (3 implicite)

	glVertexPointer(3, GL_FLOAT, 0, VertexArray);	//le buffer des sommets, 3 floats pour chaque coordoonées

		glDrawArrays(GL_TRIANGLES, 0, monobjet.nbfaces * 3);	//et hop, on envoie au rendu de la CG

glDisableClientState(GL_COLOR_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);
glDisableClientState(GL_VERTEX_ARRAY);

}

void DrawDisplayList() {
	//On allume la lumière pour voir l'effet des belles normales
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glCallList(madisplaylist);	//classique, on appelle la liste pré-compilée
}


void GlInit() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 1.0, 1.0);

	glShadeModel(GL_SMOOTH);

	//gestion de la lumière
	glEnable(GL_LIGHTING); 	// Active l'éclairage
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);  	//On applique les paramètres du matériau
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,MatDif);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,MatAmb);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,60);

	nbimages = 0;
	glutTimerFunc( 5000, callback_Timer, 0); //démarrage de la fontion action_timer dans 5s
}



void MyInit() {
	atexit(&BeforeReallyQuit); // fonction appellée dès que le signal QUIT est reçu

	srandom((unsigned int)(time(NULL))); // pour le générateur pseudo-aléatoire
}

static void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	float distance = 0.0f;
	float max = fmax(monobjet.max.x, monobjet.max.y);
	max = fmax(monobjet.max.z, max);

	//c'est nul de le faire à chaque image si l'objet ne change pas !
	distance = max / tan(M_PI / 4.0f);


	if (rendu == RENDU_DL) //rendu par Display liste, on aura besoin de la lumière
		glLightfv(GL_LIGHT0,GL_POSITION,Light1Pos);

	gluLookAt(0.0, 0.0, distance * 2.0f, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(angle, 1, 1, 1);
		glTranslatef(-monobjet.centroid.x, -monobjet.centroid.y, -monobjet.centroid.z);

	switch(rendu) {
		case RENDU_DL :
			DrawDisplayList();
		break;

		case RENDU_VA :
			DrawVertexArray();
		break;

		case RENDU_VBO :
			DrawVBO();
		break;
	}

	nbimages++;

	glutSwapBuffers();
}



static void callback_Window(GLsizei width, GLsizei height)
{
//au cas où h soit nul et entraîne une division par 0
	if(height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	windowWidth = width;
	windowHeight = height;

	windowRatio = float(windowWidth) / float(windowHeight);

	cout << "callback_Window - " << "new width " << windowWidth << " new height " << windowHeight << endl;

	// Set the correct perspective.
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, windowRatio, 1, 10000);

	glMatrixMode(GL_MODELVIEW);
}

static void callback_Idle() {
	angle += 0.05f; //pour la rotation de l'objet sur lui-même

	glutPostRedisplay();
}


static void callback_Keyboard(unsigned char key, int x, int y) {
switch (key) {
	case ECHAP:
		cout << "callback_Keyboard - " << "sortie de la boucle de rendu" << endl;
		glutLeaveMainLoop(); //retour au main()
		break;
	default:
		cerr << "La touche " << int(key) << " est non active." << endl;
		break;
	}
}

static void GlewInit() {
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
		exit(1);
	}

	cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
	//cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n";
}

static GLvoid InitializeGlutCallbacks()
{
	//quelle fonction est appelée au rendu ?
	glutDisplayFunc(RenderScene);

	//quelle fonction est appelée quand le GPU est en attente ?
	//fonction inutile, on utilise un timer pour bloquer le framerate
	glutIdleFunc(callback_Idle);

	//quelle fonction est appelée pour traiter les événements du clavier (classique) ?
	glutKeyboardFunc(callback_Keyboard);

	//quelle fonction est appelée pour traiter les alarmes ?
	//glutTimerFunc();à appeler lors de la mise en place en l'alarme

	//quelle fonction est appelée pour traiter les événements souris ?
	//glutMotionFunc(callback_Mouse);

	//quelle fonction est appelée pour traiter les événements sur la fenêtre ?
	glutReshapeFunc(callback_Window);

	//quelle fonction est appelée pour traiter les touches spéciales du clavier ?
	//glutSpecialFunc(&callback_SpecialKeys);
}

int main(int argc, char **argv) {
	//on traite la ligne de commande pour savoir quel type de rendu on fait
	if (argc == 2) {
		rendu = 0;
		//type de rendu = argv[1]
		if (strcmp (argv[1], "DL") == 0 )
			rendu = RENDU_DL;
		else if (strcmp (argv[1], "VA") == 0 )
			rendu = RENDU_VA;
		else if (strcmp (argv[1], "VBO") == 0 )
			rendu = RENDU_VBO;
	}
	else {
		cerr << "les options possibles sont " << argv[0] << " DL | VBA | VBO\n";
		return(1);
	}

	//on crée le contexte OpenGL et la fenêtre
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowHeight,windowWidth);
	glutCreateWindow("TP VertexBufferObject");

	//attachement des fonctions de l'OpenGL
	InitializeGlutCallbacks();

	MyInit();
	GlewInit();

	GlInit();
	GeomInit();

//	SetShaders();

	glutMainLoop();

	return 0;
}
