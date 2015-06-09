#include "modeleprojection.hpp"

using OrientationMATIS::ModeleProjection;

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();
	for (int i=1;i<argc; ++i)
	{
        std::cout << "Testing " << argv[i] << std::endl;
		shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(argv[i]);
		if (ori == 00)
		{
			std::cerr << "Lecture de " << argv[i] << " impossible : test non valide !!!" << std::endl;
			return -1;
		}
		if (!(ori->InternalCheck()))
		{
			std::cerr << "Test interne de " << argv[i] << " faux : test non valide !!!" << std::endl;
			return -1;
		}
		
        shared_ptr<ModeleProjection> ori_clone = ori->Clone();
		if (ori_clone == 00)
		{
			std::cerr << "Clonage de " << argv[i] << " impossible : test non valide !!!" << std::endl;
			return -1;
		}
        if (ori_clone->GetAuxiliaryData().GetSamples().size() != ori->GetAuxiliaryData().GetSamples().size())
		{
			std::cerr << "Clone de " << argv[i] << " n'a pas le meme nombre d'exemples : test non valide !!!" << std::endl;
			return -1;
		}
	    if (!ori_clone->InternalCheck())
		{
			std::cerr << "Test interne du clone de " << argv[i] << " faux : test non valide !!!" << std::endl;
			return -1;
		}
	}

	return 0;	
}

