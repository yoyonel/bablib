#include "Proj3d.h"
#include "Matrix3d.h"
#include <QGLViewer/camera.h>
#include <math.h>
BABLIB_NAMESPACE_USE

Proj3d::Proj3d(double d) {
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            coef(i,j) = (i == j ? d : 0);
    }

Proj3d::Proj3d(const double M[16], bool deleteArray) {
    for (int i=0; i<16; i++) this->M[i] = M[i];
    if (deleteArray) delete M;
    }

Proj3d::Proj3d(
        double m11, double m12, double m13, double m14,
        double m21, double m22, double m23, double m24,
        double m31, double m32, double m33, double m34,
        double m41, double m42, double m43, double m44
        ) {
    M[0] = m11; M[4] = m12; M[8]  = m13; M[12] = m14;
    M[1] = m21; M[5] = m22; M[9]  = m23; M[13] = m24;
    M[2] = m31; M[6] = m32; M[10] = m33; M[14] = m34;
    M[3] = m41; M[7] = m42; M[11] = m43; M[15] = m44;
    }

Proj3d::Proj3d(Vec4d O, Vec4d X, Vec4d Y, Vec4d Z, bool relative) {
    for (int i=0; i<4; i++) {
        coef(i,0) = X[i]/X.w - (relative ? 0 : O[i]/O.w);
        coef(i,1) = Y[i]/Y.w - (relative ? 0 : O[i]/O.w);
        coef(i,2) = Z[i]/Z.w - (relative ? 0 : O[i]/O.w);
        coef(i,3) = O[i]/O.w;
        }
    }

Proj3d::Proj3d(Vec3d O, Vec3d X, Vec3d Y, Vec3d Z, bool relative) {
    for (int i=0; i<4; i++) {
        coef(i,0) = (i==3) ? 0 : (X[i] - (relative ? 0 : O[i]));
        coef(i,1) = (i==3) ? 0 : (Y[i] - (relative ? 0 : O[i]));
        coef(i,2) = (i==3) ? 0 : (Z[i] - (relative ? 0 : O[i]));
        coef(i,3) = (i==3) ? 1 : O[i];
        }
    }

Proj3d::Proj3d(const Matrix3d &R, const Vec3d &T) {
    // affine matrix : [R T]
    //                 [0 1]
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) coef(i,j) = R.coef(i,j);
        coef(i,3) = T[i];
        }
    for (int j=0; j<3; j++) coef(3,j) = 0.0;
    coef(3,3) = 1.0;
    }

Proj3d::Proj3d(const ProjTransfo &proj) {
    for (int i=0; i<16; i++) M[i] = double(proj.M[i]);
    }

Proj3d::Proj3d(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
    double dx = xmax - xmin,
          dy = ymax - ymin,
          dz = zmax - zmin;
    *this = Proj3d::translationAndScaling( -xmin, -ymin, -zmin, 1 / dx, 1 / dy, 1 / dz);
    }

/*********************************************************************************************/

Proj3d Proj3d::operator*(Proj3d P) const {
    Proj3d res;
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j) {
            res(i,j) = 0;
            for (int k=0; k<4; k++)
                res(i,j) += coef(i,k) * P(k,j);
            }
    return res;
    }

/*
// version utilisant la GSL
Proj3d Proj3d::inv() const {
    gsl_matrix *A = gsl_matrix_alloc(4, 4);
    gsl_matrix *B = gsl_matrix_alloc(4, 4);
    gsl_permutation *p = gsl_permutation_alloc(4);
    int s;
    
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            *gsl_matrix_ptr(A, i, j) = coef(i,j);
    
    gsl_linalg_LU_decomp(A, p, &s);
    gsl_linalg_LU_invert(A, p, B);
    
    Proj3d res;
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            res(i,j) = gsl_matrix_get(B, i, j);
    
    gsl_matrix_free(A);
    gsl_matrix_free(B);
    gsl_permutation_free(p);

    return res;
    }
*/

