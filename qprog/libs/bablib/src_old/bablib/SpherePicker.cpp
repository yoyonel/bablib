#include "SpherePicker.h"
#include <math.h>

SpherePicker::SpherePicker(Vec3 center, float radius) :
    center(center),
    radius(radius),
    point(0,0,1),
    active(false) {
    //visu.loadDir("sphere_picker");  //@
    //visu.load(ProgGLSL::globalDir.sourcesDir("sphere_picker")); //@
    visu.loadDir("[bablib]/sphere_picker"); //@
    visu.activate();
        visu.setUniformVec3("center", center, false);
        visu.setUniform("radius", radius, false);
    visu.deactivate();
    }

SpherePicker::~SpherePicker() {
    visu.destroy();
    }

void SpherePicker::pick(Vec3 origRay, Vec3 dirRay) {
    Vec3 p = (origRay - center) / radius;
    Vec3 d = dirRay.normalized();    //@ / radius
    const float B = dot(p,d);
    const float C = dot(p,p) - 1.0f;
    const float delta = B*B - C;
    if (delta >= 0) {
        const float t = -B - sqrtf(delta);
        point = p + t*d;
        }
    else {
        const float t = -B;
        point = (p + t*d).normalized();
        }
    }

void SpherePicker::pick(Vec3d origRay, Vec3d dirRay) {
    Vec3d p = (origRay - center) / radius;
    Vec3d d = dirRay.normalized();    //@ / radius
    const double B = dot(p,d);
    const double C = dot(p,p) - 1.0f;
    const double delta = B*B - C;
    if (delta >= 0) {
        const double t = -B - sqrt(delta);
        point = (p + t*d).toVec3f();
        }
    else {
        const double t = -B;
        point = (p + t*d).normalized().toVec3f();
        }
    }

void SpherePicker::render() {
    if (!active) return;
    
    glDepthMask(GL_FALSE);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
    visu.activate();
    visu.setUniformVec3("point", point, false);
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(1,0);
        glVertex2f(1,1);
        glVertex2f(0,1);
    glEnd();
    visu.deactivate();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    }

