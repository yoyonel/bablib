#ifndef __MODELEPROJECTIONSPHERIQUE_HPP__
#define __MODELEPROJECTIONSPHERIQUE_HPP__

#include "modeleprojectionphysique.hpp"

namespace OrientationMATIS
{
	class ModeleProjectionSpherique  : public ModeleProjectionPhysique
	{
	protected :
        double m_cPas, m_lPas;
        double m_cPPA, m_lPPA;

	public :
		ModeleProjectionSpherique(unsigned int nc, unsigned int nl,
								AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie,
								const ModelePhysiqueExtrinseque &extr, 
								double cPas, double lPas, double cPPA, double lPPA);
		ModeleProjectionSpherique(const ModeleProjectionSpherique &mod);

		shared_ptr<ModeleProjection> Clone() const
		{
			return shared_ptr<ModeleProjection>( new ModeleProjectionSpherique(*this) );
		}

		double GetcPas() const { return m_cPas; }
        double GetlPas() const { return m_lPas; }

		void IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const;
		void IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;
	};
};

#endif //#ifndef __MODELEPROJECTIONSPHERIQUE_HPP__
