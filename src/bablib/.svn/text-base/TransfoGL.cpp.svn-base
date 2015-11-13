#include "TransfoGL.h"
BABLIB_NAMESPACE_USE

TransfoGL::TransfoGL(bool getFromGL) {
    if (getFromGL) save();
    }

TransfoGL::TransfoGL(ProjTransfo modelView, ProjTransfo projection) : modelView(modelView), projection(projection) {}

TransfoGL::TransfoGL(const qglviewer::Camera &camera) {
    modelView.getModelViewFrom(camera);
    projection.getProjectionFrom(camera);
    }

void TransfoGL::load() const {
    modelView.glLoadModelView();
    projection.glLoadProjection();
    }

void TransfoGL::save() {
    modelView  = ProjTransfo::getGLModelView();
    projection = ProjTransfo::getGLProjection();
    }

void TransfoGL::pushAndLoad() const {
    push();
    load();
    }

void TransfoGL::saveAndPop() {
    save();
    pop();
    }

void TransfoGL::push() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    }

void TransfoGL::pop() {
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    }

void TransfoGL::pushAndInit() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    }

TransfoGL* TransfoGL::get() {
    ProjTransfo modelView  = ProjTransfo::getGLModelView();
    ProjTransfo projection = ProjTransfo::getGLProjection();
    return new TransfoGL(modelView, projection);
    }

ProjTransfo TransfoGL::transfo() const {
    return projection * modelView;
    }

Vec4 TransfoGL::viewPoint() const {
    return transfo().inv() * Vec4(0,0,-1,0);
    }

float TransfoGL::screenRatio() const {
    ProjTransfo T = transfo().inv();
    Vec3 O = T * Vec3(0,0,0);
    Vec3 X = T * Vec3(1,0,0);
    Vec3 Y = T * Vec3(0,1,0);
    return dist(O,Y) / dist(O,X);
    }

