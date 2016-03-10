//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//des fonctions pour interfacer GLEW avec nos progs
#ifndef _H_FONCTIONS
#define _H_FONCTIONS

#include <GL/gl.h>

void PrintProgramInfo(GLuint);
void PrintShaderInfo(GLuint);
int ReadSourceFile(const char*, char **);

#endif