// calcul de la transposée de la comatrice divisée par le déterminant :
Proj3d Proj3d::inv() const {
    Proj3d res;
    #define det2(i1, i2, j1, j2)  coef(i1,j1) * coef(i2,j2) - coef(i2,j1) * coef(i1,j2)
    // déterminants 2x2 entre les colonnes 0 et 1 :
    #define det2a(i1, i2)  det2(i1, i2, 0, 1)
    const double a01 = det2a(0,1);
    const double a02 = det2a(0,2);
    const double a03 = det2a(0,3);
    const double a12 = det2a(1,2);
    const double a13 = det2a(1,3);
    const double a23 = det2a(2,3);
    // déterminants 2x2 entre les colonnes 2 et 3 :
    #define det2b(i1, i2)  det2(i1, i2, 2, 3)
    const double b01 = det2b(0,1);
    const double b02 = det2b(0,2);
    const double b03 = det2b(0,3);
    const double b12 = det2b(1,2);
    const double b13 = det2b(1,3);
    const double b23 = det2b(2,3);
    
    // 1ère ligne :
    res(0,0) = + coef(1,1) * b23 - coef(2,1) * b13 + coef(3,1) * b12;
    res(0,1) = - coef(0,1) * b23 + coef(2,1) * b03 - coef(3,1) * b02;
    res(0,2) = + coef(0,1) * b13 - coef(1,1) * b03 + coef(3,1) * b01;
    res(0,3) = - coef(0,1) * b12 + coef(1,1) * b02 - coef(2,1) * b01;
    // 2ème ligne :
    res(1,0) = - coef(1,0) * b23 + coef(2,0) * b13 - coef(3,0) * b12;
    res(1,1) = + coef(0,0) * b23 - coef(2,0) * b03 + coef(3,0) * b02;
    res(1,2) = - coef(0,0) * b13 + coef(1,0) * b03 - coef(3,0) * b01;
    res(1,3) = + coef(0,0) * b12 - coef(1,0) * b02 + coef(2,0) * b01;
    // 3ème ligne :
    res(2,0) = + coef(1,3) * a23 - coef(2,3) * a13 + coef(3,3) * a12;
    res(2,1) = - coef(0,3) * a23 + coef(2,3) * a03 - coef(3,3) * a02;
    res(2,2) = + coef(0,3) * a13 - coef(1,3) * a03 + coef(3,3) * a01;
    res(2,3) = - coef(0,3) * a12 + coef(1,3) * a02 - coef(2,3) * a01;
    // 4ème ligne :
    res(3,0) = - coef(1,2) * a23 + coef(2,2) * a13 - coef(3,2) * a12;
    res(3,1) = + coef(0,2) * a23 - coef(2,2) * a03 + coef(3,2) * a02;
    res(3,2) = - coef(0,2) * a13 + coef(1,2) * a03 - coef(3,2) * a01;
    res(3,3) = + coef(0,2) * a12 - coef(1,2) * a02 + coef(2,2) * a01;
    
    const double det = coef(0,0) * res(0,0) + coef(1,0) * res(0,1) + coef(2,0) * res(0,2) + coef(3,0) * res(0,3);
    for (int c=0; c<16; ++c) res.M[c] /= det;
    return res;
    }


Vec4d Proj3d::operator*(Vec4d v) const {
    Vec4d res;
    for (int i=0; i<4; ++i) {
        res[i] = 0;
        for (int j=0; j<4; ++j)
            res[i] += coef(i,j) * v[j];
        }
    return res;
    }

Vec3d Proj3d::operator*(Vec3d v) const { return Vec3d((*this) * v.toVec4d()); }

void Proj3d::transpose() {
    for (int i=0; i<4; ++i)
        for (int j=i+1; j<4; ++j) {
            double tmp = coef(i,j);
            coef(i,j) = coef(j,i);
            coef(j,i) = tmp;
            }
    }

Proj3d Proj3d::transposed() const {
    Proj3d res;
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            res(i,j) = coef(j,i);
    return res;
    }


/*********************************************************************************************/

Vec4d Proj3d::column(int j) const { return Vec4d(M + 4*j); }

void Proj3d::setColumn(int j, Vec4d c) {
    coef(0,j) = c[0];
    coef(1,j) = c[1];
    coef(2,j) = c[2];
    coef(3,j) = c[3];
    }

/*
Matrix3 Proj3d::subMatrix3() const {
    return Matrix3(
        coef(0,0), coef(0,1), coef(0,2),
        coef(1,0), coef(1,1), coef(1,2),
        coef(2,0), coef(2,1), coef(2,2)
        );
    }
*/
        
/*********************************************************************************************/

void Proj3d::getModelViewFrom(const  qglviewer::Camera &camera) { camera.getModelViewMatrix(M); }
void Proj3d::getProjectionFrom(const qglviewer::Camera &camera) { camera.getProjectionMatrix(M); }

