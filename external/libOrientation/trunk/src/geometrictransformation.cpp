#include "geometrictransformation.hpp"

#include <cmath>
#include <stdexcept>
#include <sstream>

namespace OrientationMATIS
{


void AddProjectiveTransform(shared_ptr<ModeleProjection> ori, double * a1,double * a2, double * a3, 
		bool isintern )
{
	shared_ptr<ProjectiveTransfo2D> tr =
									shared_ptr<ProjectiveTransfo2D>(new ProjectiveTransfo2D(isintern));
	tr->SetParameters(a1,a2,a3);
	ori->AddTransformation2DElementaire (tr);
	
} 

/** Flip orientation d'image in 4 directions **/
//pos:  Initial=0      1		   2		...
//  	 _____		 _____		 _____				
//  	|  *  |		|     |		|     |
//  	|  |  |		| --* |		|  |  |		...
//  	|_____|		|_____|		|__*__|

//void AddFlip(shared_ptr<ModeleProjection> ori, int direction )
//{
//	shared_ptr<Homographie2D> tr = shared_ptr<Homographie2D>(new Homographie2D(false));
//	unsigned int nc,nl;
//	ori->GetTailleImage(nc, nl);
//	switch(direction)
//	{
//	case 0:
//	{
//		// Nothing todo, just for informing that 
//		// the necessairy operation is performed 
//		// we put a unity transformation.
//		tr->SetC(1,0,0);
//		tr->SetL(0,1,0);
//		break;
//	}
//
//	case 1:
//	{
//		tr->SetC(0,1,0);
//		tr->SetL(-1,0,nc-1);
//		break;
//	}
//	case 2 :
//	{
//		
//	}
//	
//		
//	}
//}
	


void Crop(shared_ptr<ModeleProjection> ori, 
								int x0, int y0,
								int x1, int y1,
								double scale)  
	{
		if (x1 < x0 || y1 < y0 || scale <= 0.) 
		{
			std :: ostringstream erreur;
			erreur << "File : " <<__FILE__ << std::endl;
			erreur << "Function : " << __FUNCTION__ << std::endl;
			erreur << "Line : " << __LINE__ << std::endl;
			erreur << "Footprint or scale not correct" << std::endl;
			throw std::logic_error( erreur.str() );
		}

		shared_ptr<Homographie2D> tr = shared_ptr<Homographie2D>(new Homographie2D(false));
		std::cout<<"unsigned int : -x0 : "<<-x0<<std::endl;
		tr->SetC(scale,0,-x0 );
		tr->SetL(0,scale,-y0 );
		ori->AddTransformation2DElementaire(tr);
		unsigned int nc,nl;
		nc = std::floor( scale * (x1 - x0) );
		nl = std::floor( scale * (y1 - y0) );
		ori->SetTailleImage(nc,nl);
	}

	/** extraction of a sub-part of an image
	 * 
	 * x0,y0 : coordinate of top left included point of cropped image in original image system
	 * x1,y1 : coordinate of bottom right point not included of cropped image in original image system 
	 */ 
	void Extract(shared_ptr<ModeleProjection> ori, 
					int x0, int y0,
					int x1, int y1)
	{
		if (x1 < x0 || y1 < y0) 
		{
			std :: ostringstream erreur;
			erreur << "File : " <<__FILE__ << std::endl;
			erreur << "Function : " << __FUNCTION__ << std::endl;
			erreur << "Line : " << __LINE__ << std::endl;
			erreur << "Footprint not correct" << std::endl;
			throw std::logic_error( erreur.str() );
		}
		shared_ptr<Homographie2D> tr = shared_ptr<Homographie2D>(new Homographie2D(false));
		tr->SetC(1.,0,-x0 );
		tr->SetL(0,1.,-y0 );
		ori->AddTransformation2DElementaire(tr);
		unsigned int nc,nl;
		nc = std::abs(x1 - x0);
		nl = std::abs(y1 - y0);
		ori->SetTailleImage(nc,nl);
	}
					
	/** rescaling of an image
	 * 
	 * scaleX : scale in the line dimension
	 * scaleY : scale in the column dimension
	 */
	void Rescale(shared_ptr<ModeleProjection> ori, 
					double scaleX,	double scaleY)
	{
		shared_ptr<Homographie2D> tr = shared_ptr<Homographie2D>(new Homographie2D(false));
		tr->SetC(scaleX,0.,0. );
		tr->SetL(0.,scaleY,0. );
		ori->AddTransformation2DElementaire(tr);
		unsigned int nc,nl;
		ori->GetTailleImage(nc, nl);
		nc = std::floor( scaleX * nc );
		nl = std::floor( scaleY * nl );
		ori->SetTailleImage(nc,nl);
	}
					
}
