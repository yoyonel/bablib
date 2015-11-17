#include <iostream>

#include "modeleprojection.hpp"

using OrientationMATIS::ModeleProjection;

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();

	if (argc!=3)
	{
		std::cout << "con2ori nom.con nom.ori"<<std::endl;
		std::cout << "traduction des nouveau ori vers l'ancien format"<<std::endl;
		return 1;
	}
	std::string nomCon(argv[1]);
	std::string nomOri(argv[2]);
	std::cout << "Lecture de : " << nomCon << std::endl;
	shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(nomCon);
	std::cout << "fin de Lecture de : " << nomCon << std::endl;
	if (ori == 00)
	{
		std::cerr << "Lecture de " << nomCon << " impossible : test non valide !!!" << std::endl;
		return -1;
	}
	std::cout << "Ecriture de : " << nomOri << std::endl;
	if (!ModeleProjection::WriteFile(ori, nomOri))
	{
		std::cerr << "Ecriture de " << nomOri << " impossible : test non valide !!!" << std::endl;
		return -1;
	}
	
	return 0;
}
