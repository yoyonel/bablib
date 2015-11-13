#include <iostream>

#include "modeleprojection.hpp"

using OrientationMATIS::ModeleProjection;

void changenames(const std::string &infile, std::string &outfile1, std::string &outfile2)
{
	outfile1 = infile;
	outfile2 = infile;
	std::string::size_type pos = infile.find_last_of('.');
	std::string replacement1("_written.xml");
	outfile1.replace(pos, replacement1.size(), replacement1);
	std::string replacement2("_written_bis.xml");
	outfile2.replace(pos, replacement2.size(), replacement2);
}

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();
	for (int i=1;i<argc; ++i)
	{
		std::string infile(argv[i]), outfile1, outfile2;
		changenames(infile, outfile1, outfile2);
		std::cout << "Lecture de : " << infile << std::endl;
		shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(infile);
		if (ori == 00)
		{
			std::cerr << "Lecture de " << infile << " impossible : test non valide !!!" << std::endl;
			return -1;
		}
		std::cout << "Ecriture de : " << outfile1 << std::endl;
		if (!ModeleProjection::WriteFile(ori, outfile1))
		{
			std::cerr << "Ecriture de " << outfile1 << " impossible : test non valide !!!" << std::endl;
			return -1;
		}

		std::cout << "Relecture de : " << outfile1 << std::endl;
		ori = ModeleProjection::ReadFile(outfile1);
		if (ori == 00)
		{
			std::cerr << "Lecture de " << outfile1 << " impossible : test non valide !!!" << std::endl;
			return -1;
		}
		std::cout << "Re-ecriture de : " << outfile2 << std::endl;
		if (!ModeleProjection::WriteFile(ori, outfile2))
		{
			std::cerr << "Ecriture de " << outfile2 << " impossible : test non valide !!!" << std::endl;
			return -1;
		}
	}
	return 0;	
}
