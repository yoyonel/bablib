#ifndef __GEODESIE_HPP__
#define __GEODESIE_HPP__

#include <string>

namespace OrientationMATIS
{
	class Geodesie
	{
		std::string m_systemeGeodesie, m_local_system_name;
		bool m_isRTL;
		double m_localOrigin[2];

        // Contient la projection au sens PROJ4
        //  typedef void *projPJ;
		void * m_local;

        Geodesie(const Geodesie&geo) ;

	public:
		/// TRansforme une zone Lambert en son nom PROJ4
		static std::string NomSysteme(int zoneLambert);

		Geodesie(const std::string &fullName);	
		~Geodesie();

		std::string GetFullName() const { return m_systemeGeodesie; }
		
		bool WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const;
		bool LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const;

	};
} // namespace

#endif //#ifndef __GEODESIE_HPP__
