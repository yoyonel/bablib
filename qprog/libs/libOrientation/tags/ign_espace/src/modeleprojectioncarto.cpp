#include "modeleprojectioncarto.hpp"

namespace OrientationMATIS
{
	ModeleProjectionCarto::ModeleProjectionCarto(unsigned int nc, unsigned int nl, AuxiliaryData data, 
			Transfo2D transfo2D, std::string systemeGeodesie, double position_origine_x, 
							double position_origine_y, double pas_x, double pas_y) 
	: ModeleProjection(nc,nl,data,transfo2D,systemeGeodesie), m_position_origine_x(position_origine_x), m_position_origine_y(position_origine_y), m_pas_x(pas_x), m_pas_y(pas_y)
	{;}

	ModeleProjectionCarto::ModeleProjectionCarto(const ModeleProjectionCarto &mod) : ModeleProjection(mod), m_position_origine_x(mod.m_position_origine_x), m_position_origine_y(mod.m_position_origine_y), m_pas_x(mod.m_pas_x), m_pas_y(mod.m_pas_y)
	{;}


	void ModeleProjectionCarto::LocalToImage(double x, double y, double z, double &c, double &l) const
	{
		c = (x - m_position_origine_x) / m_pas_x;
		l = (m_position_origine_y - y) / m_pas_y;
		m_transfo2D.ApplyLocalToImage(c,l);
	}

	void ModeleProjectionCarto::ImageAndZToLocal(double c, double l, double z, double &x, double &y) const
	{
		m_transfo2D.ApplyImageToLocal(c,l);
		x = c * m_pas_x + m_position_origine_x;
		y = m_position_origine_y - l * m_pas_y;			
	}

	void ModeleProjectionCarto::ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
	{
		m_transfo2D.ApplyImageToLocal(c,l);
		x0 = c * m_pas_x + m_position_origine_x;
		y0 = m_position_origine_y - l * m_pas_y;
		z0 = 0;
		x1 = x0;
		y1 = y0;
		z1 = 1;
	}
} // namespace
