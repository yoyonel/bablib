#include <OpenGL.h>
#include "Vec3.h"
#include "Vec4.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

Vec4::Vec4() : x(0), y(0), z(0), w(1) {}
Vec4::Vec4(float c) : x(c), y(c), z(c), w(c) {}
Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Vec4::Vec4(const float *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
Vec4::Vec4(Vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

/*********************************************************************************************/

Vec3    Vec4::toVec3()    const { return Vec3(xn(), yn(), zn()); }
QString Vec4::toQString() const { return QString("(%1, %2, %3, %4)").arg(x).arg(y).arg(z).arg(w); }

/*********************************************************************************************/

void Vec4::glVertex() const { glVertex4fv(v); }

void Vec4::homogenize() {
    x /= w;
    y /= w;
    z /= w;
    w = 1;
    }

/*********************************************************************************************/

Vec4 dir(const Vec4 &a, const Vec4 &b) {
    Vec4 d(
        a.w * b.x - b.w * a.x,
        a.w * b.y - b.w * a.y,
        a.w * b.z - b.w * a.z,
        0);
    return ((a.w < 0.0) != (b.w < 0.0)) ? -d : d;
    /*
    return Vec4(
        a.w * b.x - b.w * a.x,
        a.w * b.y - b.w * a.y,
        a.w * b.z - b.w * a.z,
        0);
    */
    }

/*********************************************************************************************/

Vec4 operator-(const Vec4 &a) { return Vec4(-a.x, -a.y, -a.z, -a.w); }
    
/*********************************************************************************************/
    
Vec4 operator+(const Vec4 &a, const Vec4 &b) { return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
Vec4 operator-(const Vec4 &a, const Vec4 &b) { return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
Vec4 operator*(const Vec4 &a, const Vec4 &b) { return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
Vec4 operator/(const Vec4 &a, const Vec4 &b) { return Vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

/*********************************************************************************************/

Vec4 operator*(float s, const Vec4 &a) { return Vec4(s * a.x, s * a.y, s * a.z, s * a.w); }
Vec4 operator*(const Vec4 &a, float s) { return s * a; }
Vec4 operator/(const Vec4 &a, float s) { return Vec4(a.x / s, a.y / s, a.z / s, a.w / s); }
Vec4 operator/(float s, const Vec4 &a) { return Vec4(s / a.x, s / a.y, s / a.z, s / a.w); }

/*********************************************************************************************/

Vec4& Vec4::operator+=(const Vec4 &a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
Vec4& Vec4::operator-=(const Vec4 &a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
Vec4& Vec4::operator*=(const Vec4 &a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
Vec4& Vec4::operator/=(const Vec4 &a) { x/=a.x; y/=a.y; z/=a.z; w/=a.w; return *this; }
Vec4& Vec4::operator*=(float s) { x*=s; y*=s; z*=s; w*=s; return *this; }
Vec4& Vec4::operator/=(float s) { x/=s; y/=s; z/=s; w/=s; return *this; }

/*********************************************************************************************/
/*
#define EPS 1e-10
bool operator==(const Vec4 &a, const Vec4 &b) { return fabsf(b.x - a.x) < EPS && fabsf(b.y - a.y) < EPS && fabsf(b.z - a.z) < EPS && fabsf(b.w - a.w) < EPS;}
bool operator!=(const Vec4 &a, const Vec4 &b) { return fabsf(b.x - a.x) > EPS && fabsf(b.y - a.y) > EPS && fabsf(b.z - a.z) > EPS && fabsf(b.w - a.w) > EPS;}
*/
bool operator==(const Vec4 &a, const Vec4 &b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
bool operator!=(const Vec4 &a, const Vec4 &b) { return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w; }
bool operator>=(const Vec4 &a, const Vec4 &b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z && a.w >= b.w; }
bool operator<=(const Vec4 &a, const Vec4 &b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z && a.w <= b.w; }
bool operator> (const Vec4 &a, const Vec4 &b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z && a.w >  b.w; }
bool operator< (const Vec4 &a, const Vec4 &b) { return a.x <  b.x && a.y <  b.y && a.z <  b.z && a.w <  b.w; }
bool operator>=(const Vec4 &a, float  b) { return a.x >= b && a.y >= b && a.z >= b && a.w >= b; }
bool operator<=(const Vec4 &a, float  b) { return a.x <= b && a.y <= b && a.z <= b && a.w <= b; }
bool operator> (const Vec4 &a, float  b) { return a.x >  b && a.y >  b && a.z >  b && a.w >  b; }
bool operator< (const Vec4 &a, float  b) { return a.x <  b && a.y <  b && a.z <  b && a.w <  b; }

/*********************************************************************************************/

// dÃÂ©finition de fonctions de base :
namespace {
    float sign(float a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    float fract(float a) { return a - floorf(a); }
    float step(float e, float x) { return x < e ? 0 : 1; }
    float mix(float x, float y, float a) { return (1 - a) * x + a * y; }
    }
    
Vec4 abs(const Vec4 &a)   { return Vec4(fabsf(a.x),  fabsf(a.y),  fabsf(a.z),  fabsf(a.w));  }
Vec4 sign(const Vec4 &a)  { return Vec4(sign(a.x),   sign(a.y),   sign(a.z),   sign(a.w));   }
Vec4 floor(const Vec4 &a) { return Vec4(floorf(a.x), floorf(a.y), floorf(a.z), floorf(a.w)); }
Vec4 ceil(const Vec4 &a)  { return Vec4(ceilf(a.x),  ceilf(a.y),  ceilf(a.z),  ceilf(a.w));  }
Vec4 fract(const Vec4 &a) { return Vec4(fract(a.x),  fract(a.y),  fract(a.z),  fract(a.w));  }
                                                                                        
Vec4 min(const Vec4 &a, const Vec4 &b) { return Vec4(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w)); }
Vec4 min(const Vec4 &a, float b)       { return Vec4(fminf(a.x, b),   fminf(a.y, b),   fminf(a.z, b),   fminf(a.w, b));   }
Vec4 max(const Vec4 &a, const Vec4 &b) { return Vec4(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w)); }
Vec4 max(const Vec4 &a, float b)       { return Vec4(fmaxf(a.x, b),   fmaxf(a.y, b),   fmaxf(a.z, b),   fmaxf(a.w, b));   }
Vec4 clamp(const Vec4 &a, const Vec4 &vMin, const Vec4 &vMax) { return min(max(a, vMin), vMax); }
Vec4 clamp(const Vec4 &a, float vMin, float vMax)             { return min(max(a, vMin), vMax); }
Vec4 mix(const Vec4 &a, const Vec4 &b, const Vec4 &alpha) { return Vec4(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y), mix(a.z, b.z, alpha.z), mix(a.w, b.w, alpha.w)); }
Vec4 mix(const Vec4 &a, const Vec4 &b, float alpha)       { return Vec4(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha),   mix(a.z, b.z, alpha),   mix(a.w, b.w, alpha)); }
Vec4 step(const Vec4 &e, const Vec4 &a) { return Vec4(step(e.x, a.x), step(e.y, a.y), step(e.z, a.z), step(e.w, a.w)); }
Vec4 step(float e, const Vec4 &a) { return Vec4(step(e, a.x), step(e, a.y), step(e, a.z), step(e, a.w)); }

float min(const Vec4 &a) { return fminf(fminf(a.x, a.y), fminf(a.z, a.w)); }
float max(const Vec4 &a) { return fmaxf(fmaxf(a.x, a.y), fmaxf(a.z, a.w)); }

/*********************************************************************************************/

Vec4 Vec4::random() { return Vec4(Random::unif(), Random::unif(), Random::unif(), Random::unif()); }
Vec4 Vec4::random(float min, float max) { return min + (max - min) * random(); }
Vec4 Vec4::random(Vec4  min, Vec4  max) { return min + (max - min) * random(); }

/*********************************************************************************************/
// lecture / ÃÂ©criture XML :

Vec4::Vec4(const QDomElement &element) {
    initFromDOMElement(element);
    }

QDomElement Vec4::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    res.setAttribute("x2", QString::number(v[2]));
    res.setAttribute("x3", QString::number(v[3]));
    return res;
    }

void Vec4::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toFloat();
    v[1] = element.attribute("x1", "0.0").toFloat();
    v[2] = element.attribute("x2", "0.0").toFloat();
    v[3] = element.attribute("x3", "1.0").toFloat();
    }

BABLIB_NAMESPACE_END

