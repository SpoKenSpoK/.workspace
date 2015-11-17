#ifndef GRAYIMAGE_HPP
#define GRAYIMAGE_HPP

#include <iostream>
#include <fstream>

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char ubyte;

class GrayImage{
private:
	ushort width;	// Largeur
	ushort height;	//	Hauteur
	ubyte* array;	// Pointeur sur char(ubyte), alloue par la suite un tableau dynamique

public:
	GrayImage(ushort width, ushort height);	// Constructeur
	GrayImage(); 	// Constructeur par défaut
	GrayImage(const GrayImage& source);		// Constructeur par copie
	inline ~GrayImage() { delete [] array; }	// Destructeur
	
	// Différents accesseurs et mutateurs + surcharge opérateur () (foncteur)
	inline const ubyte& pixel(ushort x, ushort y) const { return array[y*width + x]; }	
	inline ubyte& pixel(ushort x, ushort y) { return array[y*width + x]; }
	inline const ubyte& operator() (ushort x, ushort y) const { return array[y*width + x]; }
	inline ubyte& operator () (ushort x, ushort y) { return array[y*width + x]; }
	
	// Dessin
	void clear(ubyte =0);	// Tous les pixels de l'image prennent la valeur ubyte donnée
	void fillRectangle(ushort x, ushort y, ushort w, ushort h, ubyte color=0);
	void rectangle(ushort x, ushort y, ushort w, ushort h, ubyte color=0);
	
	// Ecriture & Lecture d'image
	void writeRAW(std::ostream& );	// Ecriture RAW 'brute'
	void writePGM(std::ostream& );	// Ecriture dans un fichier PGM
	static GrayImage* readPGM(std::istream& ); // Lecture d'un fichier PGM
	
	static void skip_line(std::istream& );
	static void skip_comments(std::istream& );

	GrayImage* simpleScale(ushort w, ushort h) const;
	GrayImage* bilinearScale(ushort w, ushort h) const;
};

#endif //GRAYIMAGE_HPP
