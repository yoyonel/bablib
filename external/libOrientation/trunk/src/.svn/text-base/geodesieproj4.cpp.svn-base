#include "geodesieproj4.hpp"

#include "../config/config_proj4.hpp"

#if ORIENTATIONMATIS_USE_PROJ4

#include <cmath>
#include <iostream>
#include <sstream>

namespace OrientationMATIS
{
	static const double EarthRay = 6366000.0;

	GeodesiePROJ4::GeodesiePROJ4(const std::string &fullName): Geodesie(fullName), m_local(00)
	{
		m_local_system_name = GetFullName();
		if (m_local_system_name != "")
		{
			if(!(m_local=pj_init_plus(m_local_system_name.c_str())) )
                        {
				std::cout << "[GeodesiePROJ4::GeodesiePROJ4] Initialisation PROJ4 impossible pour \"" << m_local_system_name << "\""<<std::endl;
			}
//			else
//			{
//				std::cout << "[GeodesiePROJ4::GeodesiePROJ4] Initialisation PROJ4 reussie pour \"" << m_local_system_name << "\""<<std::endl;
//			}
		}
	}

	GeodesiePROJ4::~GeodesiePROJ4()
	{
		if (m_local)
			pj_free(m_local);
	}

	bool GeodesiePROJ4::WorldToLocal(const char *world_system_name,
		const double xw, const double yw, const double zw,
		double &xl, double &yl, double &zl) const
	{
		xl = xw;
		yl = yw;
		zl = zw;
		if ((world_system_name == 00) || (GetFullName() == std::string(world_system_name)))
		{
			return true;
		}

		if (std::string(world_system_name) != m_local_system_name)
		{
			projPJ world;
			//initialisation des projections
			if(!(world=pj_init_plus(world_system_name)) )
			{
				std::cout << "[GeodesiePROJ4::WorldToLocal] Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
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
		}
		return true;
	}

	bool GeodesiePROJ4::LocalToWorld( const double xl, const double yl, const double zl,
		const char *world_system_name, double &xw, double &yw, double &zw) const
	{
		xw = xl;
		yw = yl;
		zw = zl;
		if ((world_system_name == 00) || (GetFullName() == std::string(world_system_name)))
		{
			return true;
		}

		if (std::string(world_system_name) == m_local_system_name)
			return true;

		projPJ world;
		if (!(world=pj_init_plus(world_system_name)) )
		{
			std::cout << "[GeodesiePROJ4::LocalToWorld] Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
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
	}
};

#endif //#if ORIENTATIONMATIS_USE_PROJ4
