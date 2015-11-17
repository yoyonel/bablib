#ifndef __MODELEPROJECTIONRTO_HPP__
#define __MODELEPROJECTIONRTO_HPP__

#include "modeleprojection.hpp"
#include "modeleprojectionvisitor.hpp"

namespace OrientationMATIS
{
class Polynome
{
	public:
		int mdDegX;
		int mdDegY;
		int mdDegZ;
  // tableau des coef du polynome mdCoef[z*(mdDegX*mdDegY)+y*(mdDegX)+x]
		std::vector<double> mdCoef;

		Polynome():mdDegX(0),mdDegY(0),mdDegZ(0){}
		Polynome(const Polynome &poly)
		{
			*this = poly;
		}

		Polynome & operator=(const Polynome &poly)
		{
			mdDegX = poly.mdDegX;
			mdDegY = poly.mdDegY;
			mdDegZ = poly.mdDegZ;
			mdCoef = poly.mdCoef;
			return *this;
		}

		double Valeur(double x, double y, double z) const;
};

class RTO
{
	public:
		Polynome Num;
		Polynome Den;

		RTO()
		{;}

		RTO(const RTO &rto)
		{
			*this = rto;
		}

		RTO &operator=(const RTO &rto)
		{
			Num = rto.Num;
			Den = rto.Den;
			return *this;
		}

		double Valeur(double x, double y, double z) const;

};


class ModeleProjectionRTO : public ModeleProjection
{
	public :
		double CentreImageX;
		double CentreImageY;
		double CentreImageZ;
		double CoefImageX;
		double CoefImageY;
		double CoefImageZ;
		RTO ImageX;
		RTO ImageY;
		double MaxImageX;
		double MaxImageY;
		double MinImageX;
		double MinImageY;


		double CentreObjetX;
		double CentreObjetY;
		double CentreObjetZ;
		double CoefObjetX;
		double CoefObjetY;
		double CoefObjetZ;
		RTO ObjetX;
		RTO ObjetY;
		double MaxObjetX;
		double MaxObjetY;
		double MinObjetX;
		double MinObjetY;

	public :
		ModeleProjectionRTO(unsigned int nc, unsigned int nl) : ModeleProjection(nc, nl)
		{;}

		ModeleProjectionRTO(const ModeleProjectionRTO &mprto);
		bool Visit(shared_ptr<ModeleProjectionVisitor> &exporter) const { return exporter->Visit(this); };
		shared_ptr<ModeleProjection> Clone() const;

		void LocalToImage(double x, double y, double z, double &c, double &l) const;
		void ImageAndZToLocal(double c, double l, double z, double &x, double &y) const;
		void ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;
};
} // namespace

#endif
