#ifndef __UNIT_SQUARE_CAMERA__
#define __UNIT_SQUARE_CAMERA__

#include <QGLViewer/camera.h> 
#include <QGLViewer/constraint.h> 

BABLIB_NAMESPACE_BEGIN

// Cette classe repr�sente une cam�ra qui affiche le carr� [0,1]� � l'�cran
// sur le rectangle ayant un rapport largeur/hauteur = <aspectRatio> et plus
// grand possible en laissant un marge de <borderPixels> aux bords de l'�cran.
// La cam�ra ne peut �tre ni translat�e ni tourn�e.
class UnitSquareCamera : public qglviewer::Camera {
    public:
        UnitSquareCamera(int borderPixels = 20, float aspectRatio = 1);     // si aspectRatio <= 0, le ratio n'est pas fix�
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

