#include <cmath>

using namespace std;

#include "modeleprojection.hpp"


namespace OrientationMATIS
{

bool ModeleProjection::InternalCheck(double precision) const
{
	const std::vector<Sample> &samples = GetAuxiliaryData().GetSamples();
	for (unsigned int i = 0; i < samples.size(); ++i)
	{
		const Sample &s = samples[i];
		double c, l;
		if (s.TestType == Sample::TestProjWToI)
		{
			if (!WorldToImage(s.systeme_geodesique.c_str(), s.x, s.y, s.z, c, l))
				return false;
			double dc = s.c - c;
			double dl = s.l - l;
			if ((std::abs(dc) > precision) || (std::abs(dl) > precision))
			{
				std::cout << "ModeleProjection::InternalCheck WorldToImage problem" << std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout << "x:" << s.x << ";y:" << s.y << ";z:" << s.z << ";c:" << s.c << ";l:" << s.l << ";systeme :" << s.systeme_geodesique << std::endl;
				std::cout << "Calcule : c:" << c << ";l:" << l << std::endl;
				std::cout << "Ecart:dc:" << dc << ";dl:" << dl << std::endl;
				return false;
			}
		} // end TestProjWToI
		else if (s.TestType == Sample::TestTransfo2D)
		{
			/////////////////////////////////////
			// test Transfo2D::ApplyImageToLocal
			c = s.c;
			l = s.l;
			m_transfo2D.ApplyImageToLocal(c, l);
			double dc = s.x - c;
			double dl = s.y - l;
			//			std::cout<<"ApplyImageToLocal : \n";
			//			std::cout<<"c = "<<c<< " x = "<<s.x<<std::endl;
			//			std::cout<<"l = "<<l<< " y = "<<s.y<<std::endl;
			if ((std::abs(dc) > precision) || (std::abs(dl) > precision))
			{
				std::cout << "ModeleProjection::InternalCheck Transfo2D::ApplyImageToLocal  problem" << std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout << "ci:" << s.c << " li:" << s.l << "--> cg:" << s.x << " l:" << s.y << std::endl;
				std::cout << "Calcule : cg:" << c << " lg:" << l << std::endl;
				std::cout << "Ecart:dc:" << dc << " dl:" << dl << std::endl;
				return false;

			}
			/////////////////////////////////////
			// test Transfo2D::ApplyLocalToImage
			c = s.x;
			l = s.y;
			m_transfo2D.ApplyLocalToImage(c, l);
			//			std::cout<<"ApplyLocalToImage : \n";
			//			std::cout<<"c = "<<c<< " s.c = "<<s.c<<std::endl;
			//			std::cout<<"l = "<<l<< " s.l = "<<s.l<<std::endl;

			dc = s.c - c;
			dl = s.l - l;
			if ((std::abs(dc) > precision) || (std::abs(dl) > precision))
			{
				std::cout << "ModeleProjection::InternalCheck Transfo2D::ApplyLocalToImage" << std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout << "ci:" << s.c << " li:" << s.l << "--> cg:" << s.x << " l:" << s.y << std::endl;
				std::cout << "Calcule : ci:" << c << " li:" << l << std::endl;
				std::cout << "Ecart:dc:" << dc << " dl:" << dl << std::endl;
				return false;
			}
		}// end TestTransfo2D

	}
	return true;
}

bool ModeleProjection::WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const
{
	// Coord dans le systeme World 'interne'
	double xwl, ywl, zwl;
	bool res = m_geodesie->WorldToLocal(world_system_name, xw, yw, zw, xwl, ywl, zwl);
	if (!res)
		return false;
	// Passage dans le systeme euclidien
	if (m_euclidien.get())
	{
		res = m_euclidien->WorldToEuclidien(world_system_name, xwl, ywl, zwl, xl, yl, zl);
	}
	else
	{
		xl = xwl;
		yl = ywl;
		zl = zwl;
	}
	return res;
}

bool ModeleProjection::LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const
{
	// Coord dans le systeme World 'interne'
	double xwl, ywl, zwl;
	bool res = true;
	// Passage du systeme euclidien vers le world 'interne'
	if (m_euclidien.get())
	{
		res = m_euclidien->EuclidienToWorld(xl, yl, zl, world_system_name, xwl, ywl, zwl);
		if (!res)
			return false;
	}
	else
	{
		xwl = xl;
		ywl = yl;
		zwl = zl;
	}
	return m_geodesie->LocalToWorld(xwl, ywl, zwl, world_system_name, xw, yw, zw);
}

bool ModeleProjection::WorldToImage(const char *world_system, const double xw, const double yw, const double zw, double &c, double &l) const
{
	double xl, yl, zl;
	bool res = WorldToLocal(world_system, xw, yw, zw, xl, yl, zl);
	if (res)
		LocalToImage(xl, yl, zl, c, l);
	return res;
}

void ModeleProjection::FootprintHelper(double z_min, double z_max, double c0, double l0, double c1, double l1, std::vector<double> &coord_x, std::vector<double> &coord_y)
{
	double x, y;
	coord_x.reserve(8);
	coord_y.reserve(8);

	ImageAndZToLocal(c0, l0, z_min, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);
	ImageAndZToLocal(c0, l0, z_max, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);

	ImageAndZToLocal(c1, l0, z_min, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);
	ImageAndZToLocal(c1, l0, z_max, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);

	ImageAndZToLocal(c0, l1, z_min, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);
	ImageAndZToLocal(c0, l1, z_max, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);

	ImageAndZToLocal(c1, l1, z_min, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);
	ImageAndZToLocal(c1, l1, z_max, x, y);
	coord_x.push_back(x);
	coord_y.push_back(y);
}

void ModeleProjection::FootprintHelper(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, std::vector<double> &coord_x, std::vector<double> &coord_y)
{
	double x, y, xw, yw, zw;
	coord_x.reserve(8);
	coord_y.reserve(8);

	ImageAndZToLocal(c0, l0, z_min, x, y);
	LocalToWorld(x, y, z_min, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);
	ImageAndZToLocal(c0, l0, z_max, x, y);
	LocalToWorld(x, y, z_max, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);

	ImageAndZToLocal(c1, l0, z_min, x, y);
	LocalToWorld(x, y, z_min, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);
	ImageAndZToLocal(c1, l0, z_max, x, y);
	LocalToWorld(x, y, z_max, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);

	ImageAndZToLocal(c0, l1, z_min, x, y);
	LocalToWorld(x, y, z_min, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);
	ImageAndZToLocal(c0, l1, z_max, x, y);
	LocalToWorld(x, y, z_max, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);

	ImageAndZToLocal(c1, l1, z_min, x, y);
	LocalToWorld(x, y, z_min, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);
	ImageAndZToLocal(c1, l1, z_max, x, y);
	LocalToWorld(x, y, z_max, world_system_name, xw, yw, zw);
	coord_x.push_back(xw);
	coord_y.push_back(yw);
}

void ModeleProjection::FootprintLocal(double z_min, double z_max, double c0, double l0, double c1, double l1, double &xmin, double &xmax, double &ymin, double &ymax)
{
	vector<double> coord_x, coord_y;
	FootprintHelper(z_min, z_max, c0, l0, c1, l1, coord_x, coord_y);

	xmin = *min_element(coord_x.begin(), coord_x.end());
	xmax = *max_element(coord_x.begin(), coord_x.end());
	ymin = *min_element(coord_y.begin(), coord_y.end());
	ymax = *max_element(coord_y.begin(), coord_y.end());
}

void ModeleProjection::FootprintLocal(double z_min, double z_max, double &xmin, double &xmax, double &ymin, double &ymax)
{
	FootprintLocal(z_min, z_max, 0., 0., m_width, m_height, xmin, xmax, ymin, ymax);
}

#if ORIENTATIONMATIS_USE_CGAL
void ModeleProjection::FootprintLocal(double z_min, double z_max, double c0, double l0, double c1, double l1, footprint_type &footprint)
{
	footprint_type all_points;

	vector<double> coord_x, coord_y;
	FootprintHelper(z_min, z_max, c0, l0, c1, l1, coord_x, coord_y);

	// Construction du polygone
	for (std::vector<double>::const_iterator itx = coord_x.begin(), ity = coord_y.begin(); itx != coord_x.end(); ++itx, ++ity)
		all_points.push_back( Point(*itx,*ity) );

	// Calcul de l'enveloppe convexe
	CGAL::convex_hull_2( all_points.vertices_begin() , all_points.vertices_end() , back_insert_iterator<Polygon_2 >(footprint) );
}

void ModeleProjection::FootprintLocal(double z_min, double z_max, footprint_type &footprint)
{
	FootprintLocal(z_min, z_max, 0., 0., m_width, m_height, footprint);
}
#endif // ORIENTATIONMATIS_USE_CGAL

void ModeleProjection::FootprintWorld(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, double &xmin, double &xmax, double &ymin, double &ymax)
{
	vector<double> coord_x, coord_y;
	FootprintHelper(world_system_name, z_min, z_max, c0, l0, c1, l1, coord_x, coord_y);

	xmin = *min_element(coord_x.begin(), coord_x.end());
	xmax = *max_element(coord_x.begin(), coord_x.end());
	ymin = *min_element(coord_y.begin(), coord_y.end());
	ymax = *max_element(coord_y.begin(), coord_y.end());
}

void ModeleProjection::FootprintWorld(const char *world_system_name, double z_min, double z_max, double &xmin, double &xmax, double &ymin, double &ymax)
{
	FootprintWorld(world_system_name, z_min, z_max, 0., 0., m_width, m_height, xmin, xmax, ymin, ymax);
}

#if ORIENTATIONMATIS_USE_CGAL
void ModeleProjection::FootprintWorld(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, footprint_type &footprint)
{
	footprint_type all_points;

	vector<double> coord_x, coord_y;
	FootprintHelper(world_system_name, z_min, z_max, c0, l0, c1, l1, coord_x, coord_y);

	// Construction du polygone
	for (std::vector<double>::const_iterator itx = coord_x.begin(), ity = coord_y.begin(); itx != coord_x.end(); ++itx, ++ity)
		all_points.push_back( Point(*itx,*ity) );

	// Calcul de l'enveloppe convexe
	CGAL::convex_hull_2( all_points.vertices_begin() , all_points.vertices_end() , back_insert_iterator<Polygon_2 >(footprint) );
}

void ModeleProjection::FootprintWorld(const char *world_system_name, double z_min, double z_max, footprint_type &footprint)
{
	FootprintWorld(world_system_name, z_min, z_max, 0., 0., m_width, m_height, footprint);
}

#endif // ORIENTATIONMATIS_USE_CGAL

}
// namespace OrientationMATIS
