#include "Image.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <setjmp.h>
extern "C" {
    #include <jpeglib.h>
}

const char *identifier="heckel_t";
const char *informations=
 "METTEZ ICI VOS COMMENTAIRES\n"
 "Éventuellement sur plusieurs lignes.\n"
 "En n'oubliant pas les guillemets, ni les backslash-n pour passer à la ligne.\n";

GrayImage::GrayImage(ushort w, ushort h)
	:width(w), height(h), array(0)
{ array = new ubyte[width*height]; }

GrayImage::GrayImage()
	:array(0)
{ throw std::runtime_error("It's ilegal to create a GrayImage's instance without parameters."); }

GrayImage::GrayImage(const GrayImage& source)
	:width(source.width), height(source.height), array(new ubyte[width*height])
{ for(uint i=0; i<uint(width*height); ++i) array[i]=source.array[i]; }

void GrayImage::clear(ubyte gray){
	for(uint i=0; i<uint(width*height);++i)
		array[i]=gray;
}

void GrayImage::writeRAW(std::ostream& f){
	ubyte tmp[4];
	// poids faibles  |  poids forts
	tmp[0]=width%256;	tmp[1]=width>>8;
	tmp[2]=height%256;	tmp[3]=height>>8;
	f.write((const char*)tmp, 4);
	f.write((const char*)array, width*height);
}

void GrayImage::writePGM(std::ostream& f){
	f<<"P5\n"; // Magic Number
	f<<"# Commentaire \n";
	f<<width<<" "<<height<<std::endl; // Utilisation du cin car il s'agit de texte
	f<<"# Commentaire 2 \n";
	f<<"255\n"; // Valeur max des niveaux de gris
	//> No more comments here
	f.write((const char*)array, width*height);
}

GrayImage* GrayImage::readPGM(std::istream& is){
	ushort width, height;
	ubyte gray;
	std::string s;

	is>>s;
	GrayImage::skip_line(is);
	GrayImage::skip_comments(is);
	is>>width>>height;
	GrayImage::skip_line(is);
	GrayImage::skip_comments(is);
	is>>gray;
	GrayImage::skip_line(is);

	GrayImage* i = new GrayImage(width, height);

	if(s == "P5") is.read((char*)i->array, width*height);

	else if(s == "P2"){
		for (ushort j = 0; j < height; j++)
			for (ushort k = 0; k < width; k++){
				int u;
				is >> u;
				i->pixel(k, j) = ubyte(u);
			}
	}

	return i;
}

void GrayImage::fillRectangle(ushort x, ushort y, ushort w, ushort h, ubyte color){
	for(uint i=y; i< y+w ; ++i)
		for(uint j=x; j < x+h; ++j)
			pixel(i,j)=color;
}

void GrayImage::rectangle(ushort x, ushort y, ushort w, ushort h, ubyte color){
	for(uint i=x; i < uint(x+w+1) ; ++i){
		pixel(i,y)=color;
		pixel(i,y+h)=color;
	}

	for(uint j=y+1; j < y+h ; ++j){
		pixel(x,j)=color;
		pixel(x+w,j)=color;
	}
}

// skip_line(is) permet de sauter toute une série d'octets de "is" jusqu'à trouver un '\n'
void GrayImage::skip_line(std::istream& is){
	char c;
	// Lire un caractère
	do{ is.get(c); } while(c!='\n'); // tant qu'on n'atteint pas la fin de ligne.
}

// skip_comments(is) utilise la fonction précédente pour sauter zéro, une ou plusieurs lignes
// de commentaires débutées par '#' et allant jusqu'à '\n'.
void GrayImage::skip_comments(std::istream& is){
	char c;
	is.get(c);       // Lire un caractère.
	// Tant que c'est un '#'.
	while(c=='#'){
		skip_line(is); // On élimine les caractères jusqu'à la fin de ligne,
		is.get(c);     // Et on lit un nouveau caractère.
	}
	is.putback(c);   // On remet le caractère lu puisqu'il n'est pas un '#'.
}

