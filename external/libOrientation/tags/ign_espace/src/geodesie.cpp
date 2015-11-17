#include <iostream>
#include <sstream>
#include <cmath>

#if ORIENTATIONMATIS_USE_PROJ4
#include "proj_api.h"
#endif //#if ORIENTATIONMATIS_USE_PROJ4

#include "geodesie.hpp"

namespace OrientationMATIS
{
	static const double EarthRay = 6366000.0;
	std::string Geodesie::NomSysteme(int zoneLambert)
	{
		switch (zoneLambert) 
		{
			case 1:
			return "+init=IGNF:LAMB1C";
			case 2:
			return "+init=IGNF:LAMB2C";
			case 3:
			return "+init=IGNF:LAMB3C";
			case 4:
			return "+init=IGNF:LAMB4C";
			case 93:
			return "+init=IGNF:LAMB93";
		};
		return "";
	}

	Geodesie::Geodesie(const std::string &fullName): m_systemeGeodesie(fullName), m_local(00)
	{
		m_isRTL = (m_systemeGeodesie.compare(0,8, "MATISRTL") == 0);
		if (m_isRTL)
		{
			std::size_t pos = m_systemeGeodesie.find_first_of("+");
			if (pos == std::string::npos)
			{
				std::cout << "Comprehesion de " << m_systemeGeodesie << "impossible."<<std::endl;
				return;
			}
			m_local_system_name = m_systemeGeodesie.c_str() + pos;

			std::istringstream iss(m_systemeGeodesie);
			std::string rien;
			iss >> rien >> m_localOrigin[0] >> m_localOrigin[1];
		}
		else
		{
			m_local_system_name = m_systemeGeodesie;
            m_localOrigin[0] = m_localOrigin[1] = 0.;
		}
#if ORIENTATIONMATIS_USE_PROJ4
		if (m_local_system_name != "")
			if(!(m_local=pj_init_plus(m_local_system_name.c_str())) )
				std::cout << "Initialisation PROJ4 impossible pour \"" << m_local_system_name << "\""<<std::endl;
#endif //#if ORIENTATIONMATIS_USE_PROJ4
	}
	
	Geodesie::~Geodesie()
	{
#if ORIENTATIONMATIS_USE_PROJ4
		if (m_local)
			pj_free(m_local);
#endif //#if ORIENTATIONMATIS_USE_PROJ4
	}

	bool Geodesie::WorldToLocal(const char *world_system_name, 
		const double xw, const double yw, const double zw, 
		double &xl, double &yl, double &zl) const
	{
		xl = xw;
		yl = yw;
		zl = zw;
		if ((world_system_name == 00) || (m_systemeGeodesie == std::string(world_system_name)))
		{
			return true;
		}

		if (std::string(world_system_name) != m_local_system_name)
		{
#if ORIENTATIONMATIS_USE_PROJ4
			projPJ world;
			//initialisation des projections
			if(!(world=pj_init_plus(world_system_name)) )
			{
				std::cout << "Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
				return false;
			}

			//transformation de coordonnees proj4
			xl = xw;
			yl = yw;
			zl = zw;
			int err = pj_transform(world, m_local,1,0,&xl,&yl,&zl);
			pj_free(world);
			if ( err != 0)
			{
				std::cout << "Erreur pj_transform : " << err << std::endl;
				std::cout << "Lors de la transformation de " << world_system_name << " vers " << m_local_system_name << std::endl;
				return false;
			}
#else //#if ORIENTATIONMATIS_USE_PROJ4
			return false;
#endif //#if ORIENTATIONMATIS_USE_PROJ4
		}
		if (m_isRTL)
		{
			double UU[2] ;
			UU[0] = xl - m_localOrigin[0] ;
			UU[1] = yl - m_localOrigin[1] ;
			double TT = ( UU[0] * UU[0] + UU[1] * UU[1] ) / ( 4.0 * EarthRay ) ;
			double CC = ( EarthRay + zl ) / ( TT + EarthRay ) ;
			xl = UU[0] * CC ;
			yl = UU[1] * CC ;
			zl = ( (EarthRay - TT) * CC ) - EarthRay ;

		}
		return true;
	}
	
	bool Geodesie::LocalToWorld( const double xl, const double yl, const double zl, 
		const char *world_system_name, double &xw, double &yw, double &zw) const
	{
		xw = xl;
		yw = yl;
		zw = zl;
		if ((world_system_name == 00) || (m_systemeGeodesie == std::string(world_system_name)))
		{
			return true;
		}

		if (m_isRTL)
		{
			double RZ = EarthRay + zl ;
			double RH = ::sqrt( (RZ*RZ) + (xl*xl) + (yl*yl) ) ;

			RZ = (2.0 * EarthRay ) / ( RZ + RH );
			xw = m_localOrigin[0] + ( xl * RZ ) ;
			yw = m_localOrigin[1] + ( yl * RZ ) ;
			zw = RH - EarthRay ;
		}

		if (std::string(world_system_name) == m_local_system_name)
			return true;

#if ORIENTATIONMATIS_USE_PROJ4
		projPJ world;
		if (!(world=pj_init_plus(world_system_name)) )
		{
			std::cout << "Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
			return false;
		}

		//transformation de coordonnees
		int err = pj_transform(m_local, world,1,0,&xw,&yw,&zw);
		pj_free(world);

		if ( err != 0)
		{
			return false;
		}
		return true;
#else //#if ORIENTATIONMATIS_USE_PROJ4
		return false;
#endif //#if ORIENTATIONMATIS_USE_PROJ4
	}
};

