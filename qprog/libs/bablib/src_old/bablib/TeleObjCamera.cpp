#include "TeleObjCamera.h"
#include "ViewportGL.h"
#include "Message.h"
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QGLViewer/qglviewer.h>
#include <QWheelEvent>
using namespace qglviewer;
BABLIB_NAMESPACE_USE

namespace {
    class ManipZoomCameraFrame : public ManipulatedCameraFrame {
        public:
            ManipZoomCameraFrame(float vFOVref) : vFOVref(vFOVref), zoom(1.0f) {}
            
        protected:
            virtual void wheelEvent(QWheelEvent *const event, Camera *const camera) {
                if (action_ == QGLViewer::ZOOM) {
                    const float wheelSensitivityCoef = 1.2f;
                    zoom *= powf(wheelSensitivity() * wheelSensitivityCoef, event->delta() / 120.0f);
                    //ManipulatedCameraFrame::wheelEvent(event, camera);
                    camera->setFieldOfView(zoom * vFOVref);
                    Q_EMIT manipulated();
                    action_ = QGLViewer::NO_MOUSE_ACTION;
                    }
                else
                    ManipulatedCameraFrame::wheelEvent(event, camera);
                }
            
            virtual void mouseMoveEvent(QMouseEvent* const event, Camera *const camera) {
                if (action_ == QGLViewer::ZOOM) {
                    const float c = -15.0f * (event->y() - prevPos_.y()) / camera->screenHeight();
                    zoom *= powf(1.2f, c);
                    camera->setFieldOfView(zoom * vFOVref);
                    
                    prevPos_ = event->pos();
                    Q_EMIT manipulated();
                    }
                else if (action_ == QGLViewer::LOOK_AROUND) {
                    Quaternion rot = pitchYawQuaternion(event->x(), event->y(), camera);
                    rotate(rot.inverse());
                    
                    prevPos_ = event->pos();
                    Q_EMIT manipulated();
                    }
                else if (action_ == QGLViewer::ROTATE) {
                    Vec trans = camera->projectedCoordinatesOf(revolveAroundPoint());
                    Quaternion rot = deformedBallQuaternion(event->x(), event->y(), trans[0], trans[1], camera);
                    //#CONNECTION# These two methods should go together (spinning detection and activation)
                    computeMouseSpeed(event);
                    setSpinningQuaternion(rot.inverse());
                    spin();

                    prevPos_ = event->pos();
                    Q_EMIT manipulated();
                    }
            
                else
                    ManipulatedCameraFrame::mouseMoveEvent(event, camera);
                }
    
        private:
            //@ can be improved so that the camera tracks exactly the clicked point (i.e direction)
            Quaternion pitchYawQuaternion(int x, int y, const Camera* const camera) {
                //const Quaternion rotX(Vec(1.0, 0.0, 0.0),           rotationSensitivity()*(prevPos_.y()-y)/camera->screenHeight());
                //const Quaternion rotY(transformOf(flyUpVector()),   rotationSensitivity()*(prevPos_.x()-x)/camera->screenWidth());
                const float dx = (prevPos_.x()-x) / float(camera->screenWidth());
                const float dy = (prevPos_.y()-y) / float(camera->screenHeight());
                const Quaternion rotX(Vec(1.0, 0.0, 0.0),           2*atanf(dy * tanf(0.5*camera->fieldOfView())));
                const Quaternion rotY(transformOf(flyUpVector()),   2*atanf(dx * tanf(0.5*camera->horizontalFieldOfView())));
                return rotY * rotX;
                }            
        private:
            const float vFOVref;
            float zoom;
        };
    }

TeleObjCamera::TeleObjCamera(Vec3 pos, float vFOV, bool fixed) : constraint(NULL) {
    ManipulatedCameraFrame *oldFrame = frame();
    setFrame(new ManipZoomCameraFrame(vFOV));
    delete oldFrame;
        
    setType(Camera::PERSPECTIVE);
    setPosition(Vec(pos));
    setRevolveAroundPoint(Vec(pos));
    setFieldOfView(vFOV);
    
    if (fixed) {
        constraint = new LocalConstraint();
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(1,0,0));
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,1,0));
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,0,1));
        frame()->setConstraint(constraint);
        }
    }

TeleObjCamera::~TeleObjCamera() {
    if (constraint != NULL) {
        frame()->setConstraint(NULL);
        delete constraint;
        }
    }

/*********************************************************************************************/

/*
QDomElement TeleObjCamera::domElement(const QString &name, QDomDocument &doc) const {
    QDomElement res = doc.createElement(name);
    res.setAttribute("zoom",      QString::number(zoom));
    res.appendChild(position().domElement("position", doc));
    return res;
    }

void TeleObjCamera::initFromDOMElement(const QDomElement &element) {
    zoom      = element.attribute("zoom", "0.0").toFloat();
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        if (e.tagName() == "position") {
            qglviewer::Vec pos;
            pos.initFromDOMElement(e);
            setPosition(pos);
            }
        e = e.nextSibling().toElement();
        }
    }
*/

