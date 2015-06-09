#include <OpenGL.h>
#include "UtilGL.h"
BABLIB_NAMESPACE_USE

void UtilGL::unitQuadVertices() {
    glVertex2f(0,0);
    glVertex2f(1,0);
    glVertex2f(1,1);
    glVertex2f(0,1);
    }
        
void UtilGL::drawUnitQuad() {
    glBegin(GL_QUADS);
        unitQuadVertices();
    glEnd();
    }

void UtilGL::drawAAQuad(Vec2 b0, Vec2 b1) {
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(b0.x,b0.y);
        glTexCoord2f(1,0); glVertex2f(b1.x,b0.y);
        glTexCoord2f(1,1); glVertex2f(b1.x,b1.y);
        glTexCoord2f(0,1); glVertex2f(b0.x,b1.y);
    glEnd();
    }

void UtilGL::drawAAQuadRaw(Vec2 b0, Vec2 b1) {
    glBegin(GL_QUADS);
        glVertex2f(b0.x,b0.y);
        glVertex2f(b1.x,b0.y);
        glVertex2f(b1.x,b1.y);
        glVertex2f(b0.x,b1.y);
    glEnd();
    }

void UtilGL::glvAAQuad(Vec2 b0, Vec2 b1) {
    glVertex2f(b0.x,b0.y);
    glVertex2f(b1.x,b0.y);
    glVertex2f(b1.x,b1.y);
    glVertex2f(b0.x,b1.y);
    }


/*
void UtilGL::vertices(Vec3 a, Vec3 b, Vec3 c) {
    a.glVertex();
    b.glVertex();
    c.glVertex();
    }
*/

void UtilGL::triangleFlat(Vec3 a, Vec3 b, Vec3 c) {
    triangleNormal(a, b, c).glNormal();
    glBegin(GL_TRIANGLES);
        a.glVertex();
        b.glVertex();
        c.glVertex();
    glEnd();
    }

void UtilGL::triangleFlatRaw(Vec3 a, Vec3 b, Vec3 c) {
    triangleNormal(a, b, c).glNormal();
    a.glVertex();
    b.glVertex();
    c.glVertex();
    }

void UtilGL::triangle(Vec3 a, Vec3 b, Vec3 c) {
    glBegin(GL_TRIANGLES);
        a.glVertex();
        b.glVertex();
        c.glVertex();
    glEnd();
    }

