#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>

#define CORR_PGMASCII
#define CORR_PPMASCII
#define CORR_READCOLORJPEG
#define CORR_READCOLORTGA

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
	void fillRectangle(ushort x, ushort y, ushort w, ushort h, ubyte gray=0);
	void rectangle(ushort x, ushort y, ushort w, ushort h, ubyte gray=0);

	// Ecriture & Lecture d'image
	void writeRAW(std::ostream& );	// Ecriture RAW 'brute'
	void writePGM(std::ostream& );	// Ecriture dans un fichier PGM
	static GrayImage* readPGM(std::istream& ); // Lecture d'un fichier PGM

	static void skip_line(std::istream& );
	static void skip_comments(std::istream& );

	GrayImage* simpleScale(ushort w, ushort h) const;
	GrayImage* bilinearScale(ushort w, ushort h) const;
};

// -------------------- Color -------------------------------------------

class Color{
private:
	ubyte red;
	ubyte green;
	ubyte blue;

public:
	Color(ubyte r=0, ubyte g=0, ubyte b=0);
	~Color();

	inline ubyte getRed() const { return red; }
	inline ubyte getGreen() const { return green; }
	inline ubyte getBlue() const { return blue; }

	inline void setRed(const ubyte& r) { red = r; }
	inline void setGreen(const ubyte& g) { green = g; }
	inline void setBlue(const ubyte& b) { blue = b; }
};

// ------------------- Color Image --------------------------------------

class ColorImage{

private:
	ushort width;
	ushort height;
	Color* array;

public:
	ColorImage(ushort w, ushort h);
	inline ~ColorImage() { delete [] array; }
	ColorImage(); 									// Constructeur par défaut
	ColorImage(const ColorImage& source);			// Constructeur par copie

	// Différents accesseurs et mutateurs + surcharge opérateur () (foncteur)
	inline const Color& pixel(ushort x, ushort y) const { return array[y*width + x]; }
	inline Color& pixel(ushort x, ushort y) { return array[y*width + x]; }
	inline const Color& operator() (ushort x, ushort y) const { return array[y*width + x]; }
	inline Color& operator () (ushort x, ushort y) { return array[y*width + x]; }

	// Dessin
	void clear(Color c);	// Tous les pixels de l'image prennent une color RGB
	void fillRectangle(ushort x, ushort y, ushort w, ushort h, Color c);
	void rectangle(ushort x, ushort y, ushort w, ushort h, Color c);

	// Ecriture & Lecture d'image
	void writePPM(std::ostream& ) const;	// Ecriture dans un fichier PPM
	static ColorImage* readPPM(std::istream& ); // Lecture d'un fichier PPM

	ColorImage* simpleScale(ushort w, ushort h) const;
	ColorImage* bilinearScale(ushort w, ushort h) const;

	void writeJPEG(const char*, unsigned int quality=75 ) const;
	static ColorImage* readJPEG(const char* );

	void writeTGA(std::ostream&, bool =true ) const;
	static ColorImage* readTGA(std::ifstream& );
};

#endif //IMAGE_HPP
