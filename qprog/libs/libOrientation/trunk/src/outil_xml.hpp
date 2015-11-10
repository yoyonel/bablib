#ifndef __OUTIL_XML__
#define __OUTIL_XML__

#include "tinyxml.h"
#include "auxiliarydata.hpp"
#include "homographie2d.hpp"
#include "transfo2d.hpp"
#include "projectivetransfo2D.hpp"



namespace OrientationMATIS
{
	
	class SystematismeCylindriqueTopAero;
	class SystematismeAffineTopAero;
namespace XmlHandler
{
	//Deux petite structures utiles pour charger des pts et des matrices (impact limit� dans la mesure o� elles font partie de ce namespace)
	typedef struct
	{
		double X,Y,Z;
	}_t_XPt3D;

	typedef struct
	{
		_t_XPt3D l1,l2,l3;
	}_t_XMat3D;

	extern TiXmlElement*  AssertRoot(TiXmlDocument * doc, const std::string &rootname);
	bool ReadDate(const TiXmlNode* node, Date &date) ;
	bool ReadStereopolis(const TiXmlNode* node, Stereopolis & st );

	bool ReadXPt3D(const TiXmlNode* node,_t_XPt3D & pt);
	bool ReadPt2D(const TiXmlNode* h, double &x, double &y);
	bool ReadMat3D(const TiXmlNode* node,_t_XMat3D & mat);
	bool ReadStereopolis(const TiXmlNode* node, Stereopolis & st );

	void ReadSamples(TiXmlNode* node, std::vector <Sample> & mysamples) ;

	shared_ptr<Homographie2D> ReadHomographie2D(TiXmlHandle * h_orientation);
	bool ReadTransfo2D(const TiXmlHandle* h, Transfo2D & tr2D);
	
	bool WriteDate(TiXmlElement * elem, const Date &date );
	bool WritePt3d(TiXmlElement * elem, double x, double y, double z );
	bool WritePt2d(TiXmlElement * elem, double x, double y);
	bool WriteMat3D(TiXmlElement * elem,const double *l1, const double *l2 , const double *l3);
	void WriteSamples(TiXmlElement* h, std::vector <Sample> const & mysamples);
	void WriteHomographie2D(TiXmlElement * e_orientation, shared_ptr<Homographie2D> h2d);
	void WriteProjectiveTransfo2D(TiXmlElement * e_orientation, shared_ptr<ProjectiveTransfo2D> h2d);
	bool WriteAuxiliaryData(TiXmlElement * node, const AuxiliaryData & aux);
	void WriteSystematismeAffineTopAero(TiXmlElement * elem,shared_ptr<SystematismeAffineTopAero> const &sys);
	void WriteSystematismeCylindriqueTopAero(TiXmlElement * elem,shared_ptr<SystematismeCylindriqueTopAero> const &sys);
}
} // namespace

#endif
