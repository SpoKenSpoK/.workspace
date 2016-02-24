#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "stb_image.h"
#include <GL/glu.h>

class Texture {
public:
	unsigned char* img;
	GLuint id;
	int tWidth;
	int tHeight;
	int opp;

	inline ~Texture() { delete [] img; }
	bool charger(const char*, bool =false );
	void utiliser();
	void definir_filtrage(GLint , GLint);
	void definir_bouclage(GLint , GLint);
	void definir_melange(GLint );
};

#endif
