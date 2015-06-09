#ifndef __TELE_OBJ_CAMERA__
#define __TELE_OBJ_CAMERA__

#include "Vec3.h"
#include <QGLViewer/camera.h> 
#include <QGLViewer/constraint.h> 
#include <QDomDocument>
#include <QDomElement>

BABLIB_NAMESPACE_BEGIN

// if <fixed> is true, the camera cannot be translated
class TeleObjCamera : public qglviewer::Camera {
    public:
        TeleObjCamera(Vec3 pos = Vec3::ZERO, float vFOV = 0.78f, bool fixed = true);
        ~TeleObjCamera();
        
        //virtual void initFromDOMElement(const QDomElement &element);
        //virtual QDomElement domElement(const QString &name, QDomDocument &document) const;
        
    private:
        float varFOV;
        qglviewer::AxisPlaneConstraint *constraint;
    };

BABLIB_NAMESPACE_END

#endif

