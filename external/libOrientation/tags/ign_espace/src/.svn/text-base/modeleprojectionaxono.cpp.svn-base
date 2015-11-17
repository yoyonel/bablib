#include "modeleprojectionaxono.hpp"

namespace OrientationMATIS
{
	ModeleProjectionAxono::ModeleProjectionAxono(unsigned int nc, unsigned int nl,
							AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie,
							const ModelePhysiqueExtrinseque &extr, 
							double cPas, double lPas, double cPPA, double lPPA) : 
						ModeleProjectionPhysique(nc, nl, data, transfo2D, systemeGeodesie, extr), 
						m_cPas(cPas), m_lPas(lPas), m_cPPA(cPPA), m_lPPA(lPPA)
	{;}

	ModeleProjectionAxono::ModeleProjectionAxono(const ModeleProjectionAxono &mod) : 
		ModeleProjectionPhysique(mod), m_cPas(mod.m_cPas), m_lPas(mod.m_lPas), m_cPPA(mod.m_cPPA), m_lPPA(mod.m_lPPA)
	{;}

	void ModeleProjectionAxono::IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const
	{
		c = xt / m_cPas;
		l = yt / m_lPas;
		c += m_cPPA;
		l += m_lPPA;
	}

	void ModeleProjectionAxono::IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
	{
		x0=x1= (c - m_cPPA) * m_cPas;
		y0=y1= (l - m_lPPA) * m_lPas;
		z0 = 0.; z1 = 1.;
	}

};
