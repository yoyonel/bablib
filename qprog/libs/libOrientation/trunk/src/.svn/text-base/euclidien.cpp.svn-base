#include "euclidien.hpp"
#include <iostream>
#include <cmath>

namespace OrientationMATIS
{
	static const double EarthRay = 6366000.0;
	
	bool EuclidienMATISRTL::WorldToEuclidien(const char *world_system_name,const double xw, const double yw, const double zw, double &xe, double &ye, double &ze) const
	{
		double UU[2] ;
		UU[0] = xw - m_localOrigin[0] ;
		UU[1] = yw - m_localOrigin[1] ;
		double TT = ( UU[0] * UU[0] + UU[1] * UU[1] ) / ( 4.0 * EarthRay ) ;
		double CC = ( EarthRay + zw ) / ( TT + EarthRay ) ;
		xe = UU[0] * CC ;
		ye = UU[1] * CC ;
		ze = ( (EarthRay - TT) * CC ) - EarthRay ;
		return true;
	}

	bool EuclidienMATISRTL::EuclidienToWorld(const double xe, const double ye, const double ze,const char *world_system_name, double &xw, double &yw, double &zw) const
	{
		double RZ = EarthRay + ze ;
		double RH = ::sqrt( (RZ*RZ) + (xe*xe) + (ye*ye) ) ;
		RZ = (2.0 * EarthRay ) / ( RZ + RH );
		xw = m_localOrigin[0] + ( xe * RZ ) ;
		yw = m_localOrigin[1] + ( ye * RZ ) ;
		zw = RH - EarthRay ;
		return true;
	}

	EuclidienTOPAERO::EuclidienTOPAERO(std::string const &sysProjection,
									   std::string const &sysGeographique,
									   double
									   origin[2]):m_sysProjection(sysProjection),m_sysGeographique(sysGeographique)
#if ORIENTATIONMATIS_USE_PROJ4
	,pProjection(00),pGeographique(00)
#endif
	
	{
		for( int i=0;i<2;++i)
			m_localOrigin[i] = origin[i];
#if ORIENTATIONMATIS_USE_PROJ4
		bool verbose=false;
		if(!(pProjection=pj_init_plus(sysProjection.c_str())) )
		{
			std::cout << "[EuclidienTOPAERO::EuclidienTOPAERO] Initialisation PROJ4 impossible pour le systeme : \""<<sysProjection<<"\""<<std::endl;
			return;
		}
		if (verbose) std::cout << "sysGeographique : "<<sysGeographique<<std::endl;
		pGeographique = pj_init_plus(sysGeographique.c_str());
		if (verbose) std::cout << "is_latlong : "<<pj_is_latlong(pGeographique)<<std::endl;
		if (verbose) std::cout << "is_geocent : "<<pj_is_geocent(pGeographique)<<std::endl;
		if (!pj_is_latlong(pGeographique))
		{
			projPJ tmp = pGeographique;
			pGeographique = pj_latlong_from_proj(tmp);
			pj_free(tmp);
		}
		if (!pGeographique)
		{
			std::cout << "[EuclidienTOPAERO::EuclidienTOPAERO] Initialisation PROJ4 impossible pour le systeme : \""<<sysGeographique<<"\""<<std::endl;
			return;
		}
		// Passage de l'origine de la projection en coord geographique
		double latitude=origin[0],longitude=origin[1],znul=0.;
		if (verbose) std::cout << std::fixed << "coord en projection : "<<origin[0]<<" "<<origin[1]<<std::endl;
		int code_retour = pj_transform(pProjection,pGeographique,1,1,&latitude,&longitude,&znul);
		if (verbose) std::cout << std::fixed << "("<<code_retour<<") coord geographique : "<<latitude<<" "<<longitude<<std::endl;
		// Passage de l'origine en coord cartesienne
		origineGeo[0] = latitude;
		origineGeo[1] = longitude;
		origineGeo[2] = 0.;
		code_retour= pj_geodetic_to_geocentric( pGeographique->a, pGeographique->es,1,1, &origineGeo[0], &origineGeo[1], &origineGeo[2] );
		if (verbose) std::cout << "("<<code_retour<<") coord en geocentrique : "<<origineGeo[0]<<" "<<origineGeo[1]<<" "<<origineGeo[2]<<std::endl;
				
		struct FACTORS facs;
		LP position;
		position.u = latitude;
		position.v = longitude;
		 
		
		pj_factors(position, pProjection, 0., &facs);
		
		double gamma = facs.conv;
		if (verbose) std::cout << "gamma : "<<gamma<<std::endl;
		
		//Matrice de passage en coordonnÈes cartÈsiennes locales
		double sl = sin(latitude);
		double sp = sin(longitude);
		double sg = sin(gamma);
		double cl = cos(latitude);
		double cp = cos(longitude);
		double cg = cos(gamma);
		rot2EuclidienLocal[0] = -cg*sl-sg*sp*cl;
		rot2EuclidienLocal[1] = cg*cl-sg*sp*sl;
		rot2EuclidienLocal[2] = sg*cp;
		rot2EuclidienLocal[3] = sg*sl-cg*sp*cl;
		rot2EuclidienLocal[4] = -sg*cl-cg*sp*sl;
		rot2EuclidienLocal[5] = cg*cp;
		rot2EuclidienLocal[6] = cp*cl;
		rot2EuclidienLocal[7] = cp*sl;
		rot2EuclidienLocal[8] = sp;
		
		if (verbose) std::cout << "rot2EuclidienLocal : "<<std::endl;
		for(int l=0;l<3;++l)
		{
			for(int c=0;c<3;++c)
			{
				std::cout << rot2EuclidienLocal[3*l+c] << " ";
			}
			std::cout << std::endl;
		}
#else
		std::cout << "PB on a besoin de proj4"<<std::endl;
#endif
	}

