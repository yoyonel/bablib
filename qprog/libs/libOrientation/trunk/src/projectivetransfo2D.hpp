#ifndef PROJECTIVETRANSFO2D_HPP_
#define PROJECTIVETRANSFO2D_HPP_

#include "transfo2delementaire.hpp"

namespace OrientationMATIS
{
class ProjectiveTransfo2D: public Transfo2DElementaire
{
	public :

		ProjectiveTransfo2D(bool isInterne) : Transfo2DElementaire(isInterne)
		{
			m_a1[0] = 1. ;	m_a1[1] = 0;	m_a1[2] = 0;
			m_a2[0] = 0 ;	m_a2[1] = 1.;	m_a1[2] = 0;
			m_a3[0] = 0 ;	m_a3[1] = 0;	
		}

		/*
		 * a1[3]
		 * a2[3]
		 * a3[2]*/
		void SetParameters(double *a1, double *a2, double *a3)
		{
			for ( unsigned int i = 0 ; i < 3 ; i++)
			{
				m_a1[i] = a1[i];
				m_a2[i] = a2[i];
				if (i < 2) m_a3[i] = a3[i];
				
			}
			
		}

		ProjectiveTransfo2D(const ProjectiveTransfo2D &h) : Transfo2DElementaire(h)
		{
			for (unsigned int i=0; i<3;++i)
			{
				m_a1[i] = h.m_a1[i];
				m_a2[i] = h.m_a2[i];
				if (i<2) m_a3[i] = h.m_a3[i];
				
			}
		}

		shared_ptr<Transfo2DElementaire> Clone() const
		{
			return shared_ptr<Transfo2DElementaire>(new ProjectiveTransfo2D(*this));
		}

		//bahman 1 oct 2009
		// ok à commité avec un test
//		void ApplyLocalToImage(double &c, double &l) const
		void ApplyImageToLocal(double &c, double &l) const
		{
			double ci=c, li=l;
			double denom = ci * m_a3[0] + li * m_a3[1] + 1;
			c = m_a1[0] * ci + m_a1[1] * li + m_a1[2];
			c /= denom;
			
			l = m_a2[0] * ci + m_a2[1] * li + m_a2[2];
			l /=denom; 
		}

		// Bahman 1 oct 2009
		// ok à commité avec un test
		//void ApplyImageToLocal(double &c, double &l) const
		void ApplyLocalToImage(double &c, double &l) const
		{
			double h = 0 ;
			double ci = c;
			double li = l;
			h  = (m_a2[0] * m_a3[1] - m_a2[1] * m_a3[0]) * ci;
			h += (m_a1[1] * m_a3[0] - m_a1[0] * m_a3[1]) * li;
			h += (m_a1[0] * m_a2[1] - m_a1[1] * m_a2[0]) * 1.;
			
			c  = (m_a2[1] * 1 - m_a2[2] * m_a3[1]) * ci;
			c += (m_a1[2] * m_a3[1] - m_a1[1] * 1) * li;
			c += (m_a1[1] * m_a2[2] - m_a1[2] * m_a2[1]) * 1.;
			c /= h; 
			
			l  = (m_a2[2] * m_a3[0] - m_a2[0] * 1) * ci;
			l += (m_a1[0] * 1 - m_a1[2] * m_a3[0]) * li;
			l += (m_a1[2] * m_a2[0] - m_a1[0] * m_a2[2]) * 1.;
			l /= h;
			
		}

		const double *Geta1() const { return m_a1;}
		const double *Geta2() const { return m_a2;}
		const double *Geta3() const { return m_a3;}
	
	protected :
		double m_a1[3];
		double m_a2[3];
		double m_a3[2];
};
} // namespace
#endif /*PROJECTIVETRANSFO2D_HPP_*/
