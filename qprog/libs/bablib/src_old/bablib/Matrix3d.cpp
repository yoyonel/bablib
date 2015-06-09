#include "Matrix3d.h"
#include <math.h>
BABLIB_NAMESPACE_USE

Matrix3d::Matrix3d(double d) {
    coef(0,0) =   d; coef(0,1) = 0.0; coef(0,2) = 0.0;
    coef(1,0) = 0.0; coef(1,1) =   d; coef(1,2) = 0.0;
    coef(2,0) = 0.0; coef(2,1) = 0.0; coef(2,2) =   d;
    }

Matrix3d::Matrix3d(double c11, double c12, double c13,
                   double c21, double c22, double c23,
                   double c31, double c32, double c33) {
    coef(0,0) = c11; coef(0,1) = c12; coef(0,2) = c13;
    coef(1,0) = c21; coef(1,1) = c22; coef(1,2) = c23;
    coef(2,0) = c31; coef(2,1) = c32; coef(2,2) = c33;
    }

Matrix3d::Matrix3d(const double M[9]) {
    for (int i=0; i<9; i++) this->M[i] = M[i];
    }

Matrix3d::Matrix3d(Vec3d c0, Vec3d c1, Vec3d c2) {
    setColumn(0, c0);
    setColumn(1, c1);
    setColumn(2, c2);
    }

/*********************************************************************************************/

Vec3d Matrix3d::column(int j) const { return Vec3d(M + 3*j); }

void Matrix3d::setColumn(int j, Vec3d c) {
    coef(0,j) = c[0];
    coef(1,j) = c[1];
    coef(2,j) = c[2];
    }

/*********************************************************************************************/

Matrix3d Matrix3d::operator*(Matrix3d P) const {
    Matrix3d res;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j) {
            res(i,j) = 0.0;
            for (int k=0; k<3; k++)
                res(i,j) += coef(i,k) * P(k,j);
            }
    return res;
    }

Vec3d Matrix3d::operator*(Vec3d v) const {
    Vec3d res;
    for (int i=0; i<3; ++i) {
        res[i] = 0;
        for (int j=0; j<3; ++j)
            res[i] += coef(i,j) * v[j];
        }
    return res;
    }

Vec2d Matrix3d::operator*(Vec2d v) const {
    const Vec3d p = (*this) * Vec3d(v, 1.0);
    return Vec2d(p.x / p.z, p.y / p.z);
    }


Matrix3d Matrix3d::inv() const {
    #define det2(i1, i2, j1, j2)  coef(i1,j1) * coef(i2,j2) - coef(i2,j1) * coef(i1,j2)
    #define next_eval(i) next_ ## i
    #define next(i) next_eval(i)
    #define next_0 1
    #define next_1 2
    #define next_2 0
    #define cofact(i,j) det2(next(i), next(next(i)), next(next(j)), next(j))
    const double d00 = cofact(0,0);
    const double d10 = cofact(1,0);
    const double d20 = cofact(2,0);
    const double d01 = cofact(0,1);
    const double d11 = cofact(1,1);
    const double d21 = cofact(2,1);
    const double d02 = cofact(0,2);
    const double d12 = cofact(1,2);
    const double d22 = cofact(2,2);
    #undef next_0
    #undef next_1
    #undef next_2
    #undef cofact
    #undef det2
    
    // computation of determinant using the first column:
    const double det = coef(0,0) * d00 + coef(1,0) * d10 + coef(2,0) * d20;
    return Matrix3d(
        d00 / det, d10 / det, d20 / det,    // adjugate = transposed comatrix
        d01 / det, d11 / det, d21 / det,
        d02 / det, d12 / det, d22 / det
        );
    }

void Matrix3d::transpose() {
    for (int i=0; i<3; ++i)
        for (int j=i+1; j<3; ++j) {
            double tmp = coef(i,j);
            coef(i,j) = coef(j,i);
            coef(j,i) = tmp;
            }
    }

Matrix3d Matrix3d::transposed() const {
    Matrix3d res;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            res(i,j) = coef(j,i);
    return res;
    }

/*********************************************************************************************/

Matrix3d Matrix3d::rotation(double theta, Vec3 n, bool isNormed) {
    if (!isNormed) n.normalize();
    const double c = cos(theta),
                 s = sin(theta),
                 d = 1 - c;
    return Matrix3d(
        c + d*n.x*n.x     , d*n.x*n.y - s*n.z , d*n.x*n.z + s*n.y ,
        d*n.y*n.x + s*n.z , c + d*n.y*n.y     , d*n.y*n.z - s*n.x ,
        d*n.z*n.x - s*n.y , d*n.z*n.y + s*n.x , c + d*n.z*n.z
        );
    }

Matrix3d Matrix3d::rotationX(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Matrix3d(
        1 , 0 , 0 ,
        0 , c ,-s ,
        0 , s , c 
        );
    }

Matrix3d Matrix3d::rotationY(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Matrix3d(
        c , 0 , s ,
        0 , 1 , 0 ,
       -s , 0 , c 
        );
    }

Matrix3d Matrix3d::rotationZ(double theta) {
    const double c = cos(theta);
    const double s = sin(theta);
    return Matrix3d(
        c ,-s , 0 ,
        s , c , 0 ,
        0 , 0 , 1 
        );
    }
        
/*********************************************************************************************/

Matrix3 Matrix3d::toMatrix3f() const {
    return Matrix3(M);
    }
        
/*********************************************************************************************/

QString Matrix3d::toQS() const {
    return QString("[c0=%1, c1=%2, c2=%3]").arg(column(0).toQString()).arg(column(1).toQString()).arg(column(2).toQString());
    }

/*********************************************************************************************/

QDomElement Matrix3d::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    for (int j=0; j<3; j++)
        res.appendChild(column(j).domElement("C" + QString::number(j), document));
    return res;
    }

void Matrix3d::initFromDOMElement(const QDomElement &element) {
    QDomElement e = element.firstChild().toElement();
    while(!e.isNull()) {
        setColumn(e.tagName().right(1).toInt(), Vec3(e));
        e = e.nextSibling().toElement();
        }
    }

/*********************************************************************************************/
// interface with QGLViewer classes :

qglviewer::Quaternion Matrix3d::toQuat() const {
    qglviewer::Quaternion q;
    q.setFromRotatedBasis(
        qglviewer::Vec(column(0)),   //@ to improve : avoid converting down to floats
        qglviewer::Vec(column(1)),   //@ to improve : avoid converting down to floats
        qglviewer::Vec(column(2))    //@ to improve : avoid converting down to floats
        );
    return q;
    }

