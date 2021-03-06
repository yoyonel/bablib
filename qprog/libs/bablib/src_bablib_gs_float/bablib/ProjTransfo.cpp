#include "ProjTransfo.h"
#include <QGLViewer/camera.h>
#include <math.h>
BABLIB_NAMESPACE_USE

ProjTransfo::ProjTransfo(float d) {
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            coef(i,j) = (i == j ? d : 0);
    }

ProjTransfo::ProjTransfo(const float M[16], bool deleteArray) {
    for (int i=0; i<16; i++) this->M[i] = M[i];
    if (deleteArray) delete M;
    }

ProjTransfo::ProjTransfo(const double M[16], bool deleteArray) {
    for (int i=0; i<16; i++) this->M[i] = static_cast<float>(M[i]);
    if (deleteArray) delete M;
    }

ProjTransfo::ProjTransfo(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44
        ) {
    M[0] = m11; M[4] = m12; M[8]  = m13; M[12] = m14;
    M[1] = m21; M[5] = m22; M[9]  = m23; M[13] = m24;
    M[2] = m31; M[6] = m32; M[10] = m33; M[14] = m34;
    M[3] = m41; M[7] = m42; M[11] = m43; M[15] = m44;
    }

ProjTransfo::ProjTransfo(Vec4 O, Vec4 X, Vec4 Y, Vec4 Z, bool relative) {
    for (int i=0; i<4; i++) {
        coef(i,0) = X[i]/X.w - (relative ? 0 : O[i]/O.w);
        coef(i,1) = Y[i]/Y.w - (relative ? 0 : O[i]/O.w);
        coef(i,2) = Z[i]/Z.w - (relative ? 0 : O[i]/O.w);
        coef(i,3) = O[i]/O.w;
        }
    }

ProjTransfo::ProjTransfo(Vec3 O, Vec3 X, Vec3 Y, Vec3 Z, bool relative) {
    for (int i=0; i<4; i++) {
        coef(i,0) = (i==3) ? 0 : (X[i] - (relative ? 0 : O[i]));
        coef(i,1) = (i==3) ? 0 : (Y[i] - (relative ? 0 : O[i]));
        coef(i,2) = (i==3) ? 0 : (Z[i] - (relative ? 0 : O[i]));
        coef(i,3) = (i==3) ? 1 : O[i];
        }
    }

ProjTransfo::ProjTransfo(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) {
    float dx = xmax - xmin,
          dy = ymax - ymin,
          dz = zmax - zmin;
    *this = ProjTransfo::translationAndScaling( -xmin, -ymin, -zmin, 1 / dx, 1 / dy, 1 / dz);
    }

ProjTransfo::ProjTransfo( const qglviewer::Camera &camera ) {
	GLdouble _mat[16];
	camera.getModelViewProjectionMatrix( _mat );
	*this = ProjTransfo( _mat );
	}
/*********************************************************************************************/

