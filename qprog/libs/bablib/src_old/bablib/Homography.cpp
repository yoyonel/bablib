#include "Homography.h"
#include <math.h>
BABLIB_NAMESPACE_USE

Homography::Homography(Matrix3 M) : M(M) {}
Homography::Homography(float d) : M(d) {}
Homography::Homography(const float M[9], bool deleteArray) : M(M) { if (deleteArray) delete M; }
Homography::Homography(const double M[9], bool deleteArray) : M(M) { if (deleteArray) delete M; }

Homography::Homography(
    float m11, float m12, float m13,
    float m21, float m22, float m23,
    float m31, float m32, float m33)
    : M(m11, m12, m13,
        m21, m22, m23,
        m31, m32, m33) {
    }

Homography::Homography(Vec3 O, Vec3 X, Vec3 Y, bool relative) {
    for (int i=0; i<3; i++) {
        M.coef(i,0) = X[i]/X.z - (relative ? 0 : O[i]/O.z);
        M.coef(i,1) = Y[i]/Y.z - (relative ? 0 : O[i]/O.z);
        M.coef(i,2) = O[i]/O.z;
        }
    }

Homography::Homography(Vec2 O, Vec2 X, Vec2 Y, bool relative) {
    for (int i=0; i<3; i++) {
        M.coef(i,0) = (i==2) ? 0 : (X[i] - (relative ? 0 : O[i]));
        M.coef(i,1) = (i==2) ? 0 : (Y[i] - (relative ? 0 : O[i]));
        M.coef(i,2) = (i==2) ? 1 : O[i];
        }
    }

Homography::Homography(float xmin, float xmax, float ymin, float ymax) {
    float dx = xmax - xmin,
          dy = ymax - ymin;
    *this = Homography::translationAndScaling( -xmin, -ymin, 1 / dx, 1 / dy);
    }

/*********************************************************************************************/

Homography Homography::operator*(Homography P) const { return Homography(M * P.M); }
Vec3 Homography::operator*(Vec3 v) const { return M * v; }
Vec2 Homography::operator*(Vec2 v) const { return M * v; }

Homography Homography::inv() const { return Homography(M.inv()); }
void Homography::transpose() { M.transpose(); }
Homography Homography::transposed() const { return Homography(M.transposed()); }

/*********************************************************************************************/

Vec3 Homography::column(int j) const { return M.column(j); }
void Homography::setColumn(int j, Vec3 c) { M.setColumn(j,c); }

/*********************************************************************************************/
/*
void Homography::getModelViewFrom(const qglviewer::Camera &camera) {
    GLdouble matrix[16];
    camera.getModelViewMatrix(matrix);
    for (int i=0; i<16; i++) M[i] = static_cast<float>(matrix[i]);
    }

void Homography::getProjectionFrom(const qglviewer::Camera &camera) {
    GLdouble matrix[16];
    camera.getProjectionMatrix(matrix);
    for (int i=0; i<16; i++) M[i] = static_cast<float>(matrix[i]);
    }
*/
/*********************************************************************************************/
/*
Homography Homography::getGLModelView() {
    Homography res;
    glGetFloatv(GL_MODELVIEW_MATRIX, res.M);
    return res;
    }

Homography Homography::getGLProjection() {
    Homography res;
    glGetFloatv(GL_PROJECTION_MATRIX, res.M);
    return res;
    }

Homography Homography::getGLTransform() { return getGLProjection() * getGLModelView(); }
*/
/*********************************************************************************************/
/*
void Homography::glLoadMatrix() const { glLoadMatrixf(M); }
void Homography::glLoadModelView() const { glMatrixMode(GL_MODELVIEW); glLoadMatrix(); }
void Homography::glLoadProjection() const { glMatrixMode(GL_PROJECTION); glLoadMatrix(); }

void Homography::glMultMatrix() const { glMultMatrixf(M); }
void Homography::glMultModelView() const { glMatrixMode(GL_MODELVIEW); glMultMatrix(); }
void Homography::glMultProjection() const { glMatrixMode(GL_PROJECTION); glMultMatrix(); }
*/
/*********************************************************************************************/
/*
float Homography::screenRatio(float xMin, float xMax, float yMin, float yMax, float z) const {
    Homography Tinv = this->inv();
    Vec4 O = Tinv * Vec4(xMin, yMin, z, 1),
         X = Tinv * Vec4(xMax, yMin, z, 1),
         Y = Tinv * Vec4(xMin, yMax, z, 1);
    //return Vec4::distance(O, X) / Vec4::distance(O, Y);
    return dist(Vec3(O), Vec3(X)) / dist(Vec3(O), Vec3(Y));
    }
*/
/*********************************************************************************************/
/*
void Homography::drawBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(M);
    
    // face bas :
    glBegin(GL_LINE_LOOP);
        glVertex3f(xMin, yMin, zMin);
        glVertex3f(xMax, yMin, zMin);
        glVertex3f(xMax, yMax, zMin);
        glVertex3f(xMin, yMax, zMin);
    glEnd();
    
    // face haut :
    glBegin(GL_LINE_LOOP);
        glVertex3f(xMin, yMin, zMax);
        glVertex3f(xMax, yMin, zMax);
        glVertex3f(xMax, yMax, zMax);
        glVertex3f(xMin, yMax, zMax);
    glEnd();
    
    // segments verticaux :
    glBegin(GL_LINES);
        glVertex3f(xMin, yMin, zMin); glVertex3f(xMin, yMin, zMax);
        glVertex3f(xMax, yMin, zMin); glVertex3f(xMax, yMin, zMax);
        glVertex3f(xMax, yMax, zMin); glVertex3f(xMax, yMax, zMax);
        glVertex3f(xMin, yMax, zMin); glVertex3f(xMin, yMax, zMax);
    glEnd();
    
    glPopMatrix();
    }
*/
/*********************************************************************************************/
// lecture / ÃÂ©criture d'un ÃÂ©lÃÂ©ment XML :

