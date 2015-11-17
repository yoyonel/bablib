#ifndef __GEODESIE_HPP__
#define __GEODESIE_HPP__

#include <string>

#include "noboost.hpp"

namespace OrientationMATIS
{
	class Geodesie
	{
		std::string m_systemeGeodesie;
        Geodesie(const Geodesie&geo) ;

	public:
		/// Transforme une zone Lambert en son nom PROJ4
		static std::string NomSysteme(int zoneLambert);
		static std::string NomSysteme(std::string zoneLambert);
		
		static shared_ptr<Geodesie> CreateFromName(const std::string &name);

		Geodesie(const std::string &nom);
		virtual ~Geodesie();

		std::string GetFullName() const { return m_systemeGeodesie; }
		
		virtual bool WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const =0;
		virtual bool LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const =0;

	};

	class GeodesieNULLE:public Geodesie
		{			
		public:
			GeodesieNULLE(const std::string &nom);
			bool WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const;
			bool LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const;			
		};
	
	
	
} // namespace

#endif //#ifndef __GEODESIE_HPP__