/*********************************************************************************************/

Proj3d Proj3d::getModelViewGL()     { Proj3d res;  glGetDoublev(GL_MODELVIEW_MATRIX,  res.M);  return res; }
Proj3d Proj3d::getProjectionGL()    { Proj3d res;  glGetDoublev(GL_PROJECTION_MATRIX, res.M);  return res; }
Proj3d Proj3d::getTransformGL()     { return getProjectionGL() * getModelViewGL(); }

void Proj3d::loadMatrixGL()     const { glLoadMatrixd(M); }
void Proj3d::loadModelViewGL()  const { glMatrixMode(GL_MODELVIEW);  loadMatrixGL(); }
void Proj3d::loadProjectionGL() const { glMatrixMode(GL_PROJECTION); loadMatrixGL(); }

void Proj3d::multMatrixGL()     const { glMultMatrixd(M); }
void Proj3d::multModelViewGL()  const { glMatrixMode(GL_MODELVIEW);  multMatrixGL(); }
void Proj3d::multProjectionGL() const { glMatrixMode(GL_PROJECTION); multMatrixGL(); }

/*********************************************************************************************/

ProjTransfo Proj3d::toProj3f() const {
    return ProjTransfo(M, false);
    }
        
/*********************************************************************************************/

double Proj3d::screenRatio(double xMin, double xMax, double yMin, double yMax, double z) const {
    Proj3d Tinv = inv();
    Vec4d O = Tinv * Vec4d(xMin, yMin, z, 1),
          X = Tinv * Vec4d(xMax, yMin, z, 1),
          Y = Tinv * Vec4d(xMin, yMax, z, 1);
    //return Vec4d::distance(O, X) / Vec4d::distance(O, Y);
    return dist(Vec3d(O), Vec3d(X)) / dist(Vec3d(O), Vec3d(Y));
    }

/*********************************************************************************************/

void Proj3d::drawBox(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixd(M);
    
    // face bas :
    glBegin(GL_LINE_LOOP);
        glVertex3d(xMin, yMin, zMin);
        glVertex3d(xMax, yMin, zMin);
        glVertex3d(xMax, yMax, zMin);
        glVertex3d(xMin, yMax, zMin);
    glEnd();
    
    // face haut :
    glBegin(GL_LINE_LOOP);
        glVertex3d(xMin, yMin, zMax);
        glVertex3d(xMax, yMin, zMax);
        glVertex3d(xMax, yMax, zMax);
        glVertex3d(xMin, yMax, zMax);
    glEnd();
    
    // segments verticaux :
    glBegin(GL_LINES);
        glVertex3d(xMin, yMin, zMin); glVertex3d(xMin, yMin, zMax);
        glVertex3d(xMax, yMin, zMin); glVertex3d(xMax, yMin, zMax);
        glVertex3d(xMax, yMax, zMin); glVertex3d(xMax, yMax, zMax);
        glVertex3d(xMin, yMax, zMin); glVertex3d(xMin, yMax, zMax);
    glEnd();
    
    glPopMatrix();
    }

/*********************************************************************************************/
// lecture / écriture d'un élément XML :

Proj3d::Proj3d(const QDomElement &element) {
    initFromDOMElement(element);
    }

QDomElement Proj3d::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    for (int j=0; j<4; j++)
        res.appendChild(column(j).domElement("C" + QString::number(j), document));
    return res;
    }

void Proj3d::initFromDOMElement(const QDomElement &element) {
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        setColumn(e.tagName().right(1).toInt(), Vec4d(e));
        e = e.nextSibling().toElement();
        }
    }

/*********************************************************************************************/

// versions uniformes :
Proj3d Proj3d::scaling(double s)                            { return scaling(s,s,s); }
Proj3d Proj3d::scaling(Vec3d s)                             { return scaling(s.x,s.y,s.z); }
Proj3d Proj3d::translation(double t)                        { return translation(t,t,t); }
Proj3d Proj3d::translation(Vec3d t)                         { return translation(t.x,t.y,t.z); }
Proj3d Proj3d::translationAndScaling(double t, double s)    { return translationAndScaling(t,t,t, s,s,s); }
Proj3d Proj3d::translationAndScaling(Vec3d  t, Vec3d  s)    { return translationAndScaling(t.x,t.y,t.z, s.x,s.y,s.z); }
Proj3d Proj3d::scalingAndTranslation(double s, double t)    { return scalingAndTranslation(s,s,s, t,t,t); }
Proj3d Proj3d::scalingAndTranslation(Vec3d  s, Vec3d  t)    { return scalingAndTranslation(s.x,s.y,s.z, t.x,t.y,t.z); }

