#ifndef GEOMETRICTRANSFORMATION_HPP_
#define GEOMETRICTRANSFORMATION_HPP_
#include "modeleprojection.hpp"
#include "homographie2d.hpp"
#include "transfo2delementaire.hpp"
#include "projectivetransfo2D.hpp"


namespace OrientationMATIS
{
	/** crop in place of an image orientation with a scale factor
	 * 
	 * x0,y0 : coordinate of top left point of cropped image in original image system
	 * x1,y1 : coordinate of bottom right point of cropped image in original image system
	 * scale : scale factor of cropped image
	 * scale < 1.0 gives a smaller image
	 * scale > 1.0 gives a larger image 
	 */ 
	void Crop(shared_ptr<ModeleProjection> ori, 
				int x0, int y0,
				int x1, int y1, 
				double  scale = 1.);

	/** extraction of a sub-part of an image
	 * 
	 * x0,y0 : coordinate of top left included point of cropped image in original image system
	 * x1,y1 : coordinate of bottom right point not included of cropped image in original image system 
	 */ 
	void Extract(shared_ptr<ModeleProjection> ori, 
					int x0, int y0,
					int x1, int y1);
					
	/** rescaling of an image
	 * 
	 * scaleX : scale in the line dimension
	 * scaleY : scale in the column dimension
	 */
	void Rescale(shared_ptr<ModeleProjection> ori, 
					double scaleX,	double scaleY);
	
	/** Addition of a Projective transformation
	 */
	
	void AddProjectiveTransform(shared_ptr<ModeleProjection> ori,double * a1,double * a2, 
			double * a3, bool isintern = false);
	
	/** Flip orientation d'image in 4 directions **/
	//pos:  Initial=0      1		   2		...
	//  	 _____		 _____		 _____				
	//  	|  *  |		|     |		|     |
	//  	|  |  |		| --* |		|  |  |		...
	//  	|_____|		|_____|		|__*__|
	
//	void AddFlip(shared_ptr<ModeleProjection> ori, int direction ); 
	
	
	

}

#endif /*GEOMETRICTRANSFORMATION_HPP_*/