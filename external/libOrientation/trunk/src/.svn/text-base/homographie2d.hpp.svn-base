#ifndef __HOMOGRAPHIE2D_HPP__
#define __HOMOGRAPHIE2D_HPP__

#include "transfo2delementaire.hpp"

namespace OrientationMATIS
{
class Homographie2D : public Transfo2DElementaire
{
	public :

		Homographie2D(bool isInterne) : Transfo2DElementaire(isInterne)
		{
			m_c[0] = 1.; m_c[1] = 0.; m_c[2] = 0.;
			m_l[0] = 0.; m_l[1] = 1.; m_l[2] = 0.;
		}

		void SetC(double c, double l, double t)
		{
			m_c[0] = c; m_c[1] = l; m_c[2] = t;
		}

		void SetL(double c, double l, double t)
		{
			m_l[0] = c; m_l[1] = l; m_l[2] = t;
		}

		Homographie2D(const Homographie2D &h) : Transfo2DElementaire(h)
		{
			for (unsigned int i=0; i<3;++i)
			{
				m_c[i] = h.m_c[i];
				m_l[i] = h.m_l[i];
			}
		}

		shared_ptr<Transfo2DElementaire> Clone() const
		{
			return shared_ptr<Transfo2DElementaire>(new Homographie2D(*this));
		}

		void ApplyLocalToImage(double &c, double &l) const
		{
			double ci=c, li=l; 
			c = m_c[0] * ci + m_c[1] * li + m_c[2];
			l = m_l[0] * ci + m_l[1] * li + m_l[2];
		}

		void ApplyImageToLocal(double &c, double &l) const
		{
			double ci = c - m_c[2], li = l - m_l[2];
			double det = m_c[0] * m_l[1] - m_c[1] * m_l[0];
			if (det < 1e-9)
				return;
			det = 1. / det;
			c = det * ( m_l[1] * ci - m_c[1] * li);
			l = det * (-m_l[0] * ci + m_c[0] * li);
		}

		const double *GetColomn() const { return m_c;}
		const double *GetLine() const { return m_l;}
	protected :
		double m_c[3];
		double m_l[3];
};
} // namespace

#endif //#ifndef __HOMOGRAPHIE2D_HPP__
