#include <OpenGL.h>
#include "Vec3d.h"
#include "Vec2d.h"
#include "Vec4d.h"
#include "Vec3.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

/*********************************************************************************************/

const Vec3d Vec3d::ZERO   (0.0, 0.0, 0.0);
const Vec3d Vec3d::ONE    (1.0, 1.0, 1.0);
const Vec3d Vec3d::X      (1.0, 0.0, 0.0);
const Vec3d Vec3d::Y      (0.0, 1.0, 0.0);
const Vec3d Vec3d::Z      (0.0, 0.0, 1.0);

/*********************************************************************************************/

QString Vec3d::toQString() const { return QString("(%1, %2, %3)").arg(x).arg(y).arg(z); }
Vec4d    Vec3d::toVec4d()    const { return Vec4d(x, y, z, 1); }

/*********************************************************************************************/

double Vec3d::norm2()      const { return x*x + y*y + z*z; }
double Vec3d::norm()       const { return sqrt(norm2()); }
Vec3d  Vec3d::normalized() const { return (*this) / norm(); }
void   Vec3d::normalize()        { (*this) /= norm(); }

double dist(Vec3d v1, Vec3d v2) { return (v2 - v1).norm(); }

Vec3d triangleNormal(const Vec3d &a, const Vec3d &b, const Vec3d &c) { return ((b - a) ^ (c - a)).normalized(); }

Vec3d Vec3d::ortho() const {
    if ((fabs(y) >= 0.9 * fabs(x)) && (fabs(z) >= 0.9 * fabs(x)))
        return Vec3d(0, -z, y);
    else if ((fabs(x) >= 0.9 * fabs(y)) && (fabs(z) >= 0.9 * fabs(y)))
        return Vec3d(-z, 0, x);
    else
        return Vec3d(-y, x, 0);
    }
        
/*********************************************************************************************/

Vec3d operator-(const Vec3d &a) { return Vec3d(-a.x, -a.y, -a.z); }
    
/*********************************************************************************************/
    
Vec3d operator+(const Vec3d &a, const Vec3d &b) { return Vec3d(a.x + b.x, a.y + b.y, a.z + b.z); }
Vec3d operator-(const Vec3d &a, const Vec3d &b) { return Vec3d(a.x - b.x, a.y - b.y, a.z - b.z); }
Vec3d operator*(const Vec3d &a, const Vec3d &b) { return Vec3d(a.x * b.x, a.y * b.y, a.z * b.z); }
Vec3d operator/(const Vec3d &a, const Vec3d &b) { return Vec3d(a.x / b.x, a.y / b.y, a.z / b.z); }

/*********************************************************************************************/

Vec3d operator*(double s, const Vec3d &a) { return Vec3d(s * a.x, s * a.y, s * a.z); }
Vec3d operator*(const Vec3d &a, double s) { return s * a; }
Vec3d operator/(const Vec3d &a, double s) { return Vec3d(a.x / s, a.y / s, a.z / s); }
Vec3d operator/(double s, const Vec3d &a) { return Vec3d(s / a.x, s / a.y, s / a.z); }

/*********************************************************************************************/

