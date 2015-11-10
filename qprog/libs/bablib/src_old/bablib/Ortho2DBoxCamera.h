#ifndef __ORTHO_2D_BOX_CAMERA__
#define __ORTHO_2D_BOX_CAMERA__

#include "Vec2.h"
#include <QGLViewer/camera.h> 
#include <QGLViewer/constraint.h> 
#include <QDomDocument>
#include <QDomElement>

BABLIB_NAMESPACE_BEGIN

// camera orthogonal to the 2D XY plane
// showing the [<bmin>, <bmax>] rectangular area within a margin of <borderPix>
// if <fixed> is true, the view cannot be changed
class Ortho2DBoxCamera : public qglviewer::Camera {
    public:
        Ortho2DBoxCamera(Vec2 bmin, Vec2 bmax, int borderPix = 20, bool fixed = true);
        ~Ortho2DBoxCamera();
        
        void setView(Vec2 bmin, Vec2 bmax);

        virtual float 	zNear() const { return -1; }
        virtual float 	zFar()  const { return  1; }
        virtual void getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const;
        
        virtual void initFromDOMElement(const QDomElement &element);
        virtual QDomElement domElement(const QString &name, QDomDocument &document) const;
        
    private:
        Vec2 bmin, bmax;
        int borderPix;
        float zoom;
        qglviewer::AxisPlaneConstraint *constraint;
    };

BABLIB_NAMESPACE_END

#endif

