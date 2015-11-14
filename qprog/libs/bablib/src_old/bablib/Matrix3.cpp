#include "Matrix3.h"
#include <math.h>
BABLIB_NAMESPACE_USE

Matrix3::Matrix3(float d) {
    coef(0,0) =    d; coef(0,1) = 0.0f; coef(0,2) = 0.0f;
    coef(1,0) = 0.0f; coef(1,1) =    d; coef(1,2) = 0.0f;
    coef(2,0) = 0.0f; coef(2,1) = 0.0f; coef(2,2) =    d;
    }

Matrix3::Matrix3(float c11, float c12, float c13,
                 float c21, float c22, float c23,
                 float c31, float c32, float c33) {
    coef(0,0) = c11; coef(0,1) = c12; coef(0,2) = c13;
    coef(1,0) = c21; coef(1,1) = c22; coef(1,2) = c23;
    coef(2,0) = c31; coef(2,1) = c32; coef(2,2) = c33;
    }

Matrix3::Matrix3(const float M[9]) {
    for (int i=0; i<9; i++) this->M[i] = M[i];
    }

Matrix3::Matrix3(const double M[9]) {
    for (int i=0; i<9; i++) this->M[i] = static_cast<float>(M[i]);
    }

Matrix3::Matrix3(Vec3 c0, Vec3 c1, Vec3 c2) {
    setColumn(0, c0);
    setColumn(1, c1);
    setColumn(2, c2);
    }

/*********************************************************************************************/

Vec3 Matrix3::column(int j) const { return Vec3(M + 3*j); }

void Matrix3::setColumn(int j, Vec3 c) {
    coef(0,j) = c[0];
    coef(1,j) = c[1];
    coef(2,j) = c[2];
    }

/*********************************************************************************************/

Matrix3 Matrix3::operator*(Matrix3 P) const {
    Matrix3 res;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            res(i,j) = 0.0f;
            for (int k=0; k<3; k++)
                res(i,j) += coef(i,k) * P(k,j);
            }
    return res;
    }

Vec3 Matrix3::operator*(Vec3 v) const {
    Vec3 res;
    for (int i=0; i<3; ++i) {
        res[i] = 0;
        for (int j=0; j<3; ++j)
            res[i] += coef(i,j) * v[j];
        }
    return res;
    }

Vec2 Matrix3::operator*(Vec2 v) const {
    const Vec3 p = (*this) * Vec3(v, 1.0f);
    return Vec2(p.x / p.z, p.y / p.z);
    }


Matrix3 Matrix3::inv() const {
    #define det2(i1, i2, j1, j2)  coef(i1,j1) * coef(i2,j2) - coef(i2,j1) * coef(i1,j2)
    #define next_eval(i) next_ ## i
    #define next(i) next_eval(i)
    #define next_0 1
    #define next_1 2
    #define next_2 0
    #define cofact(i,j) det2(next(i), next(next(i)), next(next(j)), next(j))
    const float d00 = cofact(0,0);
    const float d10 = cofact(1,0);
    const float d20 = cofact(2,0);
    const float d01 = cofact(0,1);
    const float d11 = cofact(1,1);
    const float d21 = cofact(2,1);
    const float d02 = cofact(0,2);
    const float d12 = cofact(1,2);
    const float d22 = cofact(2,2);
    #undef next_0
    #undef next_1
    #undef next_2
    #undef cofact
    #undef det2
    
    // computation of determinant using the first column:
    const float det = coef(0,0) * d00 + coef(1,0) * d10 + coef(2,0) * d20;
    return Matrix3(
        d00 / det, d10 / det, d20 / det,    // adjugate = transposed comatrix
        d01 / det, d11 / det, d21 / det,
        d02 / det, d12 / det, d22 / det
        );
    }

void Matrix3::transpose() {
    for (int i=0; i<3; ++i)
        for (int j=i+1; j<3; ++j) {
            float tmp = coef(i,j);
            coef(i,j) = coef(j,i);
            coef(j,i) = tmp;
            }
    }

Matrix3 Matrix3::transposed() const {
    Matrix3 res;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            res(i,j) = coef(j,i);
    return res;
    }

/*********************************************************************************************/

Matrix3 Matrix3::rotation(float theta, Vec3 n, bool isNormed) {
    if (!isNormed) n.normalize();
    const float c = cosf(theta),
                s = sinf(theta),
                d = 1 - c;
    return Matrix3(
        c + d*n.x*n.x     , d*n.x*n.y - s*n.z , d*n.x*n.z + s*n.y ,
        d*n.y*n.x + s*n.z , c + d*n.y*n.y     , d*n.y*n.z - s*n.x ,
        d*n.z*n.x - s*n.y , d*n.z*n.y + s*n.x , c + d*n.z*n.z
        );
    }

Matrix3 Matrix3::rotationX(float theta) {
    const float c = cosf(theta);
    const float s = sinf(theta);
    return Matrix3(
        1 , 0 , 0 ,
        0 , c ,-s ,
        0 , s , c 
        );
    }

Matrix3 Matrix3::rotationY(float theta) {
    const float c = cosf(theta);
    const float s = sinf(theta);
    return Matrix3(
        c , 0 , s ,
        0 , 1 , 0 ,
       -s , 0 , c 
        );
    }

Matrix3 Matrix3::rotationZ(float theta) {
    const float c = cosf(theta);
    const float s = sinf(theta);
    return Matrix3(
        c ,-s , 0 ,
        s , c , 0 ,
        0 , 0 , 1 
        );
    }
        
Matrix3 Matrix3::rotation(Vec3 src, Vec3 dst, bool normalized) {
    if (!normalized) {
        src.normalize();
        dst.normalize();
        }
    const Vec3 n = vec(src, dst);
    const float s = n.norm2(),
                c = sqrtf(1 - s),
                d = (s > 1e-8f) ? (1 - c) / s : 0.5f;  // développement limité à l'ordre 1 par rapport à s (en zéro)
    return Matrix3(
        d*n.x*n.x + c   , d*n.x*n.y - n.z , d*n.x*n.z + n.y ,
        d*n.y*n.x + n.z , d*n.y*n.y + c   , d*n.y*n.z - n.x ,
        d*n.z*n.x - n.y , d*n.z*n.y + n.x , d*n.z*n.z + c   );
    }

/*********************************************************************************************/

QString Matrix3::toQS() const {
    return QString("[c0=%1, c1=%2, c2=%3]").arg(column(0).toQString()).arg(column(1).toQString()).arg(column(2).toQString());
    }

/*********************************************************************************************/

QDomElement Matrix3::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    for (int j=0; j<3; j++)
        res.appendChild(column(j).domElement("C" + QString::number(j), document));
    return res;
    }

void Matrix3::initFromDOMElement(const QDomElement &element) {
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        setColumn(e.tagName().right(1).toInt(), Vec3(e));
        e = e.nextSibling().toElement();
        }
    }
/*********************************************************************************************/
// interface with QGLViewer classes :

qglviewer::Quaternion Matrix3::toQuat() const {
    qglviewer::Quaternion q;
    q.setFromRotatedBasis(
        qglviewer::Vec(column(0)),
        qglviewer::Vec(column(1)),
        qglviewer::Vec(column(2))
        );
    return q;
    }

