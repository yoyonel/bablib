#include <OpenGL.h>
#include "Vec2d.h"
#include "Vec3d.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

Vec2d::Vec2d(double x, double y) : x(x), y(y) {}
Vec2d::Vec2d(double c) : x(c), y(c) {}
Vec2d::Vec2d(const Vec2 &v) : x(v.x), y(v.y) {}
Vec2d::Vec2d(const double *v) : x(v[0]), y(v[1]) {}
Vec2d::Vec2d(const QDomElement &element) { initFromDOMElement(element); }

/*********************************************************************************************/

QString Vec2d::toQString() const { return QString("(%1, %2)").arg(x).arg(y); }

Vec2 Vec2d::toVec2f() const { return Vec2(static_cast<float>(x), static_cast<float>(y)); }
        
/*********************************************************************************************/

double Vec2d::norm2()      const { return x*x + y*y; }
double Vec2d::norm()       const { return sqrt(norm2()); }
Vec2d  Vec2d::normalized() const { return (*this) / norm(); }
void  Vec2d::normalize()         { (*this) /= norm(); }

Vec3d  vec(const Vec2d &a, const Vec2d &b)  { return Vec3d(0, 0, a.x * b.y - a.y * b.x); }
double vecz(const Vec2d &a, const Vec2d &b) { return a.x * b.y - a.y * b.x; }
double dot(const Vec2d &a, const Vec2d &b)  { return a.x * b.x + a.y * b.y; }

double dist(Vec2d v1, Vec2d v2) { return (v2 - v1).norm(); }

double Vec2d::arg()   const { return atan2(y, x); }
Vec2d  Vec2d::polar() const { return Vec2d(norm(), arg()); }

Vec2d Vec2d::ortho() const { return Vec2d(-y, x); }
        
Vec3d  operator^(const Vec2d &a, const Vec2d &b) { return vec(a,b); }
double operator|(const Vec2d &a, const Vec2d &b) { return dot(a,b); }

Vec2d Vec2d::dir(double theta) { return Vec2d(cos(theta), sin(theta)); }
        
/*********************************************************************************************/

Vec2d operator-(const Vec2d &a) { return Vec2d(-a.x, -a.y); }
    
/*********************************************************************************************/
    
Vec2d operator+(const Vec2d &a, const Vec2d &b) { return Vec2d(a.x + b.x, a.y + b.y); }
Vec2d operator-(const Vec2d &a, const Vec2d &b) { return Vec2d(a.x - b.x, a.y - b.y); }
Vec2d operator*(const Vec2d &a, const Vec2d &b) { return Vec2d(a.x * b.x, a.y * b.y); }
Vec2d operator/(const Vec2d &a, const Vec2d &b) { return Vec2d(a.x / b.x, a.y / b.y); }

/*********************************************************************************************/

Vec2d operator*(double s, const Vec2d &a) { return Vec2d(s * a.x, s * a.y); }
Vec2d operator*(const Vec2d &a, double s) { return s * a; }
Vec2d operator/(const Vec2d &a, double s) { return Vec2d(a.x / s, a.y / s); }
Vec2d operator/(double s, const Vec2d &a) { return Vec2d(s / a.x, s / a.y); }

/*********************************************************************************************/

Vec2d& Vec2d::operator+=(const Vec2d &a) { x+=a.x; y+=a.y; return *this; }
Vec2d& Vec2d::operator-=(const Vec2d &a) { x-=a.x; y-=a.y; return *this; }
Vec2d& Vec2d::operator*=(const Vec2d &a) { x*=a.x; y*=a.y; return *this; }
Vec2d& Vec2d::operator/=(const Vec2d &a) { x/=a.x; y/=a.y; return *this; }
Vec2d& Vec2d::operator*=(double s) { x*=s; y*=s; return *this; }
Vec2d& Vec2d::operator/=(double s) { x/=s; y/=s; return *this; }