GrayImage* GrayImage::simpleScale(ushort w, ushort h) const{
	GrayImage* iprime = new GrayImage(w,h);

	for(ushort yprime = 0; yprime<h; ++yprime)
		for(ushort xprime = 0; xprime<w; ++xprime){
			double x = (double(xprime)/w)* width;
			double y = (double(yprime)/h)* height;

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			iprime->pixel(xprime, yprime) = pixel(xi, yi);
		}

	return iprime;
}

GrayImage* GrayImage::bilinearScale(ushort w, ushort h) const{
	GrayImage* iprime = new GrayImage(w,h);

	for(ushort yprime=0; yprime<h; ++yprime)
		for(ushort xprime=0; xprime<w; ++xprime){
			double x = (double(xprime)/w)* width;
			double y = (double(yprime)/h)* height;

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			ushort xiPone = (xi+1 >= width ? xi : xi+1);
			ushort yiPone = (yi+1 >= height ? yi : yi+1);

			double lambdA = x - xi;
			double mU = y - yi;

			iprime->pixel(xprime, yprime)=(1-lambdA)*(1-mU)*pixel(xi, yi)
										 +(1-lambdA)*mU*pixel(xi, yiPone)
										 +lambdA*(1-mU)*pixel(xiPone, yi)
										 +lambdA*mU*pixel(xiPone, yiPone);
		}

	return iprime;
}

//_______________________________________________________

Color::Color(ubyte r, ubyte g, ubyte b)
	:red(r), green(g), blue(b)
{}

Color::~Color() {}

//_______________________________________________________

ColorImage::ColorImage(ushort w, ushort h)
	:width(w), height(h), array(0)
{ array = new Color[width*height]; }

void ColorImage::clear(Color color){
	for(uint i=0; i<uint(width*height);++i)
		array[i]= color;
}

ColorImage::ColorImage()
	:array(0)
{ throw std::runtime_error("It's ilegal to create a ColorImage's instance without parameters."); }

ColorImage::ColorImage(const ColorImage& source)
	:width(source.width), height(source.height), array(new Color[width*height])
{ for(uint i=0; i<uint(width*height); ++i) array[i]=source.array[i]; }

void ColorImage::writePPM(std::ostream& f) const{
	f<<"P6\n"; // Magic Number
	f<<"# Commentaire \n";
	f<<width<<" "<<height<<std::endl; // Utilisation du cin car il s'agit de texte
	f<<"# Commentaire 2 \n";
	f<<"255\n"; // Valeur max des niveaux de gris
	//> No more comments here
	for(ushort j=0; j<height; ++j)
		for(ushort i=0; i<width; ++i)
			f<<pixel(i,j).getRed()<<pixel(i,j).getGreen()<<pixel(i,j).getBlue();

}


ColorImage* ColorImage::readPPM(std::istream& is){
	ushort width, height;
	ubyte gray;
	std::string s;

	is>>s;
	GrayImage::skip_line(is);
	GrayImage::skip_comments(is);
	is>>width>>height;

	GrayImage::skip_line(is);
	GrayImage::skip_comments(is);
	is>>gray;
	GrayImage::skip_line(is);

	ColorImage* i = new ColorImage(width, height);

	if(s == "P6"){
		for (ushort j=0; j < height; j++)
			for (ushort k=0; k < width; k++){
				ubyte r,g,b;
				r = is.get();	// Lit des chars
				g = is.get();
				b = is.get();

				i->pixel(k,j).setRed(r);
				i->pixel(k,j).setGreen(g);
				i->pixel(k,j).setBlue(b);
			}
	}

	else if(s == "P3"){
		for (ushort j = 0; j < height; j++)
			for (ushort k = 0; k < width; k++){
				int r,g,b;
				is >> r >> g >> b;	// Lit des ASCII

				i->pixel(k, j) = ubyte(r);
				i->pixel(k, j) = ubyte(g);
				i->pixel(k, j) = ubyte(b);
			}
	}
	else std::runtime_error("Wrong image format");


	return i;
}


