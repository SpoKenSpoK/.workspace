#include "Image.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <setjmp.h>
extern "C" {
    #include <jpeglib.h>
}

const char *identifier="HECKEL Thibault";
const char *informations=
 "J'ai traite le probleme de decalage qui pouvait operer entre bilinearScale et simpleScale dans la colorImage.\n"
  "Celui-ci est explique dans les commentaires a l'endroit voulu. \n";

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
    else throw std::runtime_error("Error in GrayImage::readPGM : Wrong image format.");

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
			double x = ((double(xprime+0.5)/w)* width)-0.5;
			double y = ((double(yprime+0.5)/h)* height)-0.5;

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			ushort xiPone = (xi+1 >= width ? xi : xi+1);
			ushort yiPone = (yi+1 >= height ? yi : yi+1);

			double lambdA = x - xi;
			double mU = y - yi;

            if(mU < 0) mU *= -1;
            if(lambdA < 0) lambdA *= -1;

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

void Color::fromCMY(ubyte c, ubyte m, ubyte y){
    red = 1-c;
    green = 1-m;
    blue = 1-y;
}

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

				i->pixel(k, j) = ubyte(r); // Convertion en unsigned char
				i->pixel(k, j) = ubyte(g);
				i->pixel(k, j) = ubyte(b);
			}
	}
	else throw std::runtime_error("Error in ColorImage::readPPM : Wrong image format.");

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
			double x = ((double(xprime+0.5)/w)* width)-0.5; // Un décalage de positions des pixels entre le bilinearScale et simpleScale apparait visuellement sur les deux images
			double y = ((double(yprime+0.5)/h)* height)-0.5; // Pour le réduire il nous est possible d'ajouter 0.5 d'un côté puis de l'enlever sur la fin du calcul

			ushort xi=ushort(x);
			ushort yi=ushort(y);

			ushort xiPone = (xi+1 >= width ? xi : xi+1);
			ushort yiPone = (yi+1 >= height ? yi : yi+1);

			double lambdA = x - xi;
			double mU = y - yi;

			if(lambdA < 0) lambdA *= -1; // En effectuant le premier décalage sur x et y des pixels apparaissent blanc, cela est du au fait qu'ils soient devenu négatif
			if(mU < 0) mU *= -1;         // Pour y répondre il suffit de multiplier par -1 (mettre au carré, puis en faire la racine carré revient au même) pour obtenir la valeur absolue de lambdA et mU
            // C'est avec cette valeur qu'au final nous allons travailler. Après application des changements les pixels précédemments blanc ont disparu.

            iprime->pixel(xprime, yprime) = (1-lambdA)*(1-mU)*pixel(xi, yi)
									        + (1-lambdA)*mU*pixel(xi, yiPone)
									        + lambdA*(1-mU)*pixel(xiPone, yi)
        								    + lambdA*mU*pixel(xiPone, yiPone);
        }

	return iprime;
}

