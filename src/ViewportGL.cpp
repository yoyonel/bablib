#include <OpenGL.h>
#include "ViewportGL.h"
BABLIB_NAMESPACE_USE

ViewportGL::ViewportGL() {
    getGL();
    }

void ViewportGL::getGL() {
    glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(this));
    }

void ViewportGL::setGL() const {
    glViewport(x, y, w, h);
    }

ViewportGL ViewportGL::subViewport(int wSub, int hSub) {
    int dx = (w - wSub) /2,
        dy = (h - hSub) /2;
    return ViewportGL(x + dx, y + dy, wSub, hSub);
    }

ViewportGL ViewportGL::subViewport(int margin, float ratio) {
    int wSub = w - 2 * margin,
        hSub = h - 2 * margin;
    if ((int)(ratio * hSub) > wSub)
        hSub = (int)(wSub / ratio);
    else
        wSub = (int)(hSub * ratio);
    return subViewport(wSub, hSub);
    }

ViewportGL ViewportGL::subViewport(int margin) {
    return ViewportGL(x + margin, y + margin, w - 2*margin, h - 2*margin);
    }

ViewportGL ViewportGL::subViewport(ViewportGL relativeView) const {
    return ViewportGL(x + relativeView.x, y + relativeView.y, relativeView.w, relativeView.h);
    }

ViewportGL ViewportGL::relativeViewport(ViewportGL view) const {
    return ViewportGL(view.x - x, view.y - y, view.w, view.h);
    }

void ViewportGL::setUnitSquare(ViewportGL subView) const {
    ViewportGL relativeView = subView.relativeViewport(*this);
    float xMin = relativeView.x / float(subView.w),
          yMin = relativeView.y / float(subView.h),
          xMax = (relativeView.x + relativeView.w) / float(subView.w),
          yMax = (relativeView.y + relativeView.h) / float(subView.h);
    glOrtho(xMin, xMax, yMin, yMax, -1, 1);
    }

void ViewportGL::pushScreenMatrices() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(x, x+w, y, y+h, -1, 1);
    }

void ViewportGL::popScreenMatrices() const {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    }

void ViewportGL::drawScreenQuad(bool filled, float z) const {
    pushScreenMatrices();
    push();
    setGL();
    glBegin(filled ? GL_QUADS : GL_LINE_LOOP);
        glTexCoord2f(0,0); glVertex3f(x,    y,    z);
        glTexCoord2f(1,0); glVertex3f(x+w-1,y,    z);
        glTexCoord2f(1,1); glVertex3f(x+w-1,y+h-1,z);
        glTexCoord2f(0,1); glVertex3f(x,    y+h-1,z);
    glEnd();
    pop();
    popScreenMatrices();
    }

void ViewportGL::push() { glPushAttrib(GL_VIEWPORT_BIT); }
void ViewportGL::pop()  { glPopAttrib(); }

/*********************************************************************************************/

void ViewportGL::pushFrame2D(float xmin, float xmax, float ymin, float ymax) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    ViewportGL view;    // le viewport courant
    const float
        dx = xmax - xmin,
        dy = ymax - ymin;    
    if (view.w * dy > view.h * dx) {
        const float d = (float(view.w) * dy / float(view.h) - dx) / 2;
        xmin -= d;
        xmax += d;
        }
    else {
        const float d = (float(view.h) * dx / float(view.w) - dy) / 2;
        ymin -= d;
        ymax += d;
        }
    glOrtho(xmin, xmax, ymin, ymax, -1, 1);
    }

void ViewportGL::popFrame2D() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    }

/*********************************************************************************************/

QDomElement ViewportGL::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x", QString::number(x));
    res.setAttribute("y", QString::number(y));
    res.setAttribute("w", QString::number(w));
    res.setAttribute("h", QString::number(h));
    return res;
    }

void ViewportGL::initFromDOMElement(const QDomElement &element) {
    x = element.attribute("x", "0").toInt();
    y = element.attribute("y", "0").toInt();
    w = element.attribute("w", "0").toInt();
    h = element.attribute("h", "0").toInt();
    }