Homography::Homography(const QDomElement &element) {
    initFromDOMElement(element);
    }

QDomElement Homography::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    for (int j=0; j<3; j++)
        res.appendChild(column(j).domElement("C" + QString::number(j), document));
    return res;
    }

void Homography::initFromDOMElement(const QDomElement &element) {
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        setColumn(e.tagName().right(1).toInt(), Vec3(e));
        e = e.nextSibling().toElement();
        }
    }

/*********************************************************************************************/

// versions uniformes :
Homography Homography::scaling(float s)                           { return scaling(s,s); }
Homography Homography::scaling(Vec2 s)                            { return scaling(s.x,s.y); }
Homography Homography::translation(float t)                       { return translation(t,t); }
Homography Homography::translation(Vec2 t)                        { return translation(t.x,t.y); }
Homography Homography::translationAndScaling(float t, float s)    { return translationAndScaling(t,t, s,s); }
Homography Homography::translationAndScaling(Vec2  t, Vec2  s)    { return translationAndScaling(t.x,t.y, s.x,s.y); }
Homography Homography::scalingAndTranslation(float s, float t)    { return scalingAndTranslation(s,s, t,t); }
Homography Homography::scalingAndTranslation(Vec2  s, Vec2  t)    { return scalingAndTranslation(s.x,s.y, t.x,t.y); }


Homography Homography::scaling(float sx, float sy) {
    return Homography(
        sx,  0, 0,
         0, sy, 0,
         0,  0, 1);
    }

Homography Homography::translation(float tx, float ty) {
    return Homography(
        1, 0, tx,
        0, 1, ty,
        0, 0,  1);
    }

Homography Homography::translationAndScaling(float tx, float ty, float sx, float sy) {
    return Homography(
        sx,  0,  sx*tx,
         0, sy,  sy*ty,
         0,  0,      1);
    }

Homography Homography::scalingAndTranslation(float sx, float sy, float tx, float ty) {
    return Homography(
        sx,  0,  tx,
         0, sy,  ty,
         0,  0,   1);
    }

/*
Homography Homography::rotation(float theta, Vec3 n, bool isNormed) {
    if (!isNormed) n.normalize();
    const float c = cosf(theta),
                s = sinf(theta),
                d = 1 - c;
    return Homography(
        c + d*n.x*n.x     , d*n.x*n.y - s*n.z , d*n.x*n.z + s*n.y ,  0,
        d*n.y*n.x + s*n.z , c + d*n.y*n.y     , d*n.y*n.z - s*n.x ,  0,
        d*n.z*n.x - s*n.y , d*n.z*n.y + s*n.x , c + d*n.z*n.z     ,  0,
        0                 , 0                 , 0                 ,  1
        );
    }

Homography Homography::rotation(Vec3 src, Vec3 dst, bool normalized) {
    if (!normalized) {
        src.normalize();
        dst.normalize();
        }
    Vec3 n = vec(src, dst);
    const float s = n.norm2(),
                c = sqrtf(1 - s),
                d = (s > 1e-8f) ? (1 - c) / s : 0.5f;  // dÃÂ©veloppement limitÃÂ© ÃÂ  l'ordre 1 par rapport ÃÂ  s (en zÃÂ©ro)
    return Homography(
        d*n.x*n.x + c   , d*n.x*n.y - n.z , d*n.x*n.z + n.y ,  0,
        d*n.y*n.x + n.z , d*n.y*n.y + c   , d*n.y*n.z - n.x ,  0,
        d*n.z*n.x - n.y , d*n.z*n.y + n.x , d*n.z*n.z + c   ,  0,
        0               , 0               , 0               ,  1
        );
    }

Homography Homography::rotation(Vec3 d) {
    const float theta = d.norm();
    return rotation(theta, d / theta, true);
    }
*/

Homography Homography::U2C() {
    return Homography(
        2, 0, -1,
        0, 2, -1,
        0, 0,  1
        );
    }

Homography Homography::C2U() {
    return Homography(
        0.5, 0  , 0.5,
        0  , 0.5, 0.5,
        0  , 0  , 1
        );
    }

