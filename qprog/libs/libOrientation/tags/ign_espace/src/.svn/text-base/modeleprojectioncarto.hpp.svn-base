#ifndef __MODELEPROJECTIONCARTO_HPP__
#define __MODELEPROJECTIONCARTO_HPP__

#include "modeleprojection.hpp"

namespace OrientationMATIS
{
	class ModeleProjectionCarto : public ModeleProjection
	{
	public :
		 		
		ModeleProjectionCarto(unsigned int nc, unsigned int nl, AuxiliaryData data, 
				Transfo2D transfo2D, std::string systemeGeodesie, double position_origine_x, 
								double position_origine_y, double pas_x, double pas_y) ;
		ModeleProjectionCarto(const ModeleProjectionCarto &mod);

		shared_ptr<ModeleProjection> Clone() const
		{
			return shared_ptr<ModeleProjectionCarto>( new ModeleProjectionCarto(*this) );
		}

		void LocalToImage(double x, double y, double z, double &c, double &l) const;
		void ImageAndZToLocal(double c, double l, double z, double &x, double &y) const;

		void ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;

		double GetXOrigin() const { return m_position_origine_x;}
		double GetYOrigin() const { return m_position_origine_y;}
		double GetXStep() const { return m_pas_x;}
		double GetYStep() const { return m_pas_y;}

	protected :
		double m_position_origine_x, m_position_origine_y, m_pas_x, m_pas_y;
	};
} // namespace
#endif
