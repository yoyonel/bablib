#include "modeleprojection.hpp"

using OrientationMATIS::ModeleProjection;

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();
	for (int i=1;i<argc; ++i)
	{
        std::cout << "Testing " << argv[i] << std::endl;
        std::cerr << "Fichier en entrée de ModeleProjection::ReadFile : " << argv[i] << std::endl;
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
		if (ori)
		{
			/* Creation de Point de Test */
			// Image vers Terrain
			unsigned int nc,nl;
			unsigned int pasSample = 1000;
			double zl=0.,xl,yl;
			double x,y,z;
			ori->GetTailleImage(nc,nl);
			for(unsigned int l=0;l<nl;l+=pasSample)
			{
				for(unsigned int c=0;c<nc;c+=pasSample)
				{
					ori->ImageAndZToLocal(c,l,zl,xl,yl);
					ori->LocalToWorld(xl,yl,zl, ori->GetSystemGeodesie().c_str(), x,y,z);

					z=0.;
					ori->WorldToLocal(ori->GetSystemGeodesie().c_str(),x,y,z, xl,yl,zl);
					
					ori->ImageAndZToLocal(c,l,zl,xl,yl);
					ori->LocalToWorld(xl,yl,zl, ori->GetSystemGeodesie().c_str(), x,y,z);
					std::cout << std::fixed << "Image ( "<<c<<" , "<<l<<" ) and Z ( "<<z<<" ) -> ( "<<x<<" , "<<y<<" ) "<<std::endl;
					
				}
			}
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

