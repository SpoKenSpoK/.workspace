#include "chainageline.hpp"
#include <iostream>

Maillon::Maillon(int _clef)
	:clef(_clef), suivant(NULL)
{}

Maillon::~Maillon(){}

Table::Table(int _taille)
	:taille(_taille), tab(NULL)
{
	tab = new Maillon*[taille];
	for(int i=0; i<taille; ++i)
		tab[i] = NULL;
}

Table::~Table(){
	for(int i=0; i<taille; ++i){
		if(tab[i] != NULL){
			Maillon* temp;
			Maillon* temp2;
			temp = tab[i];
			while(temp->suivant != NULL){
				temp2 = temp->suivant;
				delete temp;
				temp = temp2;
			}
			delete temp;
		}
	}
	delete[] tab;
}

void Table::ManageTable(){
	int choix;
	bool quitter = false;
	while(!quitter){
		std::cout<<"Que souhaitez-vous faire avec la table ?\n(0)ajouter (1)supprimer (2)afficher (3)quitter\n";
		std::cin>>choix;
		switch(choix){
			case 0:
				int tempi;
				std::cout<<"quelle clef voulez-vous ajouter ?\n";
				std::cin>>tempi;
				ajouter(tempi);
				break;
			case 1:
				int tempi2;
				std::cout<<"quelle clef voulez-vous supprimer ?\n";
				std::cin>>tempi2;
				supprimer(tempi2);
				break;
			case 2:
				afficher();
				break;
			case 3:
				quitter = true;
				break;
			default:
				std::cout<<"invalid entry\n";
				break;
		}

	}
}

void Table::ajouter(int _clef){
	int clef_hashe = _clef%taille;
	if(tab[clef_hashe] == NULL){ // si cette alveole est vide
		tab[clef_hashe] = new Maillon(_clef);
	}
	else{
		Maillon* temp = tab[clef_hashe];
		while(temp->suivant != NULL){
			temp = temp->suivant;
		}
		temp->suivant = new Maillon(_clef);
	}
}

void Table::supprimer(int _clef){
	int clef_hashe = _clef%taille;
	bool trouve = false;
	if(tab[clef_hashe] != NULL){//si il y a qqchose dedans
		if(tab[clef_hashe]->suivant == NULL){ // si il n'y a qu'un element dans cette alveole
			if(tab[clef_hashe]->clef == _clef){
				delete tab[clef_hashe];
				tab[clef_hashe] = NULL;
				trouve = true;
			}
		}
		else{
			if(tab[clef_hashe]->clef == _clef){ //si le premier element est bon
				Maillon* temp;
				temp = tab[clef_hashe]->suivant;
				delete tab[clef_hashe];
				tab[clef_hashe] = temp;
				trouve = true;
			}
			else{
				Maillon* temp_boucle = tab[clef_hashe];
				while(temp_boucle->suivant != NULL && !trouve){
					if(temp_boucle->suivant->clef == _clef){
						if(temp_boucle->suivant->suivant != NULL){ //si ce n'est pas le dernier
							Maillon* temp;
							temp = temp_boucle->suivant->suivant;
							delete temp_boucle->suivant;
							temp_boucle->suivant = temp;
							trouve = true;
						}
						else{
							delete temp_boucle->suivant;
							temp_boucle->suivant = NULL;
							trouve = true;
						}
					}
					else temp_boucle = temp_boucle->suivant;

				}
			}
		}
		if(!trouve) std::cout<<"non trouvé\n";

	}
	else
		std::cout<<"non trouvé\n";
}

void Table::afficher(){
	for(int i=0; i<taille; ++i){
		if(tab[i] == NULL){
			std::cout<<"[]\n";
		}
		else{
			Maillon* temp;
			temp = tab[i];
			while(temp->suivant != NULL){
				std::cout<<"["<<temp->clef<<"]";
				temp = temp->suivant;
			}
			std::cout<<"["<<temp->clef<<"]\n";
		}
	}
	std::cout<<"\n";
}
