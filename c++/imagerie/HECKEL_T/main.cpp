#include <iostream>
#include "Image.h"

int main(){
	std::ifstream chat("chat_petit.ppm", std::ios::binary);
	std::ifstream chatd("chat_petit.ppm", std::ios::binary);

	ColorImage* cat = ColorImage::readPPM(chat);
	ColorImage* gato = ColorImage::readPPM(chatd);

	std::ofstream chatsimple("chatsimple.ppm", std::ios::binary);
	std::ofstream chatdouble("chatdouble.ppm", std::ios::binary);

	cat = cat->bilinearScale(823, 400);
	cat->writePPM(chatdouble);

	gato = gato->simpleScale(823, 400);
	gato->writePPM(chatsimple);

	chatsimple.close();
	chatdouble.close();
	chat.close();

	return 0;
}
