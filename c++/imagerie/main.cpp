#include "GrayImage.hpp"

int main(){
	//GrayImage i; //>Déclanche une erreur
	GrayImage i(400,300);
	i.clear();
	
	i.fillRectangle(11,11,120,150, 255);
	i.rectangle(10,10,122,152,150);

	std::ofstream f("test.pgm", std::ios::out | std::ios::binary);
	std::ofstream g("test_.pgm", std::ios::out | std::ios::binary);

	i.writePGM(f);

	std::ifstream is("test.pgm", std::ios::in | std::ios::binary);
	GrayImage* pi = GrayImage::readPGM(is);

	pi->writePGM(g)

	delete pi;					
       				
	f.close();
	g.close();
	is.close();

	return 0;
}
