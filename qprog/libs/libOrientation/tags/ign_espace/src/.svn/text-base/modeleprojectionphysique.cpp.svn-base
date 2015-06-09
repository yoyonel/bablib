#include <iostream>
#include <cmath>

#include "modeleprojectionphysique.hpp"

namespace OrientationMATIS
{
	void ModeleProjectionPhysique::ImageAndZToLocal(double c, double l, double z, double &x, double &y) const
	{
		double x0, y0, z0, x1, y1, z1;
		ImageToLocalVec(c, l, x0, y0, z0, x1, y1, z1);
		double lambda = (z - z0) / (z1 - z0);
		x = x0 + (x1 - x0) * lambda;
		y = y0 + (y1 - y0) * lambda;
	}

	void ModeleProjectionPhysique::LocalToImage(double xt, double yt, double zt, double &c, double &l) const
	{
		double xi, yi, zi;
		m_extr.LocalToImage(xt, yt, zt, xi, yi, zi);
		IntrLocalToImage(xi, yi, zi, c, l);
		m_transfo2D.ApplyLocalToImage(c, l);
	}

	void ModeleProjectionPhysique::ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
	{
		m_transfo2D.ApplyImageToLocal(c,l);
		IntrImageToVec(c, l, x0, y0, z0, x1, y1, z1);
		m_extr.ImageToLocal(x0, y0, z0, x0, y0, z0);
		m_extr.ImageToLocal(x1, y1, z1, x1, y1, z1);
	}

	ModelePhysiqueExtrinseque::ModelePhysiqueExtrinseque(const ModelePhysiqueExtrinseque &mpe)
	{
		(*this) = mpe;
	}

	ModelePhysiqueExtrinseque::ModelePhysiqueExtrinseque(const double sommet[3], const double rotation[3][3])
	{
		for (unsigned int i = 0; i < 3; ++i)
		{
			m_sommet[i] = sommet[i];
			for (unsigned int j = 0; j < 3; ++j)
			{
				m_rotation[i][j] = rotation[i][j];
			}
		}
	}

    ModelePhysiqueExtrinseque & ModelePhysiqueExtrinseque::operator=(const ModelePhysiqueExtrinseque &mpe)
    {
		for (unsigned int i = 0; i < 3; ++i)
		{
			m_sommet[i] = mpe.m_sommet[i];
			for (unsigned int j = 0; j < 3; ++j)
			{
				m_rotation[i][j] = mpe.m_rotation[i][j];
			}
		}
        return (*this);
    }

	ModelePhysiqueExtrinseque::~ModelePhysiqueExtrinseque()
	{
		;
	}

	void ModelePhysiqueExtrinseque::LocalToImage(double xt, double yt, double zt, double &xi, double &yi, double &zi) const
	{
		double temp[3];
		temp[0] = xt - m_sommet[0];
		temp[1] = yt - m_sommet[1];
		temp[2] = zt - m_sommet[2];
		xi = m_rotation[0][0] * temp[0] + m_rotation[0][1] * temp[1] + m_rotation[0][2] * temp[2];
		yi = m_rotation[1][0] * temp[0] + m_rotation[1][1] * temp[1] + m_rotation[1][2] * temp[2];
		zi = m_rotation[2][0] * temp[0] + m_rotation[2][1] * temp[1] + m_rotation[2][2] * temp[2];
	}

	double ModelePhysiqueExtrinseque::GetDistance(double xt, double yt, double zt) const
	{
		double dx = xt - m_sommet[0], dy = yt - m_sommet[1], dz = zt - m_sommet[2];
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	void ModelePhysiqueExtrinseque::ImageToLocal(double xi, double yi, double zi, double &xt, double &yt, double &zt) const
	{
		double temp[3];
		temp[0] = m_rotation[0][0] * xi + m_rotation[1][0] * yi + m_rotation[2][0] * zi;
		temp[1] = m_rotation[0][1] * xi + m_rotation[1][1] * yi + m_rotation[2][1] * zi;
		temp[2] = m_rotation[0][2] * xi + m_rotation[1][2] * yi + m_rotation[2][2] * zi;
		xt = temp[0] + m_sommet[0];
		yt = temp[1] + m_sommet[1];
		zt = temp[2] + m_sommet[2];
	}


	/*
	ModeleSpherique::ModeleSpherique(double cPPA, double lPPA, double width, double height, double lambdaMin,
			double lambdaMax, double phiMin, double phiMax)
	{
		m_cPPA = cPPA;
		m_lPPA = lPPA;
		m_width = width;
		m_height = height;
		m_lambdaMin = lambdaMin;
		m_lambdaMax = lambdaMax;
		m_phiMin = phiMin;
		m_phiMax = phiMax;
	}

	ModeleSpherique::ModeleSpherique(const SphericModel &mod)
	{
		m_cPPA = mod.m_cPPA;
		m_lPPA = mod.m_lPPA;
		m_width = mod.m_width;
		m_height = mod.m_height;
		m_lambdaMin = mod.m_lambdaMin;
		m_lambdaMax = mod.m_lambdaMax;
		m_phiMin = mod.m_phiMin;
		m_phiMax = mod.m_phiMax;
	}

	void ModeleSpherique::LocalToImage(double x, double y, double z, double &c, double &l) const
	{
		double norme = sqrt(x*x + y*y + z*z);

		double lambda = m_lambdaMax - atan2(y, x);
		double phi = m_phiMax - acos(z / norme);
		c = m_width * lambda / (m_lambdaMax - m_lambdaMin );
		l = m_lPPA - m_height * phi / (m_phiMax - m_phiMin );
	}

	void ModeleSpherique::ImageToVec(double c, double l, double &x0, double &y0, double &z0,
			double &x1, double &y1, double &z1) const
	{
		x0 = y0 = z0 = 0.;

		double lambda = m_lambdaMin + (m_lambdaMax - m_lambdaMin) * c / m_width;
		double phi = m_phiMax + (m_phiMin - m_phiMax) * l / m_height;
		x1 = cos(phi) * cos(-lambda);
		y1 = cos(phi) * sin(-lambda);
		z1 = sin(phi);
	}
	*/
};
