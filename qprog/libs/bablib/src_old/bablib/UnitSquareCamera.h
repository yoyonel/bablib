#ifndef __UNIT_SQUARE_CAMERA__
#define __UNIT_SQUARE_CAMERA__

#include <QGLViewer/camera.h> 
#include <QGLViewer/constraint.h> 

BABLIB_NAMESPACE_BEGIN

// Cette classe reprÃÂ©sente une camÃÂ©ra qui affiche le carrÃÂ© [0,1]ÃÂ² ÃÂ  l'ÃÂ©cran
// sur le rectangle ayant un rapport largeur/hauteur = <aspectRatio> et plus
// grand possible en laissant un marge de <borderPixels> aux bords de l'ÃÂ©cran.
// La camÃÂ©ra ne peut ÃÂªtre ni translatÃÂ©e ni tournÃÂ©e.
class UnitSquareCamera : public qglviewer::Camera {
    public:
        UnitSquareCamera(int borderPixels = 20, float aspectRatio = 1);     // si aspectRatio <= 0, le ratio n'est pas fixÃÂ©
        ~UnitSquareCamera();

        virtual float 	zNear() const { return -1; }
        virtual float 	zFar()  const { return  1; }
        virtual void getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const;
        
    private:
        const int borderPixels;
        const float aspectRatio;
        qglviewer::AxisPlaneConstraint *constraint;
    };

BABLIB_NAMESPACE_END

#endif

