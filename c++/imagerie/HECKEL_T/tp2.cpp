#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

/*
 Toutes vos définitions de classes doivent être regroupées dans l'unique
 fichier "Image.h". Son nom n'est pas modifiable sinon vous empêcherez la
 compilation de l'autre executable servant à la correction.
*/
#include "Image.h"

extern const char *identifier;   // Ne pas modifier ces deux lignes ! Vos changements au sujet
extern const char *informations; // de ceux deux constantes doivent être faits dans Image.cpp.


int main(int argc,char *argv[])
{
 cout << "Votre identifiant tel que declare dans Image.cpp : " << identifier << endl;
 cout << "Les informations que vous avez decide d'indiquer au correcteur : " << endl << informations << endl;
 try
  {

   //pgm - pgm
   std::ifstream h("chat.pgm", std::ios::binary);
   std::ofstream k("chatpgm_pgm.pgm", std::ios::binary);
   GrayImage* j = GrayImage::readPGM(h);
   j->writePGM(k);
   delete j;
   k.close();
   h.close();

   //redim simple pgm
   h.open("chat.pgm", std::ios::binary);
   k.open("chat_simple.pgm", std::ios::binary);
   j = GrayImage::readPGM(h);
   GrayImage* j2 = j->simpleScale(9999,9999);
   j2->writePGM(k);
   delete j;
   delete j2;
   k.close();
   h.close();

   //redim bilinear pgm
   h.open("chat.pgm", std::ios::binary);
   k.open("chat_bilinear.pgm", std::ios::binary);
   j = GrayImage::readPGM(h);
   j2 = j->bilinearScale(9999,9999);
   j2->writePGM(k);
   delete j;
   delete j2;
   k.close();
   h.close();

   //ppm - ppm
   std::ifstream f("chat.ppm", std::ios::binary);
   std::ofstream g("chatppm_ppm.ppm", std::ios::binary);
   ColorImage* i = ColorImage::readPPM(f);
   i->writePPM(g);
   delete i;
   g.close();
   f.close();

   // ppm - jpeg
   f.open("chat.ppm", std::ios::binary);
   i = ColorImage::readPPM(f);
   i->writeJPEG("chatppm_jpeg.jpeg");
   delete i;
   f.close();

   //jpeg - ppm
   g.open("chatjpeg_ppm.ppm", std::ios::binary);
   i = ColorImage::readJPEG("chatppm_jpeg.jpeg");
   i->writePPM(g);
   delete i;
   g.close();

   //ppm - tga
   f.open("chat.ppm", std::ios::binary);
   g.open("chatppm_tga.tga", std::ios::binary);
   i = ColorImage::readPPM(f);
   i->writeTGA(g, false);
   delete i;
   g.close();
   f.close();

   //ppm - tga
   f.open("chat.tga", std::ios::binary);
   g.open("chat_tga_tga.tga", std::ios::binary);
   i = ColorImage::readTGA(f);
   i->writeTGA(g, false);
   delete i;
   g.close();
   f.close();

  /* //ppm - tgarle
   f.open("chat.ppm", std::ios::binary);
   g.open("chatppm_tgarle.tga", std::ios::binary);
   i = ColorImage::readPPM(f);
   i->writeTGA(g);
   delete i;
   g.close();
   f.close(); */

   //tga - ppm
   f.open("chatppm_tga.tga", std::ios::binary);
   g.open("chattga_ppm.ppm", std::ios::binary);
   i = ColorImage::readTGA(f);
   i->writePPM(g);
   delete i;
   g.close();
   f.close();

   //tgarle - ppm
  /* f.open("chatppm_tgarle.tga", std::ios::binary);
   g.open("chattgarle_ppm.ppm", std::ios::binary);
   i = ColorImage::readTGA(f);
   i->writePPM(g);
   delete i;
   g.close();
   f.close();*/


   //redim simple
   f.open("chat_petit.ppm", std::ios::binary);
   g.open("chat_simple.ppm", std::ios::binary);
   i = ColorImage::readPPM(f);
   ColorImage* i2 = i->simpleScale(1000,1000);
   i2->writePPM(g);
   delete i;
   delete i2;
   g.close();
   f.close();

   //redim bilinear
   f.open("chat_petit.ppm", std::ios::binary);
   g.open("chat_bilinear.ppm", std::ios::binary);
   i = ColorImage::readPPM(f);
   i2 = i->bilinearScale(1000,1000);
   i2->writePPM(g);
   delete i;
   delete i2;
   g.close();
   f.close();

   //rectangle
   f.open("chat.ppm", std::ios::binary);
   g.open("chat_rec.ppm", std::ios::binary);
   i = ColorImage::readPPM(f);
   i->rectangle(10,10,100,100,Color(100,100,100));
   i->writePPM(g);
   delete i;
   g.close();
   f.close();

   //fill rectangle
   f.open("chat.ppm", std::ios::binary);
   g.open("chat_fillrec.ppm", std::ios::binary);
   i = ColorImage::readPPM(f);
   i->fillRectangle(10,10,50,50,Color(100,100,100));
   i->writePPM(g);
   delete i;
   g.close();
   f.close();

  } // Trois types d'exceptions seront attrapés (les chaines C et C++ ainsi que
    // les std::exception et toutes ses dérivées). N'utilisez pas autre chose !
 catch(exception& e)
  { cerr<< "Exception :"<<e.what()<< endl; }
 catch(string& e)
  { cerr<< "Exception string :"<<e<< endl; }
 catch(const char * e)
  { cerr<< "Exception const char* :"<<e<< endl; }
 return 0;
}