void ColorImage::writeJPEG(const char* fname, unsigned int quality) const{
    struct jpeg_compress_struct cinfo; // Paramètres de notre image JPEG
	struct jpeg_error_mgr jerr; // Vérifie le bon fonctionnement de la compréssion : "Error Handler", pas de traduction française trouvée pour mieux décrire ce mot.

    FILE * outfile; // Fichier de sortie
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */

    cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

    if ((outfile = fopen(fname, "wb")) == NULL) {       // Ouverture en écriture
        throw std::runtime_error("Error in ColorImage::writeJPEG : Image can't open.");
    }

	jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; 	/* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    unsigned int row_stride = width;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = (JSAMPROW) &array[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

ColorImage* ColorImage::readJPEG(const char* fname){
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE * infile;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((infile = fopen(fname, "rb")) == NULL)
        throw std::runtime_error("Error in ColorImage::readJPEG : Impossible to open the picture.");

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    ushort width = cinfo.output_width; // Récupération de la hauteur et largeur de l'image ouverte
    ushort height = cinfo.output_height;

    ColorImage* jpegout = new ColorImage(width, height);

    unsigned int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)                  /**Je ne sais pas exactement ce que produit en mémoire ce codes */
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height){
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        for(ushort x=0; x < width; ++x){
            /** cinfo.output_scanline-1 car un décalage de 1 pixel apparaît en haut de l'image */
            jpegout->pixel(x,cinfo.output_scanline-1).setRed( buffer[0][cinfo.output_components * x]);
            jpegout->pixel(x,cinfo.output_scanline-1).setGreen( buffer[0][cinfo.output_components * x+1]);
            jpegout->pixel(x,cinfo.output_scanline-1).setBlue( buffer[0][cinfo.output_components * x+2]);
            /** idée trouvée sur le net ( surtout le buffer[0][...] ) : http://stackoverflow.com/questions/694080/how-do-i-read-jpeg-and-png-pixels-in-c-on-linux */
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    return jpegout;
}


void ColorImage::writeTGA(std::ostream& f, bool rle) const{
    if(rle)
        throw std::runtime_error("Error in ColorImage::writeTGA : RLE not done yet sorry");

    // Soit un tableau de ubyte nommé "offset" représentant les composants de l'image "Unmapped RGB"
    // Nous pouvons compter 18 éléments, ayant chacun une longueur différente
    ubyte offset[18];

    offset[0] = 0; // "Une valeur de 0 veut dire que l'image n'a pas de code d'identification"
    offset[1] = 0; // Nous travaillons avec un type "Unmapped", soit "sans map", nous mettons donc 0
    offset[2] = 2; // Représente le type de l'image - ici "Unmapped RGB", soit de type "2"

    offset[3] = 0;  // En relation direct avec le offset[1], plus précisement la ColorMap
    offset[4] = 0;  // N'en ayant pas demandé, ces paramètres (du 3 au 7), sont inutiles.
    offset[5] = 0;  // Ils sont donc tous égaux à 0
    offset[6] = 0;
    offset[7] = 0;

    offset[8] = 0; // Origine X de notre image
    offset[9] = 0;
    offset[10] = 0; // Origine Y de notre image
    offset[11] = 0;

    /**
    Nous travaillons avec des ubytes mais nous avons un ushort pour la taille et la largeur.
    Soit notre ushort : 0000 0000 0000 0000 et un ubyte 0000 0000.
    On doit donc d'abord travailler sur la première partie de notre ushort en le transformant en ubyte.
    Puis nous décallons la deuxième partie de notre ushort vers la droite de façon à pouvoir le convertir lui aussi en ubyte
    */
    // Largeur de notre image :
    offset[12] = width%256; // (0000 0000 0000 0000 & 1111 1111 = 0000 0000 par exemple)
	offset[13] = (width >> 8)%256; // Décalage de 8 bits vers la droite ( 1111 0000 0000 0000, devient 0000 0000 1111 0000) puis & logique encore une fois
    /** Au final notre largeur vaut offset[12]+offset[13]*256 */

    // Hauteur de notre image
    offset[14] = height%256;
	offset[15] = (height >> 8)%256;

	offset[16] = 24; // Nombre de bits dans un pixel, ici 3*8 = 24 => nous travaillons avec du Targa 24
    offset[17] = 0; // Nous permet notamment de choisir l'origine de notre image. J'ai choisi ici de rester en bas à gauche.

    f.write((char*)offset, 18); // Ecriture des paramètres

	// Création d'un tableau temporaire pour écrire d'un seul coup un pixel
	ubyte tmp[3];

    // Notre image se dessine ici bien de bas en haut
	for(int i=height-1; i>=0; --i)
		for(int j=0; j<width; ++j){
			tmp[2] = pixel(j,i).getRed();
			tmp[1] = pixel(j,i).getGreen();
			tmp[0] = pixel(j,i).getBlue();
			f.write((char*)tmp, 3); // Ecriture des pixels, un à un
		}
}

ColorImage* ColorImage::readTGA(std::ifstream& is){
	ubyte offset[18];
	is.read((char*)offset,18);
	ushort width = offset[12] + (offset[13]*256);
	ushort height = offset[14] + (offset[15]*256);

	ColorImage* outtga = new ColorImage(width, height);

    if( offset[16] == 24){
    	if(offset[2] == 2){
    		ubyte tmp[3];

            if(offset[17] == 0){ // Test pour savoir si l'on lis l'image de bas en haut, ou de haut en bas
                for(int i=height-1; i>=0; --i)
        			for(int j=0; j<width; ++j){
        				is.read((char*)tmp, 3); // Lecture des trois couleur RGB du pixel (On avance 3 par 3)
        				outtga->pixel(j,i).setRed( tmp[2] );
        				outtga->pixel(j,i).setGreen( tmp[1] );
        				outtga->pixel(j,i).setBlue( tmp[0] );
        			}
            }
            else{
                for(int i=0; i<height; ++i)
        			for(int j=0; j<width; ++j){
        				is.read((char*)tmp, 3); // Lecture des trois couleur RGB du pixel (On avance 3 par 3)
        				outtga->pixel(j,i).setRed( tmp[2] );
        				outtga->pixel(j,i).setGreen( tmp[1] );
        				outtga->pixel(j,i).setBlue( tmp[0] );
        			}
            }
    	}
        else
            throw std::runtime_error("Error in ColorImage::readTGA : The RGB data type of this picture isn't 2.");
    }
    else
        throw std::runtime_error("Error in ColorImage::readTGA : This picture isn't Targa 24 - More or less than 24 bits per pixel here.");

	return outtga;
}

ColorImage* ColorImage::readMaison(std::istream& is){
    //int ascii;

    ColorImage* pic = new ColorImage(0,0);
    return pic;


}

ColorImage * ColorImage::anaglyphe() const{
    /*std::ifstream scu("sculpture.ppm", std::ios::binary);

    this->readPPM(scu); */

    ColorImage* newpic = new ColorImage(width/2, height);

    /*for(int i=0; i<height; ++i)
        for(int j=0; j<width/2; ++j){
            //newpic->pixel(j,i).setRed( );
            ubyte g = this->pixel( (j+width/2), i ).getGreen();
            ubyte b = this->pixel( (j+width/2), i ).getBlue();
            newpic->pixel(j,i).setGreen( g );
            newpic->pixel(j,i).setBlue( b );
        }
*/
    return newpic;

}
