#include "modeleprojection.hpp"
#include "geometrictransformation.hpp"

#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"
#include <string>

using OrientationMATIS::ModeleProjection;

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();
	if (argc < 2) return 1;
	
		std::string rep = boost::filesystem::path(argv[1]).branch_path().string();
		std::string ficnom = boost::filesystem::basename(argv[1]);
		std::string ext = boost::filesystem::extension(argv[1]);

		std::string oriname = rep + "/" + ficnom + "_crope.xml";	
		std::string oriname2 = rep + "/" + ficnom + "_crope_rewritten.xml";
	

	boost::shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(argv[1]);
	std::cout<< "avant number of transfo : "<<ori->GetTransfo2D().GetNbTransfo2DElementaire()<<std::endl;
	Crop(ori, 1,2,3,4,100);
	Crop(ori, 5,6,7,8,.5);
	Crop(ori, 5,6,7,8,.5);
	ModeleProjection::WriteFile(ori,oriname);
	boost::shared_ptr<ModeleProjection> ori2 = ModeleProjection::ReadFile(oriname);
	ModeleProjection::WriteFile(ori2,oriname2);
	
	
	std::cout<< "Apres number of transfo : "<<ori->GetTransfo2D().GetNbTransfo2DElementaire()<<std::endl;
	
	
}
