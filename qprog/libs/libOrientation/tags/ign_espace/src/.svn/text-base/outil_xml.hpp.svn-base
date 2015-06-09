#ifndef __OUTIL_XML__
#define __OUTIL_XML__

#include "tinyxml.h"
#include "auxiliarydata.hpp"
#include "homographie2d.hpp"
#include "transfo2d.hpp"
#include "projectivetransfo2D.hpp"

namespace OrientationMATIS
{
namespace XmlHandler
{
	bool ReadDate(const TiXmlHandle* h, Date &date );
	bool ReadPt3D(const TiXmlHandle* h, double &x, double &y, double &z);
	bool ReadMat3D(const TiXmlHandle* h, double *l1, double *l2 , double *l3);
	void ReadSamples(const TiXmlHandle* h, std::vector <Sample> & mysamples);
	shared_ptr<Homographie2D> ReadHomographie2D(TiXmlHandle * h_orientation);
	void ReadTransfo2D(const TiXmlHandle* h, Transfo2D & tr2D);
	
	bool WriteDate(TiXmlElement * elem, const Date &date );
	bool WritePt3d(TiXmlElement * elem, double x, double y, double z );
	bool WriteMat3D(TiXmlElement * elem,const double *l1, const double *l2 , const double *l3);
	void WriteSamples(TiXmlElement* h, std::vector <Sample> const & mysamples);
	void WriteHomographie2D(TiXmlElement * e_orientation, shared_ptr<Homographie2D> h2d);
	void WriteProjectiveTransfo2D(TiXmlElement * e_orientation, shared_ptr<ProjectiveTransfo2D> h2d);
};
} // namespace

#endif
