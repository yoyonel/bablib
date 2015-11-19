#include <OpenGL.h>
#include "Vec4d.h"
#include "Vec3.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

Vec4d::Vec4d() : x(0), y(0), z(0), w(1) {}
Vec4d::Vec4d(double c) : x(c), y(c), z(c), w(c) {}
Vec4d::Vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
Vec4d::Vec4d(const Vec4d &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
Vec4d::Vec4d(const double *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
Vec4d::Vec4d(Vec3d v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

/*********************************************************************************************/

Vec3d   Vec4d::toVec3d()   const { return Vec3d(xn(), yn(), zn()); }
QString Vec4d::toQString() const { return QString("(%1, %2, %3, %4)").arg(x).arg(y).arg(z).arg(w); }

/*********************************************************************************************/

void Vec4d::glVertex() const { glVertex4dv(v); }

void Vec4d::homogenize() {
    x /= w;
    y /= w;
    z /= w;
    w = 1;
    }

/*********************************************************************************************/

Vec4d dir(const Vec4d &a, const Vec4d &b) {
    /*
    return Vec4d(
        a.w * b.x - b.w * a.x,
        a.w * b.y - b.w * a.y,
        a.w * b.z - b.w * a.z,
        0);
    */
    Vec4d d(
        a.w * b.x - b.w * a.x,
        a.w * b.y - b.w * a.y,
        a.w * b.z - b.w * a.z,
        0);
    return ((a.w < 0.0) != (b.w < 0.0)) ? -d : d;
    }

/*********************************************************************************************/

Vec4d operator-(const Vec4d &a) { return Vec4d(-a.x, -a.y, -a.z, -a.w); }
    
/*********************************************************************************************/
    
Vec4d operator+(const Vec4d &a, const Vec4d &b) { return Vec4d(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
Vec4d operator-(const Vec4d &a, const Vec4d &b) { return Vec4d(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
Vec4d operator*(const Vec4d &a, const Vec4d &b) { return Vec4d(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
Vec4d operator/(const Vec4d &a, const Vec4d &b) { return Vec4d(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

/*********************************************************************************************/

Vec4d operator*(double s, const Vec4d &a) { return Vec4d(s * a.x, s * a.y, s * a.z, s * a.w); }
Vec4d operator*(const Vec4d &a, double s) { return s * a; }
Vec4d operator/(const Vec4d &a, double s) { return Vec4d(a.x / s, a.y / s, a.z / s, a.w / s); }
Vec4d operator/(double s, const Vec4d &a) { return Vec4d(s / a.x, s / a.y, s / a.z, s / a.w); }

/*********************************************************************************************/

Vec4d& Vec4d::operator+=(const Vec4d &a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
Vec4d& Vec4d::operator-=(const Vec4d &a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
Vec4d& Vec4d::operator*=(const Vec4d &a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
Vec4d& Vec4d::operator/=(const Vec4d &a) { x/=a.x; y/=a.y; z/=a.z; w/=a.w; return *this; }
Vec4d& Vec4d::operator*=(double s) { x*=s; y*=s; z*=s; w*=s; return *this; }
Vec4d& Vec4d::operator/=(double s) { x/=s; y/=s; z/=s; w/=s; return *this; }

/*********************************************************************************************/

bool operator==(const Vec4d &a, const Vec4d &b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;}
bool operator!=(const Vec4d &a, const Vec4d &b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;}
bool operator>=(const Vec4d &a, const Vec4d &b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z && a.w >= b.w; }
bool operator<=(const Vec4d &a, const Vec4d &b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z && a.w <= b.w; }
bool operator> (const Vec4d &a, const Vec4d &b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z && a.w >  b.w; }
bool operator< (const Vec4d &a, const Vec4d &b) { return a.x <  b.x && a.y <  b.y && a.z <  b.z && a.w <  b.w; }
bool operator>=(const Vec4d &a, double  b) { return a.x >= b && a.y >= b && a.z >= b && a.w >= b; }
bool operator<=(const Vec4d &a, double  b) { return a.x <= b && a.y <= b && a.z <= b && a.w <= b; }
bool operator> (const Vec4d &a, double  b) { return a.x >  b && a.y >  b && a.z >  b && a.w >  b; }
bool operator< (const Vec4d &a, double  b) { return a.x <  b && a.y <  b && a.z <  b && a.w <  b; }

/*********************************************************************************************/

// définition de fonctions de base :
namespace {
    double sign(double a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    double fract(double a) { return a - floor(a); }
    double step(double e, double x) { return x < e ? 0 : 1; }
    double mix(double x, double y, double a) { return (1 - a) * x + a * y; }
    }
    
Vec4d abs(const Vec4d &a)   { return Vec4d(fabs(a.x),  fabs(a.y),  fabs(a.z),  fabs(a.w)); }
Vec4d sign(const Vec4d &a)  { return Vec4d(sign(a.x),  sign(a.y),  sign(a.z),  sign(a.w));  }
Vec4d floor(const Vec4d &a) { return Vec4d(floor(a.x), floor(a.y), floor(a.z), floor(a.w)); }
Vec4d ceil(const Vec4d &a)  { return Vec4d(ceil(a.x),  ceil(a.y),  ceil(a.z),  ceil(a.w));  }
Vec4d fract(const Vec4d &a) { return Vec4d(fract(a.x), fract(a.y), fract(a.z), fract(a.w)); }
                                                                                        
Vec4d min(const Vec4d &a, const Vec4d &b) { return Vec4d(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z), fmin(a.w, b.w)); }
Vec4d min(const Vec4d &a, double b)       { return Vec4d(fmin(a.x, b),   fmin(a.y, b),   fmin(a.z, b),   fmin(a.w, b));   }
Vec4d max(const Vec4d &a, const Vec4d &b) { return Vec4d(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z), fmax(a.w, b.w)); }
Vec4d max(const Vec4d &a, double b)       { return Vec4d(fmax(a.x, b),   fmax(a.y, b),   fmax(a.z, b),   fmax(a.w, b));   }
Vec4d clamp(const Vec4d &a, const Vec4d &vMin, const Vec4d &vMax) { return min(max(a, vMin), vMax); }
Vec4d clamp(const Vec4d &a, double vMin, double vMax)             { return min(max(a, vMin), vMax); }
Vec4d mix(const Vec4d &a, const Vec4d &b, const Vec4d &alpha) { return Vec4d(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y), mix(a.z, b.z, alpha.z), mix(a.w, b.w, alpha.w)); }
Vec4d mix(const Vec4d &a, const Vec4d &b, double alpha)       { return Vec4d(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha),   mix(a.z, b.z, alpha),   mix(a.w, b.w, alpha)); }
Vec4d step(const Vec4d &e, const Vec4d &a) { return Vec4d(step(e.x, a.x), step(e.y, a.y), step(e.z, a.z), step(e.w, a.w)); }
Vec4d step(double e, const Vec4d &a) { return Vec4d(step(e, a.x), step(e, a.y), step(e, a.z), step(e, a.w)); }

double min(const Vec4d &a) { return fmin(fmin(a.x, a.y), fmin(a.z, a.w)); }
double max(const Vec4d &a) { return fmax(fmax(a.x, a.y), fmax(a.z, a.w)); }

/*********************************************************************************************/

Vec4d Vec4d::random() { return Vec4d(double(Random::unif()), double(Random::unif()), double(Random::unif()), double(Random::unif())); }
Vec4d Vec4d::random(double min, double max) { return min + (max - min) * random(); }
Vec4d Vec4d::random(Vec4d  min, Vec4d  max) { return min + (max - min) * random(); }

/*********************************************************************************************/
// lecture / écriture XML :

Vec4d::Vec4d(const QDomElement &element) {
    initFromDOMElement(element);
    }

QDomElement Vec4d::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    res.setAttribute("x2", QString::number(v[2]));
    res.setAttribute("x3", QString::number(v[3]));
    return res;
    }

void Vec4d::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toDouble();
    v[1] = element.attribute("x1", "0.0").toDouble();
    v[2] = element.attribute("x2", "0.0").toDouble();
    v[3] = element.attribute("x3", "1.0").toDouble();
    }

BABLIB_NAMESPACE_END

