//RRAFFIN, Iut de provence, département Informatique, ARCHI IN GLSL
//romain.raffin[AT]univ-provence.fr

//une classe pour les fragment shaders
#ifndef _GLSL_FS
#define _GLSL_FS

#include <GL/gl.h>

class GLSL_FS {
public:
	GLuint idfs;

	GLSL_FS(void);
	GLSL_FS(const GLSL_FS&);

	int ReadSource(const char *);
	int Compile();
};

#endif

