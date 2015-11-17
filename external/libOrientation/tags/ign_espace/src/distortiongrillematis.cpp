#include <cmath>
#include <cstring>

#include "distortiongrillematis.hpp"

namespace OrientationMATIS
{

	Grille2D::Grille2D(long inNs, long inNl, long inStep):
		ns(inNs),
		nl(inNl),
		pas(inStep)
	{
		mDx = new double[ns*nl];
		mDy = new double[ns*nl];
	}

	Grille2D::Grille2D(const Grille2D &inGrid)
	{
		ns = inGrid.ns ;
		nl = inGrid.nl ;
		pas = inGrid.pas ;
		mDx = new double[ns*nl] ;
		mDy = new double[ns*nl] ;

		::memcpy( mDx, inGrid.mDx, ns*nl*sizeof(double) ) ;
		::memcpy( mDy, inGrid.mDy, ns*nl*sizeof(double) ) ;
	}

	Grille2D::~Grille2D()
	{
		if (mDx != 00)
		{
			delete mDx;
			mDx = 00;
		}

		if (mDy != 00)
		{
			delete mDy;
			mDy = 00;
		}
	}

	Grille2D *Grille2D::Invert() const
	{
		Grille2D *theGrid = new Grille2D( ns, nl, pas ) ;
		double *theDx = theGrid->mDx ;
		double *theDy = theGrid->mDy ;

		long is, il ;
			// Initialisation a l'opoose
		for ( is = 0 ; is < ns*nl ; is++ )
		{
			theDx[is] = -mDx[is] ;
			theDy[is] = -mDy[is] ;
		}

			// Convergence
		long ig = 0 ;
		double col, lig ;
		double deltaX, deltaY;
		for ( il = 0 ; il < pas*nl ; il += pas )
		{
			for ( is = 0 ; is < pas*ns ; is += pas )
			{
				deltaX = 0.0 ;
				deltaY = 0.0 ;
				do
				{
					theDx[ig] += deltaX ;
					theDy[ig] += deltaY ;
					col = (double)is + theDx[ig] ;
					lig = (double)il + theDy[ig] ;
					Apply( col, lig ) ;
					deltaX = (double)is - col ;
					deltaY = (double)il - lig ;
				}
				while ( (fabs( deltaX ) > 0.001) && (fabs( deltaY ) > 0.001) ) ;
				++ig;
			}
		}
			// Valeur retour
		return theGrid ;
	}

	void Grille2D::Apply(double &colomn, double &line) const
	{
		double c,l;
		if (colomn < 0)
			c = 0;
		else if (colomn >= (ns-1)*pas)
			c = (ns-1.000000001)*pas;
		else
			c = colomn;

		if (line < 0)
			l = 0;
		else if (line >= (nl-1)*pas)
			l = (nl-1.000000001)*pas;
		else
			l= line;

		double xx = c / (double) pas ;
		double yy = l / (double) pas ;
		int i0 = (int) xx;
		int j0 = (int) yy;

		int p0 = j0*ns + i0 ;
		int p1 = p0 + 1;
		int p2 = p0 + ns;
		int p3 = p2 + 1;

		double fx = (double)(i0+1) - xx ;
		double fy = (double)(j0+1) - yy ;
		double delta;
		delta =	fy * (fx * mDx[p0] + (1.0-fx) * mDx[p1]) +
				(1-fy) * (fx * mDx[p2] + (1.0-fx) * mDx[p3]) ;
		colomn += delta ;
		delta = fy * (fx * mDy[p0] + (1.0-fx) * mDy[p1]) +
				(1-fy) * (fx * mDy[p2] + (1.0-fx) * mDy[p3]) ;
		line += delta ;
	}

	DistortionGrilleMATIS::DistortionGrilleMATIS(Grille2D *grillel2i,Grille2D *grillei2l, bool isInterne) : Transfo2DElementaire(isInterne), m_grillel2i(grillel2i), m_grillei2l(grillei2l)
	{
		if ((m_grillel2i == 00) && (m_grillei2l == 00))
			return;
		if (m_grillel2i == 00)
			m_grillel2i = m_grillei2l->Invert();
		if (m_grillei2l == 00)
			m_grillei2l = m_grillel2i->Invert();
	}

	DistortionGrilleMATIS::DistortionGrilleMATIS(const DistortionGrilleMATIS &grille) : Transfo2DElementaire(grille)
	{
		m_grillel2i =  new Grille2D(*grille.m_grillel2i);
		m_grillei2l =  new Grille2D(*grille.m_grillei2l);
	}

	void DistortionGrilleMATIS::ApplyLocalToImage(double &c, double &l) const
	{
		m_grillel2i->Apply(c,l);
	}

	void DistortionGrilleMATIS::ApplyImageToLocal(double &c, double &l) const
	{
		m_grillei2l->Apply(c,l);
	}

 	shared_ptr<Transfo2DElementaire> DistortionGrilleMATIS::Clone() const
	{
		return shared_ptr<Transfo2DElementaire>(new DistortionGrilleMATIS(*this));
	}

};