Vec3d& Vec3d::operator+=(const Vec3d &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
Vec3d& Vec3d::operator-=(const Vec3d &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
Vec3d& Vec3d::operator*=(const Vec3d &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
Vec3d& Vec3d::operator/=(const Vec3d &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
Vec3d& Vec3d::operator*=(double s) { x*=s; y*=s; z*=s; return *this; }
Vec3d& Vec3d::operator/=(double s) { x/=s; y/=s; z/=s; return *this; }

/*********************************************************************************************/

bool operator==(const Vec3d &a, const Vec3d &b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
bool operator!=(const Vec3d &a, const Vec3d &b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
bool operator>=(const Vec3d &a, const Vec3d &b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z; }
bool operator<=(const Vec3d &a, const Vec3d &b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z; }
bool operator> (const Vec3d &a, const Vec3d &b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z; }
bool operator< (const Vec3d &a, const Vec3d &b) { return a.x <  b.x && a.y <  b.y && a.z <  b.z; }
bool operator>=(const Vec3d &a, double  b) { return a.x >= b && a.y >= b && a.z >= b; }
bool operator<=(const Vec3d &a, double  b) { return a.x <= b && a.y <= b && a.z <= b; }
bool operator> (const Vec3d &a, double  b) { return a.x >  b && a.y >  b && a.z >  b; }
bool operator< (const Vec3d &a, double  b) { return a.x <  b && a.y <  b && a.z <  b; }

/*********************************************************************************************/

// dÃÂ©finition de fonctions de base :
namespace {
    double sign(double a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    double fract(double a) { return a - floor(a); }
    double step(double e, double x) { return x < e ? 0 : 1; }
    double mix(double x, double y, double a) { return (1 - a) * x + a * y; }
    }
    
Vec3d abs(const Vec3d &a)   { return Vec3d(fabs(a.x),  fabs(a.y),  fabs(a.z));  }
Vec3d sign(const Vec3d &a)  { return Vec3d(sign(a.x),  sign(a.y),  sign(a.z));  }
Vec3d floor(const Vec3d &a) { return Vec3d(floor(a.x), floor(a.y), floor(a.z)); }
Vec3d ceil(const Vec3d &a)  { return Vec3d(ceil(a.x),  ceil(a.y),  ceil(a.z));  }
Vec3d fract(const Vec3d &a) { return Vec3d(fract(a.x), fract(a.y), fract(a.z)); }

Vec3d min(const Vec3d &a, const Vec3d &b) { return Vec3d(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z)); }
Vec3d min(const Vec3d &a, double b)       { return Vec3d(fmin(a.x, b),   fmin(a.y, b),   fmin(a.z, b));   }
Vec3d max(const Vec3d &a, const Vec3d &b) { return Vec3d(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z)); }
Vec3d max(const Vec3d &a, double b)       { return Vec3d(fmax(a.x, b),   fmax(a.y, b),   fmax(a.z, b));   }
Vec3d clamp(const Vec3d &a, const Vec3d &vMin, const Vec3d &vMax) { return min(max(a, vMin), vMax); }
Vec3d clamp(const Vec3d &a, double vMin, double vMax)             { return min(max(a, vMin), vMax); }
Vec3d mix(const Vec3d &a, const Vec3d &b, const Vec3d &alpha) { return Vec3d(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y), mix(a.z, b.z, alpha.z)); }
Vec3d mix(const Vec3d &a, const Vec3d &b, double alpha)       { return Vec3d(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha),   mix(a.z, b.z, alpha)); }
Vec3d step(const Vec3d &e, const Vec3d &a) { return Vec3d(step(e.x, a.x), step(e.y, a.y), step(e.z, a.z)); }
Vec3d step(double e, const Vec3d &a) { return Vec3d(step(e, a.x), step(e, a.y), step(e, a.z)); }

double min(const Vec3d &a) { return fmin(fmin(a.x, a.y), a.z); }
double max(const Vec3d &a) { return fmax(fmax(a.x, a.y), a.z); }

/*********************************************************************************************/

void Vec3d::glVertex() const { glVertex3dv(v); }
void Vec3d::glNormal() const { glNormal3dv(v); }

/*********************************************************************************************/

Vec3d Vec3d::random() { return Vec3d(double(Random::unif()), double(Random::unif()), double(Random::unif())); }
Vec3d Vec3d::random(double min, double max) { return min + (max - min) * random(); }
Vec3d Vec3d::random(Vec3d  min, Vec3d  max) { return min + (max - min) * random(); }

Vec3d Vec3d::randomDir() {
    const double theta = 2 * M_PI * double(Random::unif());
    const double z = 2 * Random::unif() - 1.0;
    const double r = sqrt(1 - z*z);
    return Vec3d(r * cos(theta), r * sin(theta), z);
    }

/*********************************************************************************************/

QDomElement Vec3d::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    res.setAttribute("x2", QString::number(v[2]));
    return res;
    }

void Vec3d::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toDouble();
    v[1] = element.attribute("x1", "0.0").toDouble();
    v[2] = element.attribute("x2", "0.0").toDouble();
    }

BABLIB_NAMESPACE_END