ProjTransfo ProjTransfo::operator*(ProjTransfo P) const {
    ProjTransfo res;
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
ProjTransfo ProjTransfo::inv() const {
    gsl_matrix *A = gsl_matrix_alloc(4, 4);
    gsl_matrix *B = gsl_matrix_alloc(4, 4);
    gsl_permutation *p = gsl_permutation_alloc(4);
    int s;
    
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            *gsl_matrix_ptr(A, i, j) = coef(i,j);
    
    gsl_linalg_LU_decomp(A, p, &s);
    gsl_linalg_LU_invert(A, p, B);
    
    ProjTransfo res;
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
ProjTransfo ProjTransfo::inv() const {
    ProjTransfo res;
    #define det2(i1, i2, j1, j2)  coef(i1,j1) * coef(i2,j2) - coef(i2,j1) * coef(i1,j2)
    // déterminants 2x2 entre les colonnes 0 et 1 :
    #define det2a(i1, i2)  det2(i1, i2, 0, 1)
    const float a01 = det2a(0,1);
    const float a02 = det2a(0,2);
    const float a03 = det2a(0,3);
    const float a12 = det2a(1,2);
    const float a13 = det2a(1,3);
    const float a23 = det2a(2,3);
    // déterminants 2x2 entre les colonnes 2 et 3 :
    #define det2b(i1, i2)  det2(i1, i2, 2, 3)
    const float b01 = det2b(0,1);
    const float b02 = det2b(0,2);
    const float b03 = det2b(0,3);
    const float b12 = det2b(1,2);
    const float b13 = det2b(1,3);
    const float b23 = det2b(2,3);
    
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
    
    const float det = coef(0,0) * res(0,0) + coef(1,0) * res(0,1) + coef(2,0) * res(0,2) + coef(3,0) * res(0,3);
    for (int c=0; c<16; ++c) res.M[c] /= det;
    return res;
    }


Vec4 ProjTransfo::operator*(Vec4 v) const {
    Vec4 res;
    for (int i=0; i<4; ++i) {
        res[i] = 0;
        for (int j=0; j<4; ++j)
            res[i] += coef(i,j) * v[j];
        }
    return res;
    }

Vec3 ProjTransfo::operator*(Vec3 v) const { return Vec3((*this) * v.toVec4()); }

void ProjTransfo::transpose() {
    for (int i=0; i<4; ++i)
        for (int j=i+1; j<4; ++j) {
            float tmp = coef(i,j);
            coef(i,j) = coef(j,i);
            coef(j,i) = tmp;
            }
    }

ProjTransfo ProjTransfo::transposed() const {
    ProjTransfo res;
    for (int i=0; i<4; ++i)
        for (int j=0; j<4; ++j)
            res(i,j) = coef(j,i);
    return res;
    }


/*********************************************************************************************/

Vec4 ProjTransfo::column(int j) const { return Vec4(M + 4*j); }

void ProjTransfo::setColumn(int j, Vec4 c) {
    coef(0,j) = c[0];
    coef(1,j) = c[1];
    coef(2,j) = c[2];
    coef(3,j) = c[3];
    }

/*********************************************************************************************/

void ProjTransfo::getModelViewFrom(const qglviewer::Camera &camera) {
    GLdouble matrix[16];
    camera.getModelViewMatrix(matrix);
    for (int i=0; i<16; i++) M[i] = static_cast<float>(matrix[i]);
    }

void ProjTransfo::getProjectionFrom(const qglviewer::Camera &camera) {
    GLdouble matrix[16];
    camera.getProjectionMatrix(matrix);
    for (int i=0; i<16; i++) M[i] = static_cast<float>(matrix[i]);
    }

ProjTransfo ProjTransfo::getModelView(const qglviewer::Camera &camera) {
	ProjTransfo res;
	res.getModelViewFrom( camera );
	return res;    
    }

ProjTransfo ProjTransfo::getProjection(const qglviewer::Camera &camera) {
	ProjTransfo res;
	res.getProjectionFrom( camera );
	return res;    
    }

/*********************************************************************************************/

ProjTransfo ProjTransfo::getGLModelView() {
    ProjTransfo res;
    glGetFloatv(GL_MODELVIEW_MATRIX, res.M);
    return res;
    }

ProjTransfo ProjTransfo::getGLProjection() {
    ProjTransfo res;
    glGetFloatv(GL_PROJECTION_MATRIX, res.M);
    return res;
    }

ProjTransfo ProjTransfo::getGLTransform() { return getGLProjection() * getGLModelView(); }

/*********************************************************************************************/

void ProjTransfo::glLoadMatrix() const { glLoadMatrixf(M); }
void ProjTransfo::glLoadModelView() const { glMatrixMode(GL_MODELVIEW); glLoadMatrix(); }
void ProjTransfo::glLoadProjection() const { glMatrixMode(GL_PROJECTION); glLoadMatrix(); }

void ProjTransfo::glMultMatrix() const { glMultMatrixf(M); }
void ProjTransfo::glMultModelView() const { glMatrixMode(GL_MODELVIEW); glMultMatrix(); }
void ProjTransfo::glMultProjection() const { glMatrixMode(GL_PROJECTION); glMultMatrix(); }

/*********************************************************************************************/

float ProjTransfo::screenRatio(float xMin, float xMax, float yMin, float yMax, float z) const {
    ProjTransfo Tinv = this->inv();
    Vec4 O = Tinv * Vec4(xMin, yMin, z, 1),
         X = Tinv * Vec4(xMax, yMin, z, 1),
         Y = Tinv * Vec4(xMin, yMax, z, 1);
    //return Vec4::distance(O, X) / Vec4::distance(O, Y);
    return dist(Vec3(O), Vec3(X)) / dist(Vec3(O), Vec3(Y));
    }

/*********************************************************************************************/

void ProjTransfo::drawBox(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) const {
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

/*********************************************************************************************/
// lecture / écriture d'un élément XML :

ProjTransfo::ProjTransfo(const QDomElement &element) {
    initFromDOMElement(element);
    }

QDomElement ProjTransfo::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    for (int j=0; j<4; j++)
        res.appendChild(column(j).domElement("C" + QString::number(j), document));
    return res;
    }

void ProjTransfo::initFromDOMElement(const QDomElement &element) {
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        setColumn(e.tagName().right(1).toInt(), Vec4(e));
        e = e.nextSibling().toElement();
        }
    }

/*********************************************************************************************/

// versions uniformes :
ProjTransfo ProjTransfo::scaling(float s)                           { return scaling(s,s,s); }
ProjTransfo ProjTransfo::scaling(Vec3 s)                            { return scaling(s.x,s.y,s.z); }
ProjTransfo ProjTransfo::translation(float t)                       { return translation(t,t,t); }
ProjTransfo ProjTransfo::translation(Vec3 t)                        { return translation(t.x,t.y,t.z); }
ProjTransfo ProjTransfo::translationAndScaling(float t, float s)    { return translationAndScaling(t,t,t, s,s,s); }
ProjTransfo ProjTransfo::translationAndScaling(Vec3  t, Vec3  s)    { return translationAndScaling(t.x,t.y,t.z, s.x,s.y,s.z); }
ProjTransfo ProjTransfo::scalingAndTranslation(float s, float t)    { return scalingAndTranslation(s,s,s, t,t,t); }
ProjTransfo ProjTransfo::scalingAndTranslation(Vec3  s, Vec3  t)    { return scalingAndTranslation(s.x,s.y,s.z, t.x,t.y,t.z); }


ProjTransfo ProjTransfo::scaling(float sx, float sy, float sz) {
    return ProjTransfo(
        sx,  0,  0,  0,
         0, sy,  0,  0,
         0,  0, sz,  0,
         0,  0,  0,  1);
    }

ProjTransfo ProjTransfo::translation(float tx, float ty, float tz) {
    return ProjTransfo(
        1, 0, 0, tx,
        0, 1, 0, ty,
        0, 0, 1, tz,
        0, 0, 0,  1);
    }

ProjTransfo ProjTransfo::translationAndScaling(float tx, float ty, float tz, float sx, float sy, float sz) {
    return ProjTransfo(
        sx,  0,  0, sx*tx,
         0, sy,  0, sy*ty,
         0,  0, sz, sz*tz,
         0,  0,  0,     1);
    }

ProjTransfo ProjTransfo::scalingAndTranslation(float sx, float sy, float sz, float tx, float ty, float tz) {
    return ProjTransfo(
        sx,  0,  0, tx,
         0, sy,  0, ty,
         0,  0, sz, tz,
         0,  0,  0,  1);
    }

ProjTransfo ProjTransfo::rotation(float theta, Vec3 n, bool isNormed) {
    if (!isNormed) n.normalize();
    const float c = cosf(theta),
                s = sinf(theta),
                d = 1 - c;
    return ProjTransfo(
        c + d*n.x*n.x     , d*n.x*n.y - s*n.z , d*n.x*n.z + s*n.y ,  0,
        d*n.y*n.x + s*n.z , c + d*n.y*n.y     , d*n.y*n.z - s*n.x ,  0,
        d*n.z*n.x - s*n.y , d*n.z*n.y + s*n.x , c + d*n.z*n.z     ,  0,
        0                 , 0                 , 0                 ,  1
        );
    }

ProjTransfo ProjTransfo::rotation(Vec3 src, Vec3 dst, bool normalized) {
    if (!normalized) {
        src.normalize();
        dst.normalize();
        }
    Vec3 n = vec(src, dst);
    const float s = n.norm2(),
                c = sqrtf(1 - s),
                d = (s > 1e-8f) ? (1 - c) / s : 0.5f;  // développement limité à l'ordre 1 par rapport à s (en zéro)
    return ProjTransfo(
        d*n.x*n.x + c   , d*n.x*n.y - n.z , d*n.x*n.z + n.y ,  0,
        d*n.y*n.x + n.z , d*n.y*n.y + c   , d*n.y*n.z - n.x ,  0,
        d*n.z*n.x - n.y , d*n.z*n.y + n.x , d*n.z*n.z + c   ,  0,
        0               , 0               , 0               ,  1
        );
    }

ProjTransfo ProjTransfo::rotation(Vec3 d) {
    const float theta = d.norm();
    return rotation(theta, d / theta, true);
    }

ProjTransfo ProjTransfo::U2C() {
    return ProjTransfo(
        2, 0, 0, -1,
        0, 2, 0, -1,
        0, 0, 2, -1,
        0, 0, 0,  1
        );
    }

ProjTransfo ProjTransfo::C2U() {
    return ProjTransfo(
        0.5, 0  , 0  , 0.5,
        0  , 0.5, 0  , 0.5,
        0  , 0  , 0.5, 0.5,
        0  , 0  , 0  , 1
        );
    }

