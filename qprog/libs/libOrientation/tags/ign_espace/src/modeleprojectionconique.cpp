#include "modeleprojectionconique.hpp"

namespace OrientationMATIS
{
		ModeleProjectionConique::ModeleProjectionConique(unsigned int nc, unsigned int nl,
								AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie,
								const ModelePhysiqueExtrinseque &extr, 
								double focal, double cPPA, double lPPA) : 
					ModeleProjectionPhysique(nc, nl, data, transfo2D, systemeGeodesie, extr), 
					m_focal(focal), m_cPPA(cPPA), m_lPPA(lPPA)
		{;}
		
		ModeleProjectionConique::ModeleProjectionConique(const ModeleProjectionConique &mod) : ModeleProjectionPhysique(mod), m_focal(mod.m_focal), m_cPPA(mod.m_cPPA), m_lPPA(mod.m_lPPA)
		{;}

		void ModeleProjectionConique::IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const
		{
			c = xt * m_focal / zt;
			l = yt * m_focal / zt;
			c += m_cPPA;
			l += m_lPPA;
		}

		void ModeleProjectionConique::IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
		{
			x0=y0=z0= 0.;
			x1 = c - m_cPPA;
			y1 = l - m_lPPA;
			z1 = m_focal;
        }    
};