	bool EuclidienTOPAERO::WorldToEuclidien(const char *world_system_name,const double xw, const double yw, const double zw, double &xe, double &ye, double &ze) const
	{
#if ORIENTATIONMATIS_USE_PROJ4
		// Passage en coord geographique
		double latitude=xw,longitude=yw/*,h=0.*/;
		bool verbose=false;
		
		if (std::string(world_system_name) != m_sysGeographique)
		{
			projPJ world;
			if(!(world=pj_init_plus(world_system_name)) )
			{
				std::cout << "[EuclidienTOPAERO::WorldToEuclidien] Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
				return false;
			}
			double znul=0.;
			pj_transform(world, pGeographique, 1, 1, &latitude, &longitude, &znul);
			pj_free(world);
		}
		double positionGeo[3];
		positionGeo[0] = latitude;
		positionGeo[1] = longitude;
		positionGeo[2] = zw;			
		pj_geodetic_to_geocentric( pGeographique->a, pGeographique->es,1,1, &positionGeo[0], &positionGeo[1], &positionGeo[2] );
		
		// Passage en coord locales
		double dx, dy, dz;
		dx = positionGeo[0] - origineGeo[0];
		dy = positionGeo[1] - origineGeo[1];
		dz = positionGeo[2] - origineGeo[2];
		xe = rot2EuclidienLocal[0]*dx + rot2EuclidienLocal[1]*dy + rot2EuclidienLocal[2]*dz + m_localOrigin[0];
		ye = rot2EuclidienLocal[3]*dx + rot2EuclidienLocal[4]*dy + rot2EuclidienLocal[5]*dz + m_localOrigin[1];
		ze = rot2EuclidienLocal[6]*dx + rot2EuclidienLocal[7]*dy + rot2EuclidienLocal[8]*dz;
		
		if (verbose) std::cout << "[EuclidienTOPAERO::WorldToEuclidien] : "<<xw<<" "<<yw<<" "<<zw<<" ---> "<<xe<<" "<<ye<<" "<<ze<<std::endl;
		if (verbose) std::cout << "[EuclidienTOPAERO::WorldToEuclidien] : sans origine "<<xe- m_localOrigin[0]<<" "<<ye-m_localOrigin[1]<<" "<<ze<<std::endl;
		return true;
#else
		return false;
#endif
	}

