//#include <OpenGL.h>
#include "ShadedLineRenderer.h"
#include <Message.h>
BABLIB_NAMESPACE_USE

ShadedLineRenderer::ShadedLineRenderer() {
    prog.loadDir("[bablib]/shaded_lines");
    }

ShadedLineRenderer::~ShadedLineRenderer() {
    prog.destroy();
    }

void ShadedLineRenderer::begin() {
    prog.activate();
    glBegin(GL_LINES);
    }

void ShadedLineRenderer::draw(const Vec3 &a, const Vec3 &b) {
    draw(Vec3d(a), Vec3d(b));
    }

void ShadedLineRenderer::draw(const Vec3d &a, const Vec3d &b) {
    (b-a).normalized().glNormal();
    a.glVertex();
    b.glVertex();
    }

void ShadedLineRenderer::draw(const Vec4d &a, const Vec4d &b) {
    /*
    Vec3d d = (a.w * b.xyz() - b.w * a.xyz());
    double n = d.norm();
    if (n == 0.0) return;   // two points at infinity
    d /= n;
    if ((a.w < 0.0) != (b.w < 0.0)) d = -d;
    //if (b.w < 0.0) d = -d;
    */
    Vec3d d = dir(a,b).xyz();
    double n = d.norm();
    if (n == 0.0) return;   // two points at infinity
    d /= n;
    
    d.glNormal();
    a.glVertex();
    b.glVertex();
    
    //draw(a.toVec3d(), b.toVec3d());
    }

void ShadedLineRenderer::end() {
    glEnd();
    prog.deactivate();
    }

