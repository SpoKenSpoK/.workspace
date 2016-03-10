//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//une classe pour construire des shader program

#ifndef _H_GLSL_PROGRAM
#define _H_GLSL_PROGRAM

#include <GL/gl.h>

#include "glsl_fs.h"
#include "glsl_vs.h"

class GLSL_Program {

public:

	GLuint idprogram;
	GLSL_Program(void);
	GLSL_Program(const GLSL_Program&);
	~GLSL_Program();

	void Use_VertexShader(GLSL_VS&);
	void Use_FragmentShader(GLSL_FS&);
	void Link_Shaders();
	void Activate();
	void Deactivate();
};

#endif
