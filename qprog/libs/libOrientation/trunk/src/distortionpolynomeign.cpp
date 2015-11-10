#include <cmath>
#include "distortionpolynomeign.hpp"

namespace OrientationMATIS
{
	void DistortionPolynomeIGN::ApplyLocalToImage(double &colomn, double &line ) const
	{
		double r_photo_col = colomn - m_xPPS;
		double r_photo_lin = line - m_yPPS;
		double r2 = sqr(r_photo_col) + sqr(r_photo_lin);
		if (r2 > m_r2_max) return; // zone de validite du polynome
		double dr = epsilon(r2); // = f(r)/r-1
		colomn += dr * r_photo_col;
		line += dr * r_photo_lin;
	}

	void DistortionPolynomeIGN::ApplyImageToLocal(double &colomn, double &line ) const
	{
		/*
		On veut inverser la fonction f(r)=(1+c1)*r+c3*r^3+c5*r^5+c7*r^7
		afin de trouver r a partir de y=f(r) avec l'iteration:
		r(0) = y
		r(n+1) = r(n) + (y-f(r(n))) / f'(r(n))
		*/
		double r_photo_col = colomn - m_xPPS;
		double r_photo_lin = line - m_yPPS;
		double y2=sqr(r_photo_col)+sqr(r_photo_lin);
		// Si y==0 on est sur le PPS donc il n'y a pas de distortion
		if (y2==0.) return;
		double y=sqrt(y2), r = y; // initialisation de l'iteration
		// quantites a maintenir a jour
		double r2=r*r, upe=1.+epsilon(r2), err = (y-r*upe), err2=err*err; // r*upe=f(r)
		int i=0, i_max=50; // max num of iterations
		while(err2 > m_error_max && i<i_max && r2 < m_r2_max)// a 1e-3 pixel
		{
			// nouvelle estimation de r tel que f(r)=y
			double D=upe+2.*r2*depsilon(r2); // = df(r);
			if (D<0.1) r += err;
			else r += err / D;
			r2=r*r; upe=1.+epsilon(r2); err = (y-r*upe), err2=err*err;
			i++;
		}
		if(err2 > m_error_max)
		{
//			std::cout << "ApplyImageToLocal(" << colomn << "," << line << ") ";
//			if(r2 > m_r2_max) std::cout << "diverge";
//			else if (i==i_max) std::cout << "ne converge pas";
//			else std::cout << "fait un truc bizzare";
//			std::cout << " apres " << i << " iterations" << std::endl;
//			std::cout << "y = " << y << ", r = "<<r <<
//				", f(r) = " << r*upe << ", err = " << err << std::endl;
		}
		double ratio = r/y;
		colomn = m_xPPS + ratio*r_photo_col;
		line = m_yPPS + ratio*r_photo_lin;
	}

}
