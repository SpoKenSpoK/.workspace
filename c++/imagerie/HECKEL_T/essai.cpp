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
  /*
  Vous pouvez modifier l'intérieur de ce main() comme bon vous semble. Cette
  fonction ne sera pas utilisée lors de la correction.
  */
  
  std::ifstream chatgray("chat.pgm", std::ios::binary);
  std::ifstream chat("chat_petit.ppm", std::ios::binary);
  std::ifstream chatd("chat_petit.ppm", std::ios::binary);
  std::ifstream image("chat.ppm", std::ios::binary);

  ColorImage* cat = ColorImage::readPPM(chat);
  ColorImage* gato = ColorImage::readPPM(chatd);
  ColorImage* im = ColorImage::readPPM(image);
  GrayImage* gray = GrayImage::readPGM(chatgray);

  std::ofstream chatsimple("chatsimple.ppm", std::ios::binary);
  std::ofstream chatdouble("chatdouble.ppm", std::ios::binary);
  std::ofstream chatrec("chatrectangle.ppm", std::ios::binary);
  std::ofstream wgray("chatgray.pgm", std::ios::binary);

  Color color(250,0,0);
  Color coloro(0,250,0);
  Color colorp(0,0,250);
  im->rectangle(5,5,310,230,color);
  im->rectangle(10,10,300,220,coloro);
  im->rectangle(15,15,290,210,colorp);

  cat = cat->bilinearScale(823, 400);
  cat->writePPM(chatdouble);

  gato = gato->simpleScale(823, 400);
  gato->writePPM(chatsimple);

  im->writePPM(chatrec);

  gray = gray->simpleScale(1500,1000);
  gray->writePGM(wgray);

  const char* imgJpeg = "Image.jpg";
  im->writeJPEG(imgJpeg);

  const char* jpegin = "out_100.jpg";
  ColorImage* picjpegin = ColorImage::readJPEG(jpegin);

  const char* jpegout = "test_jpeg_out.jpg";
  picjpegin->writeJPEG(jpegout,100);

  for(unsigned int quality=0;quality<=100;quality+=5)  {
    std::ostringstream oss; // Variable pour former le nom de chaque fichier.
    oss << "out_" << std::setfill('0') << std::setw(3) << quality << ".jpg";
    im->writeJPEG(oss.str().c_str(),quality);
  }

  chatsimple.close();
  chatdouble.close();
  chatrec.close();
  chat.close();
  chatd.close();
  image.close();
  chatgray.close();
  wgray.close();  

  delete cat;
  delete gato;
  delete im;
  delete gray;

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