/*********************************************************************************************/
/*
#define EPS 1e-10
bool operator==(const Vec2d &a, const Vec2d &b) { return abs(b.x - a.x) < EPS && abs(b.y - a.y) < EPS;}
bool operator!=(const Vec2d &a, const Vec2d &b) { return abs(b.x - a.x) > EPS && abs(b.y - a.y) > EPS;}
*/
bool operator==(const Vec2d &a, const Vec2d &b) { return a.x == b.x && a.y == b.y;}
bool operator!=(const Vec2d &a, const Vec2d &b) { return a.x != b.x || a.y != b.y;}
bool operator>=(const Vec2d &a, const Vec2d &b) { return a.x >= b.x && a.y >= b.y; }
bool operator<=(const Vec2d &a, const Vec2d &b) { return a.x <= b.x && a.y <= b.y; }
bool operator> (const Vec2d &a, const Vec2d &b) { return a.x >  b.x && a.y >  b.y; }
bool operator< (const Vec2d &a, const Vec2d &b) { return a.x <  b.x && a.y <  b.y; }
bool operator>=(const Vec2d &a, double  b) { return a.x >= b && a.y >= b; }
bool operator<=(const Vec2d &a, double  b) { return a.x <= b && a.y <= b; }
bool operator> (const Vec2d &a, double  b) { return a.x >  b && a.y >  b; }
bool operator< (const Vec2d &a, double  b) { return a.x <  b && a.y <  b; }

/*********************************************************************************************/

// définition de fonctions de base :
namespace {
    double sign(double a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    double fract(double a) { return a - floor(a); }
    double step(double e, double x) { return x < e ? 0 : 1; }
    double mix(double x, double y, double a) { return (1 - a) * x + a * y; }
    }
    
Vec2d abs(const Vec2d &a)   { return Vec2d(fabs(a.x),   fabs(a.y));  }
Vec2d sign(const Vec2d &a)  { return Vec2d(sign(a.x),   sign(a.y));  }
Vec2d floor(const Vec2d &a) { return Vec2d(floor(a.x),  floor(a.y)); }
Vec2d ceil(const Vec2d &a)  { return Vec2d(ceil(a.x),   ceil(a.y));  }
Vec2d fract(const Vec2d &a) { return Vec2d(fract(a.x),  fract(a.y)); }

Vec2d min(const Vec2d &a, const Vec2d &b) { return Vec2d(fmin(a.x, b.x), fmin(a.y, b.y)); }
Vec2d min(const Vec2d &a, double b)       { return Vec2d(fmin(a.x, b),   fmin(a.y, b));   }
Vec2d max(const Vec2d &a, const Vec2d &b) { return Vec2d(fmax(a.x, b.x), fmax(a.y, b.y)); }
Vec2d max(const Vec2d &a, double b)       { return Vec2d(fmax(a.x, b),   fmax(a.y, b));   }
Vec2d clamp(const Vec2d &a, const Vec2d &vMin, const Vec2d &vMax) { return min(max(a, vMin), vMax); }
Vec2d clamp(const Vec2d &a, double vMin, double vMax)             { return min(max(a, vMin), vMax); }
Vec2d mix(const Vec2d &a, const Vec2d &b, const Vec2d &alpha) { return Vec2d(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y)); }
Vec2d mix(const Vec2d &a, const Vec2d &b, double alpha)       { return Vec2d(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha));   }
Vec2d step(const Vec2d &e, const Vec2d &a) { return Vec2d(step(e.x, a.x), step(e.y, a.y)); }
Vec2d step(double e, const Vec2d &a) { return Vec2d(step(e, a.x), step(e, a.y)); }

double min(const Vec2d &a) { return fmin(a.x, a.y); }
double max(const Vec2d &a) { return fmax(a.x, a.y); }

/*********************************************************************************************/

void Vec2d::glVertex() const { glVertex2dv(v); }

/*********************************************************************************************/

Vec2d Vec2d::random() { return Vec2d(double(Random::unif()), double(Random::unif())); }
Vec2d Vec2d::random(double min, double max) { return min + (max - min) * random(); }
Vec2d Vec2d::random(Vec2d  min, Vec2d  max) { return min + (max - min) * random(); }

/*********************************************************************************************/

QDomElement Vec2d::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    return res;
    }

void Vec2d::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toDouble();
    v[1] = element.attribute("x1", "0.0").toDouble();
    }

BABLIB_NAMESPACE_END

