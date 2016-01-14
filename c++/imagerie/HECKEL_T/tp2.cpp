#include <iostream>
#include "Image.h"

#include <iomanip>
#include <sstream>


int main(){
	std::ifstream chatgray("chat.pgm", std::ios::binary);
	std::ifstream chat("chat_petit.ppm", std::ios::binary);
	std::ifstream chatd("chat_petit.ppm", std::ios::binary);
	std::ifstream image("chat.ppm", std::ios::binary);
	std::ifstream chattgar("chat.tga", std::ios::binary);

	ColorImage* cat = ColorImage::readPPM(chat);
	ColorImage* gato = ColorImage::readPPM(chatd);
	ColorImage* im = ColorImage::readPPM(image);
	GrayImage* gray = GrayImage::readPGM(chatgray);
	ColorImage* tga = ColorImage::readTGA(chattgar);

	std::ofstream chatsimple("chatsimple.ppm", std::ios::binary);
	std::ofstream chatdouble("chatdouble.ppm", std::ios::binary);
	std::ofstream chatrec("chatrectangle.ppm", std::ios::binary);
	std::ofstream wgray("chatgray.pgm", std::ios::binary);
	std::ofstream chattga("chatchat.tga", std::ios::binary);
	std::ofstream chattgareaded("chatreaded.tga", std::ios::binary);

	Color color(255,0,0);
	Color coloro(0,255,0);
	Color colorp(0,0,255);
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

	picjpegin->writeTGA(chattga,false);
	tga->writeTGA(chattgareaded,false);


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
	delete tga;

	return 0;
}
