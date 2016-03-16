#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"

bool Texture::charger(const char* file_name, bool mipmap){
	img = stbi_load(file_name, &tWidth, &tHeight, &opp, 0);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	if( img!= NULL){
		switch (opp){
			case 3:
				if(mipmap)
					gluBuild2DMipmaps( GL_TEXTURE_2D, opp, tWidth, tHeight, GL_RGB, GL_UNSIGNED_BYTE, img);
				else
					glTexImage2D( GL_TEXTURE_2D, 0, opp, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

				return true;
				break;

			case 4:
				if(mipmap)
					gluBuild2DMipmaps( GL_TEXTURE_2D, opp, tWidth, tHeight, GL_RGBA, GL_UNSIGNED_BYTE, img);
				else
					glTexImage2D( GL_TEXTURE_2D, 0, opp, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

				return true;
				break;

			default:
				return false;
		}
	}
	else return false;
}

void Texture::utiliser() { glBindTexture(GL_TEXTURE_2D, id); }
void Texture::definir_filtrage(GLint mode_min, GLint mode_mag){
	utiliser();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode_mag);
}

void Texture::definir_bouclage(GLint mode_axe_s, GLint mode_axe_t){
	utiliser();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode_axe_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode_axe_t);
}

void Texture::definir_melange(GLint melange){
	utiliser();

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, melange);
}
