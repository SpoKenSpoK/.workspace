#include <iostream>
#include "image.h"

int main(){
	//GrayImage i; //>Déclanche une erreur
	GrayImage i(400,300);
	i.clear(200);

	ColorImage cc(420,610);
	Color color(0,35,120);
	cc.clear(color);

	i.fillRectangle(11,120,120,150, 10);
	i.rectangle(10,10,122,152,210);

	std::ofstream f("test.pgm", std::ios::binary);
	std::ofstream k("bruh.ppm", std::ios::binary);
	std::ofstream h("z.pgm", std::ios::binary);
	std::ofstream g("test_.pgm", std::ios::binary);
	std::ofstream imgcolor("color.ppm", std::ios::binary);

	GrayImage* z = i.simpleScale(420,310);
	GrayImage* w = i.bilinearScale(560,210);

	i.writePGM(f);
	z->writePGM(h);

	cc.writePPM(imgcolor);

	std::ifstream is("storm.pgm", std::ios::binary);
	std::ifstream es("chat.ppm", std::ios::binary);
	GrayImage* pi = GrayImage::readPGM(is);

	ColorImage* pe = ColorImage::readPPM(es);

	pi->writePGM(g);
	pe->writePPM(k);

	delete pi;
	delete z;
	delete w;
	delete pe;

	f.close();
	g.close();
	is.close();
	k.close();
	h.close();
	imgcolor.close();

	return 0;
}
