#include "systematismeaffine.hpp"

#include <iostream>

using namespace OrientationMATIS;

std::string OrientationMATIS::SystematismeAffineTopAero::nomTagXml = std::string("systematismeAffineTopAero");

double SystematismeAffineTopAero::GetS6()const
{
	return _c0;
}
double SystematismeAffineTopAero::GetS7()const
{
	return _c1;
}
double SystematismeAffineTopAero::GetS8()const
{
	return _c2;
}
double SystematismeAffineTopAero::GetC0()const
{
	return _ppaCol;
}
double SystematismeAffineTopAero::GetL0()const
{
	return _ppaLig;
}

SystematismeAffineTopAero::SystematismeAffineTopAero(bool isInterne) : Transfo2DElementaire(isInterne)
{
	_c0 = 0.;
	_c1 = 0.;
	_c2 = 0.;
	_ppaCol = 0.;
	_ppaLig = 0.;
}

SystematismeAffineTopAero::SystematismeAffineTopAero(double ppaCol,double ppaLig, double S6, double S7, double S8, bool isInterne):Transfo2DElementaire(isInterne),
_ppaCol(ppaCol),_ppaLig(ppaLig),_c0(S6),_c1(S7),_c2(S8)
{}

SystematismeAffineTopAero::SystematismeAffineTopAero(const SystematismeAffineTopAero &h) : Transfo2DElementaire(h)
{
	_ppaCol = h._ppaCol;
	_ppaLig = h._ppaLig;
	_c0 = h._c0;
	_c1 = h._c1;
	_c2 = h._c2;
}

shared_ptr<Transfo2DElementaire> SystematismeAffineTopAero::Clone() const
{
	return shared_ptr<Transfo2DElementaire>(new SystematismeAffineTopAero(*this));
}

void SystematismeAffineTopAero::ApplyLocalToImage(double &c, double &l) const
{
	double ci=c, li=l;
	double R=sqrt((ci-_ppaCol)*(ci-_ppaCol)+(li-_ppaLig)*(li-_ppaLig))/100.;
	// l <-> x
	// dx = x(c0+c1*(R-1)+c2(R-1)*(R-1))
	l = li + ((li-_ppaLig)*(_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.)))/100.;
	// dy = y(-c0+c1*(R-1)+c2(R-1)*(R-1))
	c = ci + ((ci-_ppaCol)*(-_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.)))/100.;
	//std::cout << "ecart : en x (ligne) "<<(li-_ppaLig)*(_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))<<" en y (colonne) "<<(ci-_ppaCol)*(-_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))<<std::endl;
	//std::cout << "SystematismeAffineTopAero::ApplyLocalToImage : "<<ci<<" "<<li<<" -> "<<c<<" "<<l<<std::endl;
}

void SystematismeAffineTopAero::ApplyImageToLocal(double &c, double &l) const
{
	// On considere que le R ne varie pas (donc pas besoin de faire des iterations)
	double ci=c, li=l;
	double R=sqrt((ci-_ppaCol)*(ci-_ppaCol)+(li-_ppaLig)*(li-_ppaLig))/100.;
	l = li - (li-_ppaLig)*(_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))/100.;
	c = ci - (ci-_ppaCol)*(-_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))/100.;
}