//RRAFFIN, Iut de provence, département Informatique, ARCHI IN GLSL
//romain.raffin[AT]univ-provence.fr

//quelques fonctions utiles pour les shaders (lecture de fichier, affichage d'erreurs GLSL, chargement d'une texture par Freeimage)
#ifndef _H_FONCTIONS
#define _H_FONCTIONS

#include <GL/gl.h>
#include <iostream>

void PrintProgramInfo(GLuint);
void PrintShaderInfo(GLuint);
int ReadSourceFile(const char*, char **);
void LoadTexture(std::string *, GLuint*, unsigned int *, unsigned int *);

#endif
