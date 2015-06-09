#include <cmath>
#include "distortionpolynomeign.hpp"

namespace OrientationMATIS
{

	void DistortionPolynomeIGN::ApplyImageToLocal(double &colomn, double &line ) const
	{
	  /*
		On veut inverser la fonction f(r)=r+c3*r^3+c5*r^5+c7*r^7
		afin de trouver r a partir de y=f(r)
		pour ca on va estimer x avec:
		r(0) = y
		r(n+1) = r(n) + (y-f(r(n))) / f'(r(n))
	  */

		double col_groun,lin_groun,col_photo_estimee,lin_photo_estimee;
		double r_photo_col = colomn - m_xPPS;
		double r_photo_lin = line - m_yPPS;
		double err2;
		double y=sqrt(sqr(r_photo_col)+sqr(r_photo_lin));
	  // Si y==0 on est sur le PPS donc il n'y a pas de distortion
		if (y==0.)
			return;
		double vcol = r_photo_col / y;
		double vlin = r_photo_lin / y;
		double r = y;
		do
		{
			double r_pred = r;
			double diff = (y-f(r_pred));
		  // nouvelle estimation de r tel que f(r)=y
			double D=df(r_pred);
			if (D<0.1)
			{
				r = r_pred + diff;
			}
			else
				r = r_pred + diff / D;
		  // on en deduit une estimation des coord Local
			col_groun = m_xPPS + r*vcol;
			lin_groun = m_yPPS + r*vlin;
		  // on verifie l'ecart (err2: distance en pixel au carre) entre les coord photo initiales
		  // et celles donnes en passant le point Local estime en photo
			col_photo_estimee = col_groun;
			lin_photo_estimee = lin_groun;
			ApplyLocalToImage(col_photo_estimee,lin_photo_estimee);
			err2 = sqr(col_photo_estimee - colomn) + sqr(lin_photo_estimee - line);
		  //SORTIEMESSAGE("err2 = "<<err2<<std::endl);
		}
		while(err2 > m_error_max);// a 1e-3 pixel
		colomn = col_groun;
		line = lin_groun;
	  /*
		double c, l;
		double cf, lf;
		double resc, resl;

		double dr = valcorrection(colomn, line);
		SORTIEMESSAGE("dr = "<<dr<<std::endl);
		c = colomn - dr * (colomn-xPPS);
		l = line - dr * (line-yPPS);

		do
		{
		cf = c;
		lf = l;
		LocalToPhoto(cf,lf);

		resc = colomn - cf;
		resl = line - lf;
		c += resc;
		l += resl;
		SORTIEMESSAGE("c="<<c<<" l="<<l<<" resc="<<resc<<" resl="<<resl<<std::endl);
	}
		while ((ABS(resc) > 1e-3) || (ABS(resl) > 1e-3));

		colomn = c;
		line = l;
	  */
	}

};