	bool EuclidienTOPAERO::EuclidienToWorld(const double xe, const double ye, const double ze,const char *world_system_name, double &xw, double &yw, double &zw) const
	{
#if ORIENTATIONMATIS_USE_PROJ4
		bool verbose=false;
		double dx = xe - m_localOrigin[0];
		double dy = ye - m_localOrigin[1];
		double dz = ze;
		if (verbose) std::cout << "d : "<<dx<<" "<<dy<<" "<<dz<<std::endl;
		double positionGeo[3];
		positionGeo[0] = rot2EuclidienLocal[0]*dx + rot2EuclidienLocal[3]*dy + rot2EuclidienLocal[6]*dz + origineGeo[0];
		positionGeo[1] = rot2EuclidienLocal[1]*dx + rot2EuclidienLocal[4]*dy + rot2EuclidienLocal[7]*dz + origineGeo[1];
		positionGeo[2] = rot2EuclidienLocal[2]*dx + rot2EuclidienLocal[5]*dy + rot2EuclidienLocal[8]*dz + origineGeo[2];	
		if (verbose) std::cout << "positionGeo : "<<positionGeo[0]<<" "<<positionGeo[1]<<" "<<positionGeo[2]<<std::endl;
		// Passage en coord geographique
		double lambda_phi_h[3];
		lambda_phi_h[0] = positionGeo[0];
		lambda_phi_h[1] = positionGeo[1];
		lambda_phi_h[2] = positionGeo[2];
		pj_geocentric_to_geodetic( pGeographique->a, pGeographique->es,1,1, &lambda_phi_h[0], &lambda_phi_h[1], &lambda_phi_h[2] );
		if (verbose) std::cout << "lambda_phi_h : "<<lambda_phi_h[0]<<" "<<lambda_phi_h[1]<<" "<<lambda_phi_h[2]<<std::endl;
		if (std::string(world_system_name) != m_sysGeographique)
		{
			//std::cout << "passage de : "<<m_sysGeographique<<" vers : "<<world_system_name<<std::endl; 
			projPJ world;
			if(!(world=pj_init_plus(world_system_name)) )
			{
				std::cout << "[EuclidienTOPAERO::EuclidienToWorld] Initialisation PROJ4 impossible pour " << world_system_name << std::endl;
				return false;
			}
			double znul=0.;
			pj_transform(pGeographique, world, 1, 1, &lambda_phi_h[0], &lambda_phi_h[1], &znul);
			if (verbose) std::cout << "res : "<<lambda_phi_h[0]<<" "<<lambda_phi_h[1]<<" "<<lambda_phi_h[2]<<std::endl;
			pj_free(world);
		}
		xw = lambda_phi_h[0];
		yw = lambda_phi_h[1];
		zw = lambda_phi_h[2];
		if (verbose) std::cout << "[EuclidienTOPAERO::EuclidienToWorld] : "<<xe<<" "<<ye<<" "<<ze<<" -> "<<xw<<" "<<yw<<" "<<zw<<std::endl;
		return true;
#else
		return false;
#endif
	}

	void EuclidienTOPAERO::GetLocalOrigin(double &localOriginX,double &localOriginY)const
	{
		localOriginX = m_localOrigin[0];
		localOriginY = m_localOrigin[1];
	}
	const double* EuclidienTOPAERO::GetMatrice()const
	{
		return rot2EuclidienLocal;
	}
	std::string EuclidienTOPAERO::GetSysProjection()const
	{
		return m_sysProjection;
	}
	std::string EuclidienTOPAERO::GetSysGeographique()const
	{
		return m_sysGeographique;
	}
	
	EuclidienTOPAERO::~EuclidienTOPAERO()
	{
#if ORIENTATIONMATIS_USE_PROJ4
		if (pProjection)
			pj_free(pProjection);
		if (pGeographique)
			pj_free(pGeographique);
#endif
	}
	
}

