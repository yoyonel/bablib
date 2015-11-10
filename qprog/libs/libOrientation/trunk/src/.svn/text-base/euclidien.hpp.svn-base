#ifndef __EUCLIDIEN_HPP__
#define __EUCLIDIEN_HPP__

#include <string>

#include "../config/config_proj4.hpp"

#if ORIENTATIONMATIS_USE_PROJ4
#include "projects.h"
#include "proj_api.h"
#endif

namespace OrientationMATIS
{
	class Euclidien
		{
		public:
			virtual bool WorldToEuclidien(const char *world_system_name,const double xw, const double yw, const double zw, double &xe, double &ye, double &ze) const = 0;
			virtual bool EuclidienToWorld(const double xe, const double ye, const double ze, const char *world_system_name, double &xw, double &yw, double &ew) const = 0;
			virtual ~Euclidien(){;}
		};
	
	class EuclidienMATISRTL: public Euclidien
		{
			double m_localOrigin[2];
		public:
			EuclidienMATISRTL()
			{
				m_localOrigin[0] = 0.;
				m_localOrigin[1] = 0.;
			}
			
			EuclidienMATISRTL(const double localOriginX, const double localOriginY)
			{
				m_localOrigin[0] = localOriginX;
				m_localOrigin[1] = localOriginY;
			}
			
			~EuclidienMATISRTL()
			{;}
			
			void GetLocalOrigin(double &localOriginX,double &localOriginY)const
			{
				localOriginX=m_localOrigin[0];
				localOriginY=m_localOrigin[1];
			}
		
			bool WorldToEuclidien(const char *world_system_name,const double xw, const double yw, const double zw, double &xe, double &ye, double &ze) const;
			bool EuclidienToWorld(const double xe, const double ye, const double ze,const char *world_system_name, double &xw, double &yw, double &zw) const;
		};
	
	class EuclidienTOPAERO: public Euclidien
		{
			double m_localOrigin[2];
			double rot2EuclidienLocal[9];
			std::string m_sysProjection;
			std::string m_sysGeographique;
			double origineGeo[3];
			
#if ORIENTATIONMATIS_USE_PROJ4
			projPJ pProjection;
			projPJ pGeographique;
			
#endif
		public:
			EuclidienTOPAERO()
			{;}
			
			EuclidienTOPAERO(std::string const &localSystem,std::string const &sysGeographique,double origin[2]);
			
			~EuclidienTOPAERO();
			
			bool WorldToEuclidien(const char *world_system_name,const double xw, const double yw, const double zw, double &xe, double &ye, double &ze) const;
			bool EuclidienToWorld(const double xe, const double ye, const double ze,const char *world_system_name, double &xw, double &yw, double &zw) const;
			
			void GetLocalOrigin(double &localOriginX,double &localOriginY)const;
			const double * GetMatrice()const;
			std::string GetSysProjection()const;
			std::string GetSysGeographique()const;
		};
	
	
}

#endif
