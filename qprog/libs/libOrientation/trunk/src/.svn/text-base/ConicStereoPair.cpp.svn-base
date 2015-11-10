#include "ConicStereoPair.hpp"
using namespace OrientationMATIS;
ConicStereoPair::ConicStereoPair()
{
	ModeleProjection::InitAllIO();
}

ConicStereoPair::ConicStereoPair(std::string nomleft, std::string nomright)
{
	ModeleProjection::InitAllIO();
	try{
		int LeftOk = ReadLeftOrientation(nomleft);
		int RightOk = ReadRightOrientation(nomright);
	}
	catch(const std :: exception & e)
		{throw e;}
	
}

int ConicStereoPair::ReadLeftOrientation(std::string nomleft)
{
	m_LeftOrientation = dynamic_pointer_cast<OrientationType> 
		(ModeleProjection::ReadFile(nomleft));
	if (!m_LeftOrientation) 
		{
			throw std::logic_error("Error in (ConicStereoPair::ReadLeftOrientation)\n") ;	
			return 0;
		}
	return 1;
}

int ConicStereoPair::ReadRightOrientation(std::string nomright)
{
	m_RightOrientation = dynamic_pointer_cast<OrientationType> 
		(ModeleProjection::ReadFile(nomright));
	if (!m_RightOrientation) 
		{
			throw std::logic_error("Error in (ConicStereoPair::ReadRightOrientation)\n") ;	
			return 0;
		}
	return 1;
}

int ConicStereoPair::SetLeftOrientation(OrientationPointerType ori)
{
	m_LeftOrientation = dynamic_pointer_cast<OrientationType> 
			(ori->Clone());
	if (!m_LeftOrientation) 
		{
			throw std::logic_error("Error in (ConicStereoPair::SetLeftOrientation)\n") ; 	
			return 0;
		}
		return 1;
	
}

int ConicStereoPair::SetRightOrientation(OrientationPointerType ori)
{
	m_RightOrientation = dynamic_pointer_cast<OrientationType> 
				(ori->Clone());
	if (!m_RightOrientation) 
	{
		throw std::logic_error("Error in (ConicStereoPair::SetRightOrientation)\n") ; 
		return 0;
	}
	return 1;
}
int ConicStereoPair::SetOrientations(OrientationPointerType orig, OrientationPointerType orid)
{
	
	try{
		int LeftOk = SetLeftOrientation(orig);
		int RightOk = SetRightOrientation(orid);
	}
	catch(const std :: exception & e)
	{
		throw e;
		return 0;
	}
	return 1;
}

ConicStereoPair::OrientationPointerType ConicStereoPair::GetLeftOrientation()
{
	return (dynamic_pointer_cast<OrientationType> (m_LeftOrientation->Clone()));
}

ConicStereoPair::OrientationPointerType ConicStereoPair::GetRightOrientation()
{
	return (dynamic_pointer_cast<OrientationType> (m_RightOrientation->Clone()));
}

double ConicStereoPair::ImageToLocal(double cg, double lg, double cd, double ld,
									 double &X, double &Y, double &Z)
{
	double XYZ0[3];
	double XYZ1[3];
	double UVW0[3];
	double UVW1[3];
	double S0S1[3];
	
	m_LeftOrientation->ImageToLocalVec(cg,lg, XYZ0[0], XYZ0[1], XYZ0[2], UVW0[0], UVW0[1], UVW0[2]);
	m_RightOrientation->ImageToLocalVec(cd,ld, XYZ1[0], XYZ1[1], XYZ1[2], UVW1[0], UVW1[1], UVW1[2]);

	for ( long ii = 0 ; ii < 3 ; ii++ ) 
	{ 
		
		S0S1[ii] = XYZ1[ii] - XYZ0[ii] ; 
		UVW0[ii] -= XYZ0[ii];
		UVW1[ii] -= XYZ1[ii];
	} 
//	std::cout<<"base : "<<S0S1[0]<<", "<<S0S1[1]<<", "<<S0S1[2]<<std::endl;
	 
	double aa = std::inner_product(UVW0, UVW0+3, UVW0,0.0);
	double bb = -1 * std::inner_product(UVW0, UVW0+3, UVW1,0.0);
	double cc = std::inner_product(UVW1, UVW1+3, UVW1, 0.0);
	double dd = std::inner_product(UVW0, UVW0 +3, S0S1, 0.0);
	double ee = -1 * std::inner_product(UVW1, UVW1+3, S0S1,0.0);
	double det = (cc*aa) - (bb*bb);
//	std::cout<<"det : "<<det<<std::endl;
	
	// On ne peut pas intersecter deux rayons paralleles
	if ( det <= 0 ) 
	{ 
		return -1.0 ; 
	}
	double lambda1 = ( (cc * dd) - (bb * ee) ) / det;

	double lambda2 = ( (aa * ee) - (bb * dd) ) / det;

	// coordonnes en metres dans le repere terrestre
	UVW0[0] = XYZ0[0] + lambda1 * UVW0[0];
	UVW0[1] = XYZ0[1] + lambda1 * UVW0[1];
	UVW0[2] = XYZ0[2] + lambda1 * UVW0[2];

	UVW1[0] = XYZ1[0] + lambda2 * UVW1[0];
	UVW1[1] = XYZ1[1] + lambda2 * UVW1[1];
	UVW1[2] = XYZ1[2] + lambda2 * UVW1[2];

	X = (UVW0[0] + UVW1[0] ) / 2.0;
	Y = (UVW0[1] + UVW1[1] ) / 2.0;
	Z = (UVW0[2] + UVW1[2] ) / 2.0;

	UVW1[0] -= UVW0[0];
	UVW1[1] -= UVW0[1];
	UVW1[2] -= UVW0[2];
	double residual = sqrt(UVW1[0]*UVW1[0] + UVW1[1]*UVW1[1] + UVW1[2]*UVW1[2]); 
	if (lambda2 >0 && lambda1 > 0)	return (residual);
	else return (-residual);

}
void ConicStereoPair::Write(std::string nomleft, std::string nomright)
{
	ModeleProjection::WriteFile(m_LeftOrientation,nomleft);
	ModeleProjection::WriteFile(m_RightOrientation,nomright);
}

void ConicStereoPair::LocalToImage(double X, double Y, double Z, 
		double &cg, double &lg, double &cd, double &ld )
{
	m_LeftOrientation->LocalToImage(X,Y,Z,cg,lg);
	m_RightOrientation->LocalToImage(X,Y,Z,cd,ld);
}
