#ifndef __DISTORTIONPOLYNOMEIGN_HPP__
#define __DISTORTIONPOLYNOMEIGN_HPP__

#include "transfo2delementaire.hpp"

namespace OrientationMATIS
{
class DistortionPolynomeIGN : public Transfo2DElementaire
{
	public :
		DistortionPolynomeIGN(bool isInterne): Transfo2DElementaire(isInterne), m_xPPS(0), m_yPPS(0), m_cr3(0), m_cr5(0), m_cr7(0), m_error_max(1e-9)
		{;}
		
		DistortionPolynomeIGN(double thexPPS, double theyPPS, double thecr3, double thecr5, double thecr7, bool isInterne): Transfo2DElementaire(isInterne), m_xPPS(thexPPS), m_yPPS(theyPPS), m_cr3(thecr3), m_cr5(thecr5), m_cr7(thecr7), m_error_max(1e-9)
		{;}

		DistortionPolynomeIGN(DistortionPolynomeIGN const &dp): Transfo2DElementaire(dp), m_xPPS(dp.m_xPPS), m_yPPS(dp.m_yPPS), m_cr3(dp.m_cr3), m_cr5(dp.m_cr5), m_cr7(dp.m_cr7), m_error_max(dp.m_error_max)
		{;}

		shared_ptr<Transfo2DElementaire> Clone() const
		{
			return shared_ptr<Transfo2DElementaire> ( new DistortionPolynomeIGN(*this) );
		}

		void ApplyLocalToImage(double &colomn, double &line ) const
		{
			double dr = valcorrection(colomn, line);
			colomn += dr * (colomn-m_xPPS);
			line += dr * (line-m_yPPS);
		}


		void ApplyImageToLocal(double &colomn, double &line ) const;

		double GetcPPS() const { return m_xPPS; }
		double GetlPPS() const { return m_yPPS; }
		double GetR3() const { return m_cr3; }
		double GetR5() const { return m_cr5; }
		double GetR7() const { return m_cr7; }

	protected :

		inline double sqr(double x) const { return x*x; }
		inline double f(double r)const  {double r2=sqr(r);return r*(1+r2*(m_cr3+r2*(m_cr5+m_cr7*r2)));};
		inline double df(double r)const {double r2=sqr(r);return 1+r2*(3*m_cr3+r2*(5*m_cr5+7*m_cr7*r2));};

		inline double valcorrection(double const &colomn, double const &line ) const
		{
			double r2 = sqr(colomn - m_xPPS) + sqr(line - m_yPPS);
			double r4 = sqr(r2);
			double r6 = r2 * r4;
			double dr = m_cr3 * r2 + m_cr5 * r4 + m_cr7 * r6;
			double dr2 = dr * dr;
			if (dr2 > r2)
				dr = 0.;
			return dr;
		}

		double m_xPPS, m_yPPS;
		double m_cr3, m_cr5, m_cr7;
		double m_error_max;
};
} // namespace

#endif //#ifndef __DISTORTIONPOLYNOMEIGN_HPP__
