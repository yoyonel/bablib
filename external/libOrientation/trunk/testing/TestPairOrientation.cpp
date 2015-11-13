#include <iostream>
#include <sstream>

#include "modeleprojection.hpp"
#include "noboost.hpp"
#include "string.h"
#include <fstream>
#include "tinyxml.h"
#include "outil_xml.hpp"
#include "ConicStereoPair.hpp"

using OrientationMATIS::ModeleProjection;
using namespace OrientationMATIS;
int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Can not find test file name in command line! "
				<< std::endl;
		return -1;
	}
	std::ifstream fic(argv[1]);
	double tol = 0.0001;
	
#if ORIENTATIONMATIS_USE_NOBOOST
        std::string rep = path(argv[1]);
        std::string ficnom = basename(argv[1]);
        std::string ext = extension(argv[1]);
#else
        std::string rep = boost::filesystem::path(argv[1]).branch_path().string();
        std::string ficnom = boost::filesystem::basename(argv[1]);
        std::string ext = boost::filesystem::extension(argv[1]);
#endif

        std::string fileorig_out = rep + "/" + "Pair_Left.ori";
        std::string fileorid_out = rep + "/" + "Pair_Right.ori";

	// Pairorientation
	ConicStereoPair pair(argv[2], argv[3]);
	
	ConicStereoPair::OrientationPointerType orig = pair.GetLeftOrientation();
	ConicStereoPair::OrientationPointerType orid = pair.GetRightOrientation();
	
        ModeleProjection::WriteFile(orig,fileorig_out);
        ModeleProjection::WriteFile(orid,fileorid_out);

	fic.precision(12);
	std::cout.precision(12);
	while (!fic.eof())
	{
		double X,Y,Z;
		fic>>X;
//		std::cout<<X<<" ";;
		fic>>Y;
//		std::cout<<Y<<" ";
		fic>>Z;
//		std::cout<<Z<<"\n";
		double cg,lg,cd,ld;
		double XP,YP,ZP;
//		std::cout<<"avat local to image \n";
		orig->LocalToImage(X,Y,Z,cg,lg);
//		std::cout<<"(cg,lg) : "<<cg<<","<<lg<<std::endl;
			
		orid->LocalToImage(X,Y,Z,cd,ld);
//		std::cout<<"(cd,ld) : "<<cd<<","<<ld<<std::endl;
		
//		std::cout<<"apres local to image \n";
		double res = pair.ImageToLocal(cg,lg,cd,ld,XP,YP,ZP);
//		std::cout<<"apres ImageToLocal : res = "<<res <<"\n";
		double dx = std::abs(X-XP);
		double dy = std::abs(Y-YP);
		double dz = std::abs(Z-ZP);
//		std::cout<<"dx : "<<dx<<" dy : "<<dy<<" dz : "<<dz<<std::endl;
		if ( dx > tol || dy > tol || dz > tol ) return -1;				
	}
return 0;
}
