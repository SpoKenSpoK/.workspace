//RRAFFIN, Iut de provence, département Informatique, ARCHI IN GLSL
//romain.raffin[AT]univ-provence.fr

//une classe pour les VertexShader
#include <cstdlib>
#include <GL/glew.h>

#include "glsl_vs.h"
#include "fonctions.h"

GLSL_VS::GLSL_VS(void) {
	idvs = glCreateShader(GL_VERTEX_SHADER);
}

GLSL_VS::GLSL_VS(const GLSL_VS & _copie) {
	idvs = _copie.idvs;
}

int GLSL_VS::ReadSource(const char* _filename) {

char * pointeur_source;
const char * constVS;

	if (ReadSourceFile(_filename, &pointeur_source) > 0) {
		constVS = pointeur_source;
		glShaderSource(idvs, 1, &constVS, NULL);
		delete [] pointeur_source;
	}

return 0;
}

int GLSL_VS::Compile() {
	glCompileShader(idvs);
	return EXIT_SUCCESS;
}


