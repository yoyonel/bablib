#include <iostream>
#include <sstream>

#include "modeleprojection.hpp"
#include "noboost.hpp"
#include "string.h"
#include <fstream>
#include "tinyxml.h"
#include "outil_xml.hpp"

using OrientationMATIS::ModeleProjection;

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << "Can not find test file name in command line! "
				<< std::endl;
		return -1;
	}

#if ORIENTATIONMATIS_USE_NOBOOST
	std::string rep = path(argv[1]);
	std::string ficnom = basename(argv[1]);
	std::string ext = extension(argv[1]);
#else
	std::string rep = boost::filesystem::path(argv[1]).branch_path().string();
	std::string ficnom = boost::filesystem::basename(argv[1]);
	std::string ext = boost::filesystem::extension(argv[1]);
#endif

	std::string oriname = rep + "/" + ficnom + ".ori";
	std::string nomspl = rep + "/" + ficnom + ".spl";
	std::string orixmlname = rep + "/" + ficnom + "_written.xml";
	std::string neworixmlname = rep + "/" + ficnom + "_written_bis.xml";

	ModeleProjection::InitAllIO();
	shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(oriname);

	TiXmlDocument doc(nomspl.c_str());
	if (!doc.LoadFile()) {
		throw std::logic_error("Can not find " + nomspl + " file!");
		//			return shared_ptr<ModeleProjection>();
	}
	TiXmlHandle h_Doc(&doc);
	std::vector<OrientationMATIS::Sample> mysamples;
	OrientationMATIS::XmlHandler::ReadSamples(&doc, mysamples);
	//	std::cout<<"lecture de mysamples fini ! \n";
	ori->GetAuxiliaryData().AddSamples(mysamples);
	//	std::cout<<"mysamples added to ori ! \n";

	if (!ori->InternalCheck(0.0001)) {
		std::cerr << "InternalCheck failed for " << oriname << " and "
				<< nomspl << std::endl;
		return -1;
	}
	//	std::cout<<" internal check fini ! \n";
	ModeleProjection::WriteFile(ori, orixmlname);

	shared_ptr<ModeleProjection> ori2 = ModeleProjection::ReadFile(orixmlname);
	//	std::cout<< "number of samples after reading : "<<ori2->GetAuxiliaryData().GetSamples().size()<<std::endl;
	if (mysamples.size() != ori2->GetAuxiliaryData().GetSamples().size()) {
		std::cerr << "Nb Samples changed from " << nomspl << " to "
				<< orixmlname << std::endl;
		return -1;
	}
	if (!ori2->InternalCheck(0.0001)) {
		std::cerr << "InternalCheck failed for " << orixmlname << std::endl;
		return -1;
	}
	ModeleProjection::WriteFile(ori2, neworixmlname);

	return 0;
}

