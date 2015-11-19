#ifndef __UNIT_SQUARE_CAMERA__
#define __UNIT_SQUARE_CAMERA__

#include <QGLViewer/camera.h>
#include <QGLViewer/constraint.h>

BABLIB_NAMESPACE_BEGIN

// Cette classe reprï¿½sente une camï¿½ra qui affiche le carrï¿½ [0,1]ï¿½ ï¿½ l'ï¿½cran
// sur le rectangle ayant un rapport largeur/hauteur = <aspectRatio> et plus
// grand possible en laissant un marge de <borderPixels> aux bords de l'ï¿½cran.
// La camï¿½ra ne peut ï¿½tre ni translatï¿½e ni tournï¿½e.
class UnitSquareCamera : public qglviewer::Camera {
    public:
        UnitSquareCamera(int borderPixels = 20, float aspectRatio = 1);     // si aspectRatio <= 0, le ratio n'est pas fixï¿½
        ~UnitSquareCamera();

        virtual float 	zNear() const { return -1.; }
        virtual float 	zFar()  const { return  1.; }
        virtual void getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const;

    private:
        const int borderPixels;
        const float aspectRatio;
        qglviewer::AxisPlaneConstraint *constraint;
    };

BABLIB_NAMESPACE_END

#endif
