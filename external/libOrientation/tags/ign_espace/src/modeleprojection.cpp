
#include <cmath>

#include "modeleprojection.hpp"

namespace OrientationMATIS 
{

bool ModeleProjection::InternalCheck(double precision) const
{
	const std::vector <Sample> &samples = GetAuxiliaryData().GetSamples();
	for (unsigned int i=0; i<samples.size(); ++i)
	{
		const Sample &s = samples[i];
		double c,l;
		if (s.TestType == Sample::TestProjWToI)
		{
			if (!WorldToImage(s.systeme_geodesique.c_str(), s.x,s.y,s.z,c,l))
			return false;
			double dc = s.c-c;
			double dl = s.l-l;
			if ((std::abs(dc)> precision) || (std::abs(dl)> precision))
			{
				std::cout<<"ModeleProjection::InternalCheck WorldToImage problem"<<std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout <<"x:"<<s.x<<";y:"<<s.y<<";z:"<< s.z<<";c:"<<s.c<<";l:"<<s.l<<";systeme :"<<s.systeme_geodesique<< std::endl;
				std::cout << "Calcule : c:"<< c <<";l:"<<l<< std::endl;
				std::cout << "Ecart:dc:"<<dc<<";dl:"<<dl<<std::endl;
				return false;
			}
		} // end TestProjWToI
		else if (s.TestType == Sample::TestTransfo2D)
		{
			/////////////////////////////////////
			// test Transfo2D::ApplyImageToLocal
			c = s.c;
			l = s.l;
			m_transfo2D.ApplyImageToLocal(c,l);
			double dc = s.x - c;
			double dl = s.y - l;
//			std::cout<<"ApplyImageToLocal : \n";
//			std::cout<<"c = "<<c<< " x = "<<s.x<<std::endl;
//			std::cout<<"l = "<<l<< " y = "<<s.y<<std::endl;
			if ((std::abs(dc)> precision) || (std::abs(dl)> precision))
			{
				std::cout<<"ModeleProjection::InternalCheck Transfo2D::ApplyImageToLocal  problem"<<std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout <<"ci:"<<s.c<<" li:"<<s.l<<"--> cg:"<<s.x<<" l:"<<s.y<< std::endl;
				std::cout << "Calcule : cg:"<< c <<" lg:"<<l<< std::endl;
				std::cout << "Ecart:dc:"<<dc<<" dl:"<<dl<<std::endl;
				return false;

			}
			/////////////////////////////////////
			// test Transfo2D::ApplyLocalToImage
			c = s.x;
			l = s.y;
			m_transfo2D.ApplyLocalToImage(c,l);
//			std::cout<<"ApplyLocalToImage : \n";
//			std::cout<<"c = "<<c<< " s.c = "<<s.c<<std::endl;
//			std::cout<<"l = "<<l<< " s.l = "<<s.l<<std::endl;
			
			dc = s.c - c;
			dl = s.l - l;
			if ((std::abs(dc)> precision) || (std::abs(dl)> precision))
			{
				std::cout<<"ModeleProjection::InternalCheck Transfo2D::ApplyLocalToImage"<<std::endl;
				std::cout << "Sample : " << std::endl;
				std::cout <<"ci:"<<s.c<<" li:"<<s.l<<"--> cg:"<<s.x<<" l:"<<s.y<< std::endl;
				std::cout << "Calcule : ci:"<< c <<" li:"<<l<< std::endl;
				std::cout << "Ecart:dc:"<<dc<<" dl:"<<dl<<std::endl;
				return false;
			} 
		}// end TestTransfo2D

	}
	return true;
}

bool ModeleProjection::WorldToLocal(const char *world_system_name,
const double xw, const double yw, const double zw,
double &xl, double &yl, double &zl) const
{
	return m_geodesie.WorldToLocal(world_system_name, xw,yw,zw,xl,yl,zl);
}

bool ModeleProjection::LocalToWorld( const double xl, const double yl, const double zl,
const char *world_system_name, double &xw, double &yw, double &zw) const
{
	return m_geodesie.LocalToWorld(xl,yl,zl, world_system_name, xw,yw,zw);
}

bool ModeleProjection::WorldToImage(const char *world_system, const double xw, const double yw, const double zw, double &c, double &l) const
{
	double xl, yl, zl;
	bool res = WorldToLocal(world_system, xw, yw, zw, xl, yl, zl);
	if (res)
	LocalToImage(xl, yl, zl, c,l);
	return res;
}
};