Proj3d Proj3d::scaling(double sx, double sy, double sz) {
    return Proj3d(
        sx,  0,  0,  0,
         0, sy,  0,  0,
         0,  0, sz,  0,
         0,  0,  0,  1);
    }

Proj3d Proj3d::translation(double tx, double ty, double tz) {
    return Proj3d(
        1, 0, 0, tx,
        0, 1, 0, ty,
        0, 0, 1, tz,
        0, 0, 0,  1);
    }

Proj3d Proj3d::translationAndScaling(double tx, double ty, double tz, double sx, double sy, double sz) {
    return Proj3d(
        sx,  0,  0, sx*tx,
         0, sy,  0, sy*ty,
         0,  0, sz, sz*tz,
         0,  0,  0,     1);
    }

Proj3d Proj3d::scalingAndTranslation(double sx, double sy, double sz, double tx, double ty, double tz) {
    return Proj3d(
        sx,  0,  0, tx,
         0, sy,  0, ty,
         0,  0, sz, tz,
         0,  0,  0,  1);
    }

//#include <Message.h>
Proj3d Proj3d::rotation(double theta, Vec3d n, bool isNormed) {
    if (!isNormed) n.normalize();
    const double c = cos(theta),
                 s = sin(theta),
                 d = 1 - c;
                 //d = (1-c > 1e-3) ?  1 - c : 0.5 * theta*theta;     // dl ordre 2
                 //MSG_DEBUG_VAR(1-c > 1e-5);
    return Proj3d(
        c + d*n.x*n.x     , d*n.x*n.y - s*n.z , d*n.x*n.z + s*n.y ,  0,
        d*n.y*n.x + s*n.z , c + d*n.y*n.y     , d*n.y*n.z - s*n.x ,  0,
        d*n.z*n.x - s*n.y , d*n.z*n.y + s*n.x , c + d*n.z*n.z     ,  0,
        0                 , 0                 , 0                 ,  1
        );
    }

Proj3d Proj3d::rotation(Vec3d src, Vec3d dst, bool normalized) {
    if (!normalized) {
        src.normalize();
        dst.normalize();
        }
    Vec3d n = vec(src, dst);
    const double s = n.norm2(),
                 c = sqrt(1 - s),
                 d = (s > 1e-8) ? (1 - c) / s : 0.5;  // développement limité à l'ordre 1 par rapport à s (en zéro)
    return Proj3d(
        d*n.x*n.x + c   , d*n.x*n.y - n.z , d*n.x*n.z + n.y ,  0,
        d*n.y*n.x + n.z , d*n.y*n.y + c   , d*n.y*n.z - n.x ,  0,
        d*n.z*n.x - n.y , d*n.z*n.y + n.x , d*n.z*n.z + c   ,  0,
        0               , 0               , 0               ,  1
        );
    }

Proj3d Proj3d::rotation(Vec3d d) {
    const double theta = d.norm();
    return rotation(theta, d / theta, true);
    }

Proj3d Proj3d::rotationX(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Proj3d(
        1 , 0 , 0 ,  0,
        0 , c ,-s ,  0,
        0 , s , c ,  0,
        0 , 0 , 0 ,  1
        );
    }

Proj3d Proj3d::rotationY(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Proj3d(
        c , 0 , s ,  0,
        0 , 1 , 0 ,  0,
       -s , 0 , c ,  0,
        0 , 0 , 0 ,  1
        );
    }

Proj3d Proj3d::rotationZ(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Proj3d(
        c ,-s , 0 ,  0,
        s , c , 0 ,  0,
        0 , 0 , 1 ,  0,
        0 , 0 , 0 ,  1
        );
    }
        
Proj3d Proj3d::U2C() {
    return Proj3d(
        2, 0, 0, -1,
        0, 2, 0, -1,
        0, 0, 2, -1,
        0, 0, 0,  1
        );
    }

Proj3d Proj3d::C2U() {
    return Proj3d(
        0.5, 0  , 0  , 0.5,
        0  , 0.5, 0  , 0.5,
        0  , 0  , 0.5, 0.5,
        0  , 0  , 0  , 1
        );
    }

