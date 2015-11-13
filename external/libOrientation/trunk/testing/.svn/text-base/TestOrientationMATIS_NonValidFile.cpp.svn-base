#include <iostream>

#include "modeleprojection.hpp"

using OrientationMATIS::ModeleProjection;


int main (int argc, char **argv)
{
        ModeleProjection::InitAllIO();
        for (int i=1;i<argc; ++i)
        {
                std::string infile(argv[i]), outfile1, outfile2;
//		changenames(infile, outfile1, outfile2);
                std::cout << "Lecture de : " << infile << std::endl;
                shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(infile);
                std::cout << "fin de Lecture de : " << infile << std::endl;
                if (ori != 00)
                {
                    std::cerr << "LibOrientation must return NULL after reading the file: " << infile << " !!!\n" << std::endl;
                    return -1;
                }
        }
        return 0;
}

