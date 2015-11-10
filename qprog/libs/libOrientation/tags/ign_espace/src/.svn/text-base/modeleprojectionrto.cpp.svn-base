#include <limits>
#include <cmath>

#include "modeleprojectionrto.hpp"

namespace OrientationMATIS
{
	double Polynome::Valeur(double x, double y, double z) const
	{
		double val = 0.;
		double pZ = 1.;
		std::vector<double>::const_iterator it = mdCoef.begin();
		for(int i=0;i<=mdDegZ;++i)
		{
			double pY = 1.;
			for(int j=0;j<=mdDegY;++j)
			{
				double pX = 1.;
				for(int k=0;k<=mdDegX;++k)
				{
					val += (*it) * pX * pY * pZ;
					++it;
					pX*= x;
				}
				pY*= y;
			}
			pZ *= z;
		}
		return val;
	}

	double RTO::Valeur(double x, double y, double z) const
	{
		double numVal = Num.Valeur(x,y,z);
		double denVal = Den.Valeur(x,y,z);
		if (std::abs(denVal) < std::numeric_limits<double>::epsilon() )
		{
			return std::numeric_limits<double>::infinity();
		}
		return numVal/denVal;
	}

	ModeleProjectionRTO::ModeleProjectionRTO(const ModeleProjectionRTO &mprto) : ModeleProjection(mprto)
	{
		CentreImageX	= mprto.CentreImageX;
		CentreImageY	= mprto.CentreImageY;
		CentreImageZ	= mprto.CentreImageZ;
		CoefImageX	= mprto.CoefImageX;
		CoefImageY	= mprto.CoefImageY;
		CoefImageZ	= mprto.CoefImageZ;
		ImageX		= mprto.ImageX;
		ImageY		= mprto.ImageY;
		MaxImageX	= mprto.MaxImageX;
		MaxImageY	= mprto.MaxImageY;
		MinImageX	= mprto.MinImageX;
		MinImageY	= mprto.MinImageY;

		CentreObjetX	= mprto.CentreObjetX;
		CentreObjetY	= mprto.CentreObjetY;
		CentreObjetZ	= mprto.CentreObjetZ;
		CoefObjetX	= mprto.CoefObjetX;
		CoefObjetY	= mprto.CoefObjetY;
		CoefObjetZ	= mprto.CoefObjetZ;
		ObjetX		= mprto.ObjetX;
		ObjetY		= mprto.ObjetY;
		MaxObjetX	= mprto.MaxObjetX;
		MaxObjetY	= mprto.MaxObjetY;
		MinObjetX	= mprto.MinObjetX;
		MinObjetY	= mprto.MinObjetY;
	}

	shared_ptr<ModeleProjection> ModeleProjectionRTO::Clone() const
	{
		return shared_ptr<ModeleProjectionRTO>( new ModeleProjectionRTO(*this) );
	}

	void ModeleProjectionRTO::LocalToImage(double x, double y, double z, double &c, double &l) const
	{
	 // les coord de depart verifiees
		double xv=x,yv=y;
		if (xv<MinObjetX) xv = MinObjetX;
		else if (xv>MaxObjetX) xv = MaxObjetX;
		if (yv<MinObjetY) yv = MinObjetY;
		else if (yv>MaxObjetY) yv = MaxObjetY;
		double X,Y,Z;
		X = (xv-CentreObjetX)/CoefObjetX;
		Y = (yv-CentreObjetY)/CoefObjetY;
		Z = (z-CentreObjetZ)/CoefObjetZ;
		c = ObjetX.Valeur(X,Y,Z) * CoefImageX + CentreImageX;
		l = ObjetY.Valeur(X,Y,Z) * CoefImageY + CentreImageY;
		m_transfo2D.ApplyLocalToImage(c,l);
	}

	void ModeleProjectionRTO::ImageAndZToLocal(double c, double l, double z, double &x, double &y) const
	{
	 // les coord de depart verfiees
		m_transfo2D.ApplyImageToLocal(c,l);
		double cv=c,lv=l;
		if (cv<MinImageX) cv = MinImageX;
		else if (cv>MaxImageX) cv = MaxImageX;
		if (lv<MinImageY) lv = MinImageY;
		else if (lv>MaxImageY) lv = MaxImageY;
		double X,Y,Z;
		X = (cv-CentreImageX)/CoefImageX;
		Y = (lv-CentreImageY)/CoefImageY;
		Z = (z-CentreImageZ)/CoefImageZ;
		x = ImageX.Valeur(X,Y,Z) * CoefObjetX + CentreObjetX;
		y = ImageY.Valeur(X,Y,Z) * CoefObjetY + CentreObjetY;
	}

	void ModeleProjectionRTO::ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
	{
		z0 = 1;
		z1 = 0;
		ImageAndZToLocal(c, l, z0, x0, y0);
		ImageAndZToLocal(c, l, z1, x1, y1);
	}
};
