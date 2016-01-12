#include <iostream>
#include "Image.h"

#include <iomanip>
#include <sstream>


int main(){
	std::ifstream chat("chat_petit.ppm", std::ios::binary);
	std::ifstream chatd("chat_petit.ppm", std::ios::binary);
	std::ifstream image("chat.ppm", std::ios::binary);

	ColorImage* cat = ColorImage::readPPM(chat);
	ColorImage* gato = ColorImage::readPPM(chatd);
	ColorImage* im = ColorImage::readPPM(image);

	std::ofstream chatsimple("chatsimple.ppm", std::ios::binary);
	std::ofstream chatdouble("chatdouble.ppm", std::ios::binary);
	std::ofstream chatrec("chatrectangle.ppm", std::ios::binary);

	Color color(250,0,0);
	Color coloro(0,250,0);
	Color colorp(0,0,250);
	im->rectangle(2,2,320,215,color);
	im->rectangle(4,4,71,51,coloro);
	im->rectangle(6,6,67,47,colorp);

	cat = cat->bilinearScale(823, 400);
	cat->writePPM(chatdouble);

	gato = gato->simpleScale(823, 400);
	gato->writePPM(chatsimple);

	im->writePPM(chatrec);

	const char* imgJpeg = "Image.jpeg";
	im->writeJPEG(imgJpeg);

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

    delete cat;
    delete gato;
	delete im;

	return 0;
}
