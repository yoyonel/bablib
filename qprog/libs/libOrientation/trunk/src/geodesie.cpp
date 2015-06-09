#include <iostream>
#include <sstream>
#include "geodesie.hpp"
#include "geodesieproj4.hpp"

#include "../config/config_proj4.hpp"

namespace OrientationMATIS
{
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

	std::string Geodesie::NomSysteme(std::string zoneLambert)
	{
		if(zoneLambert=="2E")
			return "+init=IGNF:LAMBE";

		int zone = atoi(zoneLambert.c_str());
		return NomSysteme(zone);
	}
	 

	shared_ptr<Geodesie> Geodesie::CreateFromName(const std::string &name)
	{
#if ORIENTATIONMATIS_USE_PROJ4
		return shared_ptr<Geodesie> (new GeodesiePROJ4(name));
#else //#if ORIENTATIONMATIS_USE_PROJ4
		return shared_ptr<Geodesie> (new GeodesieNULLE(name));
#endif //#if ORIENTATIONMATIS_USE_PROJ4
	}

	Geodesie::Geodesie(const std::string &nom) : m_systemeGeodesie(nom) {}

	Geodesie::~Geodesie() {}


	GeodesieNULLE::GeodesieNULLE(const std::string &nom): Geodesie(nom)
	{
	}

	bool GeodesieNULLE::WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const
	{
		xl = xw;
		yl = yw;
		zl = zw;
		if (std::string(world_system_name) == GetFullName())
		{
			return true;
		}
		return false;
	}
	bool GeodesieNULLE::LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const
	{
		xw = xl;
		yw = yl;
		zw = zl;
		if (std::string(world_system_name) == GetFullName())
			return true;
		return false;
	}

}