void ColorImage::fillRectangle(ushort x, ushort y, ushort w, ushort h, Color color){
	for(uint i=y; i< y+w ; ++i)
		for(uint j=x; j < x+h; ++j)
			pixel(i,j)=color;
}

void ColorImage::rectangle(ushort x, ushort y, ushort w, ushort h, Color color){
	for(uint i=x; i < uint(x+w+1) ; ++i){
		pixel(i,y)=color;
		pixel(i,y+h)=color;
	}

	for(uint j=y+1; j < y+h ; ++j){
		pixel(x,j)=color;
		pixel(x+w,j)=color;
	}
}

ColorImage* ColorImage::simpleScale(ushort w, ushort h) const{
	ColorImage* iprime = new ColorImage(w,h);

	for(ushort yprime = 0; yprime<h; ++yprime)
		for(ushort xprime = 0; xprime<w; ++xprime){


			double x = (double(xprime)/w)* width;
			double y = (double(yprime)/h)* height;

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			iprime->pixel(xprime, yprime) = pixel(xi, yi);
		}

	return iprime;
}

ColorImage* ColorImage::bilinearScale(ushort w, ushort h) const{
	ColorImage* iprime = new ColorImage(w,h);

	for(ushort yprime=0; yprime<h; ++yprime)
		for(ushort xprime=0; xprime<w; ++xprime){
			double x = ((double(xprime+0.5)/w)* width)-0.5;
			double y = ((double(yprime+0.5)/h)* height)-0.5;

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			ushort xiPone = (xi+1 >= width ? xi : xi+1);
			ushort yiPone = (yi+1 >= height ? yi : yi+1);

			double lambdA = x - xi;
			double mU = y - yi;

			if(lambdA < 0) lambdA *= -1;
			if(mU < 0) mU *= -1;

			iprime->pixel(xprime, yprime).setRed( (1-lambdA)*(1-mU)*pixel(xi, yi).getRed()
										 		+(1-lambdA)*mU*pixel(xi, yiPone).getRed()
										 		+lambdA*(1-mU)*pixel(xiPone, yi).getRed()
										 		+lambdA*mU*pixel(xiPone, yiPone).getRed() );

			iprime->pixel(xprime, yprime).setGreen( (1-lambdA)*(1-mU)*pixel(xi, yi).getGreen()
										 		+(1-lambdA)*mU*pixel(xi, yiPone).getGreen()
										 		+lambdA*(1-mU)*pixel(xiPone, yi).getGreen()
										 		+lambdA*mU*pixel(xiPone, yiPone).getGreen() );

			iprime->pixel(xprime, yprime).setBlue( (1-lambdA)*(1-mU)*pixel(xi, yi).getBlue()
										 		+(1-lambdA)*mU*pixel(xi, yiPone).getBlue()
										 		+lambdA*(1-mU)*pixel(xiPone, yi).getBlue()
										 		+lambdA*mU*pixel(xiPone, yiPone).getBlue() );
		}

	return iprime;
}

void ColorImage::writeJPEG(const char* fname, unsigned int quality) const{
    struct jpeg_compress_struct cinfo; // Paramètres de notre image JPEG
	struct jpeg_error_mgr jerr; // Vérifie le bon fonctionnement de la compréssion "Error Handler", pas de traduction française trouvée pour mieux décrire ce mot.

    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
    /* Now we can initialize the JPEG compression object. */

    if ((outfile = fopen(fname, "wb")) == NULL) {
        throw std::runtime_error("Image can't open");
    }

	jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; 	/* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
    
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    int row_stride = width;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = (JSAMPROW) &array[cinfo.next_scanline * row_stride] ;
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}
