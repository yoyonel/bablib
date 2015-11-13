#include <OpenGL.h>
#include "Vec3.h"
#include "Vec2.h"
#include "Vec4.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

#ifdef NO_INLINE_FUNCTIONS
Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
Vec3::Vec3(float c) : x(c), y(c), z(c) {}
Vec3::Vec3(Vec2 v, float z) : x(v.x), y(v.y), z(z) {}
Vec3::Vec3(float x, Vec2 yz) : x(x), y(yz.x), z(yz.y) {}
Vec3::Vec3(Vec4 vec) : x(vec.xn()), y(vec.yn()), z(vec.zn()) {}
Vec3::Vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}
Vec3::Vec3(const QDomElement &element) { initFromDOMElement(element); }
#endif

/*********************************************************************************************/

QString Vec3::toQString() const { return QString("(%1, %2, %3)").arg(x).arg(y).arg(z); }
Vec4    Vec3::toVec4()    const { return Vec4(x, y, z, 1); }

/*********************************************************************************************/

#ifdef NO_INLINE_FUNCTIONS
float Vec3::norm2()      const { return x*x + y*y + z*z; }
float Vec3::norm()       const { return sqrtf(norm2()); }
Vec3  Vec3::normalized() const { return (*this) / norm(); }
void  Vec3::normalize()        { (*this) /= norm(); }

Vec3 vec(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
        );
    }

float dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

float dist(Vec3 v1, Vec3 v2) { return (v2 - v1).norm(); }

Vec3 triangleNormal(const Vec3 &a, const Vec3 &b, const Vec3 &c) { return ((b - a) ^ (c - a)).normalized(); }
#endif


Vec3 Vec3::ortho() const {
    if ((fabsf(y) >= 0.9f * fabsf(x)) && (fabsf(z) >= 0.9f * fabsf(x)))
        return Vec3(0, -z, y);
    else if ((fabsf(x) >= 0.9f * fabsf(y)) && (fabsf(z) >= 0.9f * fabsf(y)))
        return Vec3(-z, 0, x);
    else
        return Vec3(-y, x, 0);
    }
        
#ifdef NO_INLINE_FUNCTIONS
/*********************************************************************************************/

Vec3 operator-(const Vec3 &a) { return Vec3(-a.x, -a.y, -a.z); }
    
/*********************************************************************************************/
    
