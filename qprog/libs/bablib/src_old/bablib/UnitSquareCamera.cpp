#include "UnitSquareCamera.h"
#include "ViewportGL.h"
using namespace qglviewer;
BABLIB_NAMESPACE_USE

UnitSquareCamera::UnitSquareCamera(int borderPixels, float aspectRatio) : borderPixels(borderPixels), aspectRatio(aspectRatio) {
    setType(Camera::ORTHOGRAPHIC);
    setPosition(Vec(0.5, 0.5, 0));
    
    constraint = new LocalConstraint();
    constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(1,0,0));
    constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,1,0));
    constraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,0,1));
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(1,0,0));
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,1,0));
    constraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0,0,1));
    frame()->setConstraint(constraint);
    }

UnitSquareCamera::~UnitSquareCamera() {
    frame()->setConstraint(NULL);
    delete constraint;
    }

void UnitSquareCamera::getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const {
    ViewportGL
        view(0, 0, screenWidth(), screenHeight()),
        subView = aspectRatio > 0 ? view.subViewport(borderPixels, aspectRatio) : view.subViewport(borderPixels);

    halfWidth  = view.w / float(2 * subView.w),
    halfHeight = view.h / float(2 * subView.h);
    }

