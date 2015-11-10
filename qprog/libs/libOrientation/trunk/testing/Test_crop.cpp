#include "modeleprojection.hpp"
#include "geometrictransformation.hpp"

#include <string>
#include "noboost.hpp"

using OrientationMATIS::ModeleProjection;

int main(int argc, char **argv) {
	ModeleProjection::InitAllIO();
	if (argc < 2)
		return 1;

#if ORIENTATIONMATIS_USE_NOBOOST
	std::string rep = path(argv[1]);
	std::string ficnom = basename(argv[1]);
	std::string ext = extension(argv[1]);
#else
	std::string rep = boost::filesystem::path(argv[1]).branch_path().string();
	std::string ficnom = boost::filesystem::basename(argv[1]);
	std::string ext = boost::filesystem::extension(argv[1]);
#endif

	std::string oriname = rep + "/" + ficnom + "_crope.xml";
	std::string oriname2 = rep + "/" + ficnom + "_crope_rewritten.xml";

	shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(argv[1]);
	std::cout << "avant number of transfo : "
			<< ori->GetTransfo2D().GetNbTransfo2DElementaire() << std::endl;
	Crop(ori, 1, 2, 3, 4, 100);
	Crop(ori, 5, 6, 7, 8, .5);
	Crop(ori, 5, 6, 7, 8, .5);
	ModeleProjection::WriteFile(ori, oriname);
	shared_ptr<ModeleProjection> ori2 = ModeleProjection::ReadFile(oriname);
	ModeleProjection::WriteFile(ori2, oriname2);

	std::cout << "Apres number of transfo : "
			<< ori->GetTransfo2D().GetNbTransfo2DElementaire() << std::endl;

}
