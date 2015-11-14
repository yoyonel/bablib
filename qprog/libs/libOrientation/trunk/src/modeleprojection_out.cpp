#include <map>
#include <iostream>
#include <sstream>

#include "noboost.hpp"

#include "modeleprojection.hpp"
#include "modeleprojectionvisitor.hpp"

namespace OrientationMATIS 
{

typedef shared_ptr<ModeleProjectionVisitor> (*PtrFonctVisitor)(const std::string &filename);
		
typedef std::multimap<std::string, PtrFonctVisitor>
		MapExportVisitor;

static MapExportVisitor & GetExporters() {
	static MapExportVisitor exporters;
	return exporters;
}


shared_ptr<ModeleProjectionVisitor> xml_exporter(const std::string &filename);

//Bahman 4 mai 2011
// a la demande de MPD
shared_ptr<ModeleProjectionVisitor> ori_exporter(const std::string &filename);

void InitOut()
{
	GetExporters().clear();
	GetExporters().insert(std::pair<std::string, PtrFonctVisitor>(".xml", xml_exporter));
        GetExporters().insert(std::pair<std::string, PtrFonctVisitor>(".ori", ori_exporter));
}

bool ModeleProjection::WriteFile(const shared_ptr<ModeleProjection> ori,
const std::string &filename)
{
//	std::cout<<"ModeleProjection::WriteFile"<<std::endl;
	std::string ext=extension(filename);
//	std::cout << "extension : "<<ext<<std::endl;

	MapExportVisitor::const_iterator it_ext = GetExporters().lower_bound(ext);
	MapExportVisitor::const_iterator it = it_ext;

	// D'abord ceux qui gèrent à peu près la même extension
	for (it = it_ext; it != GetExporters().end(); ++it)
	{
		shared_ptr<ModeleProjectionVisitor> exporter = (*it).second(filename);
//		std::cout << "ICI" << std::endl;
//		std::cout << exporter << std::endl;
		if (ori->Visit(exporter))
			return true;
	}
	// Puis tous les autres
	for (it = GetExporters().begin(); it != it_ext; ++it)
	{
		shared_ptr<ModeleProjectionVisitor> exporter = (*it).second(filename);
		if (ori->Visit(exporter))
			return true;
	}
	return false;
}
 
}
