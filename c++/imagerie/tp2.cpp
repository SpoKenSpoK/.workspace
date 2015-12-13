#include <iostream>
#include "image.h"

int main(){
	std::ifstream chat("chat_petit.ppm", std::ios::binary);
	std::ifstream chatd("chat_petit.ppm", std::ios::binary);

	ColorImage* cat = ColorImage::readPPM(chat);
	ColorImage* gato = ColorImage::readPPM(chatd);

	std::ofstream chatsimple("chatsimple.ppm", std::ios::binary);
	std::ofstream chatdouble("chatdouble.ppm", std::ios::binary);

	Color color(250,0,0);
	Color coloro(0,250,0);
	Color colorp(0,0,250);
	cat->rectangle(2,2,75,55,color);
	cat->rectangle(4,4,71,51,coloro);
	cat->rectangle(6,6,67,47,colorp);

	cat = cat->bilinearScale(823, 400);
	cat->writePPM(chatdouble);

	gato = gato->simpleScale(823, 400);
	gato->writePPM(chatsimple);

	chatsimple.close();
	chatdouble.close();
	chat.close();
	chatd.close();

    delete cat;
    delete gato;

	return 0;
}
