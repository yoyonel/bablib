#ifndef __GEODESIEPROJ4_HPP__
#define __GEODESIEPROJ4_HPP__

#include "../config/config_proj4.hpp"

#if ORIENTATIONMATIS_USE_PROJ4

#include "geodesie.hpp"
#include "proj_api.h"

namespace OrientationMATIS
{

	class GeodesiePROJ4 : public Geodesie
	{
		std::string m_local_system_name;

		projPJ m_local;


	public:

		GeodesiePROJ4(const std::string &fullName);
		~GeodesiePROJ4();

		bool WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const;
		bool LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const;

	};
}; //namespace

#endif //#if ORIENTATIONMATIS_USE_PROJ4

#endif //#ifndef __GEODESIEPROJ4_HPP__
