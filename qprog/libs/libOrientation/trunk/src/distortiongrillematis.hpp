#ifndef __DISTORTIONGRILLEMATIS_HPP__
#define __DISTORTIONGRILLEMATIS_HPP__

#include "transfo2delementaire.hpp"

namespace OrientationMATIS
{
class Grille2D
{
protected:
	long ns, nl;
	long pas;
	double *mDx;
	double *mDy;

public:
	Grille2D(long inNs, long inNl, long inStep);
	Grille2D(const Grille2D &inGrid);
	~Grille2D();

	double *GetDx()
	{
		return mDx;
	}
	double *GetDy()
	{
		return mDy;
	}
	const double *GetDx() const
	{
		return mDx;
	}
	const double *GetDy() const
	{
		return mDy;
	}

	long GetNs() const
	{
		return ns;
	}
	long GetNl() const
	{
		return nl;
	}
	long GetPas() const
	{
		return pas;
	}

	Grille2D *Invert() const;
	void Apply(double &colomn, double &line) const;
};

class DistortionGrilleMATIS: public Transfo2DElementaire
{
	Grille2D *m_grillel2i, *m_grillei2l;

public:
	DistortionGrilleMATIS(Grille2D *grilleg2i, Grille2D *grillei2g, bool isInterne);
	DistortionGrilleMATIS(const DistortionGrilleMATIS &grille);

	void ApplyLocalToImage(double &c, double &l) const;
	void ApplyImageToLocal(double &c, double &l) const;

	shared_ptr<Transfo2DElementaire> Clone() const;
	const Grille2D *GetLocalToImage() const
	{
		return m_grillel2i;
	}
};
}

#endif //#ifndef __DISTORTIONGRILLEMATIS_HPP__