Vec3 operator+(const Vec3 &a, const Vec3 &b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
Vec3 operator-(const Vec3 &a, const Vec3 &b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
Vec3 operator*(const Vec3 &a, const Vec3 &b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
Vec3 operator/(const Vec3 &a, const Vec3 &b) { return Vec3(a.x / b.x, a.y / b.y, a.z / b.z); }

/*********************************************************************************************/

Vec3 operator*(float s, const Vec3 &a) { return Vec3(s * a.x, s * a.y, s * a.z); }
Vec3 operator*(const Vec3 &a, float s) { return s * a; }
Vec3 operator/(const Vec3 &a, float s) { return Vec3(a.x / s, a.y / s, a.z / s); }
Vec3 operator/(float s, const Vec3 &a) { return Vec3(s / a.x, s / a.y, s / a.z); }

/*********************************************************************************************/

Vec3& Vec3::operator+=(const Vec3 &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
Vec3& Vec3::operator-=(const Vec3 &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
Vec3& Vec3::operator*=(const Vec3 &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
Vec3& Vec3::operator/=(const Vec3 &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
Vec3& Vec3::operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
Vec3& Vec3::operator/=(float s) { x/=s; y/=s; z/=s; return *this; }

/*********************************************************************************************/
/*
#define EPS 1e-10
bool operator==(const Vec3 &a, const Vec3 &b) { return fabsf(b.x - a.x) < EPS && fabsf(b.y - a.y) < EPS && fabsf(b.z - a.z) < EPS;}
bool operator!=(const Vec3 &a, const Vec3 &b) { return fabsf(b.x - a.x) > EPS && fabsf(b.y - a.y) > EPS && fabsf(b.z - a.z) > EPS;}
*/
bool operator==(const Vec3 &a, const Vec3 &b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
bool operator!=(const Vec3 &a, const Vec3 &b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
bool operator>=(const Vec3 &a, const Vec3 &b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z; }
bool operator<=(const Vec3 &a, const Vec3 &b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z; }
bool operator> (const Vec3 &a, const Vec3 &b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z; }
bool operator< (const Vec3 &a, const Vec3 &b) { return a.x <  b.x && a.y <  b.y && a.z <  b.z; }
bool operator>=(const Vec3 &a, float  b) { return a.x >= b && a.y >= b && a.z >= b; }
bool operator<=(const Vec3 &a, float  b) { return a.x <= b && a.y <= b && a.z <= b; }
bool operator> (const Vec3 &a, float  b) { return a.x >  b && a.y >  b && a.z >  b; }
bool operator< (const Vec3 &a, float  b) { return a.x <  b && a.y <  b && a.z <  b; }

/*********************************************************************************************/
#endif

// définition de fonctions de base :
namespace {
    float sign(float a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    float fract(float a) { return a - floorf(a); }
    float step(float e, float x) { return x < e ? 0 : 1; }
    float mix(float x, float y, float a) { return (1 - a) * x + a * y; }
    }
    
Vec3 abs(const Vec3 &a)   { return Vec3(fabsf(a.x),  fabsf(a.y),  fabsf(a.z));  }
Vec3 sign(const Vec3 &a)  { return Vec3(sign(a.x),   sign(a.y),   sign(a.z));   }
Vec3 floor(const Vec3 &a) { return Vec3(floorf(a.x), floorf(a.y), floorf(a.z)); }
Vec3 ceil(const Vec3 &a)  { return Vec3(ceilf(a.x),  ceilf(a.y),  ceilf(a.z));  }
Vec3 fract(const Vec3 &a) { return Vec3(fract(a.x),  fract(a.y),  fract(a.z));  }

Vec3 min(const Vec3 &a, const Vec3 &b) { return Vec3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)); }
Vec3 min(const Vec3 &a, float b)       { return Vec3(fminf(a.x, b),   fminf(a.y, b),   fminf(a.z, b));   }
Vec3 max(const Vec3 &a, const Vec3 &b) { return Vec3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); }
Vec3 max(const Vec3 &a, float b)       { return Vec3(fmaxf(a.x, b),   fmaxf(a.y, b),   fmaxf(a.z, b));   }
Vec3 clamp(const Vec3 &a, const Vec3 &vMin, const Vec3 &vMax) { return min(max(a, vMin), vMax); }
Vec3 clamp(const Vec3 &a, float vMin, float vMax)             { return min(max(a, vMin), vMax); }
Vec3 mix(const Vec3 &a, const Vec3 &b, const Vec3 &alpha) { return Vec3(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y), mix(a.z, b.z, alpha.z)); }
Vec3 mix(const Vec3 &a, const Vec3 &b, float alpha)       { return Vec3(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha),   mix(a.z, b.z, alpha)); }
Vec3 step(const Vec3 &e, const Vec3 &a) { return Vec3(step(e.x, a.x), step(e.y, a.y), step(e.z, a.z)); }
Vec3 step(float e, const Vec3 &a) { return Vec3(step(e, a.x), step(e, a.y), step(e, a.z)); }

float min(const Vec3 &a) { return fminf(fminf(a.x, a.y), a.z); }
float max(const Vec3 &a) { return fmaxf(fmaxf(a.x, a.y), a.z); }

/*********************************************************************************************/

void Vec3::glVertex() const { glVertex3fv(v); }
void Vec3::glNormal() const { glNormal3fv(v); }

/*********************************************************************************************/

Vec3 Vec3::random() { return Vec3(Random::unif(), Random::unif(), Random::unif()); }
Vec3 Vec3::random(float min, float max) { return min + (max - min) * random(); }
Vec3 Vec3::random(Vec3  min, Vec3  max) { return min + (max - min) * random(); }

Vec3 Vec3::randomDir() {
    const float theta = 2 * (float)M_PI * Random::unif();
    const float z = 2 * Random::unif() - 1.0f;
    const float r = sqrtf(1 - z*z);
    return Vec3(r * (float)cosf(theta), r * (float)sinf(theta), z);
    }

/*********************************************************************************************/

QDomElement Vec3::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    res.setAttribute("x2", QString::number(v[2]));
    return res;
    }

void Vec3::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toFloat();
    v[1] = element.attribute("x1", "0.0").toFloat();
    v[2] = element.attribute("x2", "0.0").toFloat();
    }

BABLIB_NAMESPACE_END

