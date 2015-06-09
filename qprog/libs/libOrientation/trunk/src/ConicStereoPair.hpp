#ifndef CONICSTEREOPAIR_HPP_
#define CONICSTEREOPAIR_HPP_

#include "noboost.hpp"
#include "modeleprojection.hpp"
#include "modeleprojectionconique.hpp"
#include "modeleprojectionphysique.hpp"
#include <numeric>

namespace OrientationMATIS
{
class ConicStereoPair
{
public :
	typedef ModeleProjectionConique OrientationType;
	typedef shared_ptr<OrientationType> OrientationPointerType;
	
	ConicStereoPair();
	ConicStereoPair(std::string nomleft, std::string nomright);
	int ReadLeftOrientation(std::string nomleft);
	int ReadRightOrientation(std::string nomleft);
	int SetLeftOrientation(OrientationPointerType ori);
	int SetRightOrientation(OrientationPointerType ori);
	int SetOrientations(OrientationPointerType orig, OrientationPointerType orid);
	
	// The returned value is the residual of intersection 
	// this value is >0 if intersection is in front of cameras and <0 if not
	double ImageToLocal(double cg, double lg, double cd, double ld, double &X, double &Y, double &Z);
	
	OrientationPointerType GetLeftOrientation();
	OrientationPointerType GetRightOrientation();
	void Write(std::string nomleft, std::string nomright);
	void LocalToImage(double X, double Y, double Z, double &cg, double &lg, double &cd, double &ld );
	
	
protected:
	OrientationPointerType m_LeftOrientation;
	OrientationPointerType m_RightOrientation;
};
}
#endif /*CONICSTEREOPAIR_HPP_*/
