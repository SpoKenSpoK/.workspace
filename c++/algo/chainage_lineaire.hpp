#ifndef CHAINAGE_LINEAIRE_HPP
#define CHAINAGE_LINEAIRE_HPP

class Maillon{
public:
	int clef;
	Maillon* suivant;
	Maillon(int _clef);
	~Maillon();
};

class Table{
private:
	int taille;
	Maillon** tab;
public:
	Table(int taille);
	~Table();
	void ManageTable();
	void ajouter(int clef);
	void supprimer(int clef);
	void afficher();
};

#endif