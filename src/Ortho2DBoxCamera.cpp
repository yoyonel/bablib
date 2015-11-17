#include "Ortho2DBoxCamera.h"
#include "ViewportGL.h"
#include "Message.h"
#include <QGLViewer/manipulatedCameraFrame.h>
#include <QGLViewer/qglviewer.h>
#include <QWheelEvent>
using namespace qglviewer;
BABLIB_NAMESPACE_USE

class ManipZoomCameraFrame : public ManipulatedCameraFrame {
    public:
        ManipZoomCameraFrame(float &zoom) : zoom(zoom) {}
        
    protected:
        virtual void wheelEvent(QWheelEvent *const event, Camera *const /*camera*/) {
            const float wheelSensitivityCoef = 1.2f;
            zoom *= powf(wheelSensitivity() * wheelSensitivityCoef, event->delta() / 120.0f);
            //ManipulatedCameraFrame::wheelEvent(event, camera);
            Q_EMIT manipulated();
            }
        
        virtual void mouseMoveEvent(QMouseEvent* const event, Camera *const camera) {
            if (action_ == QGLViewer::ZOOM) {
                const float c = -15.0f * (event->y() - prevPos_.y()) / camera->screenHeight();
                zoom *= powf(1.2f, c);
                prevPos_ = event->pos();
                Q_EMIT manipulated();
                }
            else
                ManipulatedCameraFrame::mouseMoveEvent(event, camera);
            }

        
    private:
        float &zoom;
    };

Ortho2DBoxCamera::Ortho2DBoxCamera(Vec2 bmin, Vec2 bmax, int borderPix, bool fixed) : bmin(bmin), bmax(bmax), borderPix(borderPix), zoom(1.0f), constraint(NULL) {
    if (!fixed) {
        ManipulatedCameraFrame *oldFrame = frame();
        setFrame(new ManipZoomCameraFrame(zoom));
        delete oldFrame;
        }
    setType(Camera::ORTHOGRAPHIC);
    Vec2 c = (bmin + bmax) / 2;
    setPosition(Vec(c.x, c.y, 0));
    
    constraint = new LocalConstraint();
    if (fixed) {
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(1,0,0));
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,1,0));
        constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,0,1));
        }
        
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(1,0,0));
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,1,0));
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,0,1));
    frame()->setConstraint(constraint);
    }

Ortho2DBoxCamera::~Ortho2DBoxCamera() {
    if (constraint != NULL) {
        frame()->setConstraint(NULL);
        delete constraint;
        }
    }

void Ortho2DBoxCamera::getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const {
    ViewportGL view(0, 0, screenWidth(), screenHeight());
    const int w = view.w - 2 * borderPix;
    const int h = view.h - 2 * borderPix;
    const double sx = double(view.w) / double(w);
    const double sy = double(view.h) / double(h);
    
    Vec2 d = bmax - bmin;
    if (w * d.y > h * d.x)  d.x = float(w) * d.y / float(h);
    else                    d.y = float(h) * d.x / float(w);
    halfWidth  = zoom * sx * d.x / 2;
    halfHeight = zoom * sy * d.y / 2;
    }

