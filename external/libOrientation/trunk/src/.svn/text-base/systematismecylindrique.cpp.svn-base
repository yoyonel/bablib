#include "systematismecylindrique.hpp"
#include <iostream>

using namespace OrientationMATIS; 

std::string SystematismeCylindriqueTopAero::nomTagXml = std::string("systematismeCylindriqueTopAero");


double SystematismeCylindriqueTopAero::GetS1()const
{
	return _c0;
}
double SystematismeCylindriqueTopAero::GetS2()const
{
	return _c1;
}
double SystematismeCylindriqueTopAero::GetC0()const
{
	return _ppaCol;
}
double SystematismeCylindriqueTopAero::GetL0()const
{
	return _ppaLig;
}
SystematismeCylindriqueTopAero::SystematismeCylindriqueTopAero(bool isInterne) : Transfo2DElementaire(isInterne)
{
	_c0 = 0.;
	_c1 = 0.;
	_ppaCol = 0.;
	_ppaLig = 0.;
}

SystematismeCylindriqueTopAero::SystematismeCylindriqueTopAero(double ppaCol,double ppaLig, double S1, double S2, bool isInterne):Transfo2DElementaire(isInterne),
_ppaCol(ppaCol),_ppaLig(ppaLig),_c0(S1),_c1(S2)
{}

SystematismeCylindriqueTopAero::SystematismeCylindriqueTopAero(const SystematismeCylindriqueTopAero &h) : Transfo2DElementaire(h)
{
	_ppaCol = h._ppaCol;
	_ppaLig = h._ppaLig;
	_c0 = h._c0;
	_c1 = h._c1;
}

shared_ptr<Transfo2DElementaire> SystematismeCylindriqueTopAero::Clone() const
{
	return shared_ptr<Transfo2DElementaire>(new SystematismeCylindriqueTopAero(*this));
}

void SystematismeCylindriqueTopAero::ApplyLocalToImage(double &c, double &l) const
{
	double ci=c, li=l;
	//std::cout << "colonne : "<<ci<<" | "<<(ci-_ppaCol)<<" x "<<_c1<<" = "<<(ci-_ppaCol)*_c1<<std::endl;
	//std::cout << "ligne   : "<<li<<" | "<<(ci-_ppaCol)<<" x "<<_c0<<" = "<<(ci-_ppaCol)*_c0<<std::endl;
	l = li + (ci-_ppaCol)*_c0;
	c = ci + (ci-_ppaCol)*_c1;
	//std::cout << "SystematismeCylindriqueTopAero::ApplyLocalToImage "<<ci<<" "<<li<<" -> "<<c<<" "<<l<<std::endl;
}

void SystematismeCylindriqueTopAero::ApplyImageToLocal(double &c, double &l) const
{
	// On fait l'hypothese que l'on peut negliger les variation de colonne
	double ci=c, li=l;
	l = li - (ci-_ppaCol)*_c0;
	c = ci - (ci-_ppaCol)*_c1;
	//std::cout << "SystematismeCylindriqueTopAero::ApplyImageToLocal "<<ci<<" "<<li<<" -> "<<c<<" "<<l<<std::endl;
}			


