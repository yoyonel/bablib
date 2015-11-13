#include <OpenGL.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Random.h"
#include <math.h>
BABLIB_NAMESPACE_BEGIN

Vec2::Vec2(float x, float y) : x(x), y(y) {}
Vec2::Vec2(float c) : x(c), y(c) {}
Vec2::Vec2(const float *v) : x(v[0]), y(v[1]) {}
Vec2::Vec2(const QDomElement &element) { initFromDOMElement(element); }

/*********************************************************************************************/

QString Vec2::toQString() const { return QString("(%1, %2)").arg(x).arg(y); }

/*********************************************************************************************/

float Vec2::norm2()      const { return x*x + y*y; }
float Vec2::norm()       const { return sqrt(norm2()); }
Vec2  Vec2::normalized() const { return (*this) / norm(); }
void  Vec2::normalize()        { (*this) /= norm(); }

Vec3  vec(const Vec2 &a, const Vec2 &b)  { return Vec3(0, 0, a.x * b.y - a.y * b.x); }
float vecz(const Vec2 &a, const Vec2 &b) { return a.x * b.y - a.y * b.x; }
float dot(const Vec2 &a, const Vec2 &b)  { return a.x * b.x + a.y * b.y; }

float dist(Vec2 v1, Vec2 v2) { return (v2 - v1).norm(); }

float Vec2::arg()   const { return atan2(y, x); }
Vec2  Vec2::polar() const { return Vec2(norm(), arg()); }

Vec3  operator^(const Vec2 &a, const Vec2 &b) { return vec(a,b); }
float operator|(const Vec2 &a, const Vec2 &b) { return dot(a,b); }

Vec2 Vec2::ortho() const { return Vec2(-y, x); }
        
Vec2 Vec2::dir(float theta) { return Vec2(cosf(theta), sinf(theta)); }
        
/*********************************************************************************************/

Vec2 operator-(const Vec2 &a) { return Vec2(-a.x, -a.y); }
    
/*********************************************************************************************/
    
Vec2 operator+(const Vec2 &a, const Vec2 &b) { return Vec2(a.x + b.x, a.y + b.y); }
Vec2 operator-(const Vec2 &a, const Vec2 &b) { return Vec2(a.x - b.x, a.y - b.y); }
Vec2 operator*(const Vec2 &a, const Vec2 &b) { return Vec2(a.x * b.x, a.y * b.y); }
Vec2 operator/(const Vec2 &a, const Vec2 &b) { return Vec2(a.x / b.x, a.y / b.y); }

/*********************************************************************************************/

Vec2 operator*(float s, const Vec2 &a) { return Vec2(s * a.x, s * a.y); }
Vec2 operator*(const Vec2 &a, float s) { return s * a; }
Vec2 operator/(const Vec2 &a, float s) { return Vec2(a.x / s, a.y / s); }
Vec2 operator/(float s, const Vec2 &a) { return Vec2(s / a.x, s / a.y); }

/*********************************************************************************************/

Vec2& Vec2::operator+=(const Vec2 &a) { x+=a.x; y+=a.y; return *this; }
Vec2& Vec2::operator-=(const Vec2 &a) { x-=a.x; y-=a.y; return *this; }
Vec2& Vec2::operator*=(const Vec2 &a) { x*=a.x; y*=a.y; return *this; }
Vec2& Vec2::operator/=(const Vec2 &a) { x/=a.x; y/=a.y; return *this; }
Vec2& Vec2::operator*=(float s) { x*=s; y*=s; return *this; }
Vec2& Vec2::operator/=(float s) { x/=s; y/=s; return *this; }

/*********************************************************************************************/
/*
#define EPS 1e-10
bool operator==(const Vec2 &a, const Vec2 &b) { return fabsf(b.x - a.x) < EPS && fabsf(b.y - a.y) < EPS;}
bool operator!=(const Vec2 &a, const Vec2 &b) { return fabsf(b.x - a.x) > EPS && fabsf(b.y - a.y) > EPS;}
*/
bool operator==(const Vec2 &a, const Vec2 &b) { return a.x == b.x && a.y == b.y;}
bool operator!=(const Vec2 &a, const Vec2 &b) { return a.x != b.x || a.y != b.y;}
bool operator>=(const Vec2 &a, const Vec2 &b) { return a.x >= b.x && a.y >= b.y; }
bool operator<=(const Vec2 &a, const Vec2 &b) { return a.x <= b.x && a.y <= b.y; }
bool operator> (const Vec2 &a, const Vec2 &b) { return a.x >  b.x && a.y >  b.y; }
bool operator< (const Vec2 &a, const Vec2 &b) { return a.x <  b.x && a.y <  b.y; }
bool operator>=(const Vec2 &a, float  b) { return a.x >= b && a.y >= b; }
bool operator<=(const Vec2 &a, float  b) { return a.x <= b && a.y <= b; }
bool operator> (const Vec2 &a, float  b) { return a.x >  b && a.y >  b; }
bool operator< (const Vec2 &a, float  b) { return a.x <  b && a.y <  b; }

/*********************************************************************************************/

// dÃÂ©finition de fonctions de base :
namespace {
    float sign(float a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
    float fract(float a) { return a - floorf(a); }
    float step(float e, float x) { return x < e ? 0 : 1; }
    float mix(float x, float y, float a) { return (1 - a) * x + a * y; }
    }
    
Vec2 abs(const Vec2 &a)   { return Vec2(fabsf(a.x),  fabsf(a.y));  }
Vec2 sign(const Vec2 &a)  { return Vec2(sign(a.x),   sign(a.y));   }
Vec2 floor(const Vec2 &a) { return Vec2(floorf(a.x), floorf(a.y)); }
Vec2 ceil(const Vec2 &a)  { return Vec2(ceilf(a.x),  ceilf(a.y));  }
Vec2 fract(const Vec2 &a) { return Vec2(fract(a.x),  fract(a.y));  }

Vec2 min(const Vec2 &a, const Vec2 &b) { return Vec2(fminf(a.x, b.x), fminf(a.y, b.y)); }
Vec2 min(const Vec2 &a, float b)       { return Vec2(fminf(a.x, b),   fminf(a.y, b));   }
Vec2 max(const Vec2 &a, const Vec2 &b) { return Vec2(fmaxf(a.x, b.x), fmaxf(a.y, b.y)); }
Vec2 max(const Vec2 &a, float b)       { return Vec2(fmaxf(a.x, b),   fmaxf(a.y, b));   }
Vec2 clamp(const Vec2 &a, const Vec2 &vMin, const Vec2 &vMax) { return min(max(a, vMin), vMax); }
Vec2 clamp(const Vec2 &a, float vMin, float vMax)             { return min(max(a, vMin), vMax); }
Vec2 mix(const Vec2 &a, const Vec2 &b, const Vec2 &alpha) { return Vec2(mix(a.x, b.x, alpha.x), mix(a.y, b.y, alpha.y)); }
Vec2 mix(const Vec2 &a, const Vec2 &b, float alpha)       { return Vec2(mix(a.x, b.x, alpha),   mix(a.y, b.y, alpha));   }
Vec2 step(const Vec2 &e, const Vec2 &a) { return Vec2(step(e.x, a.x), step(e.y, a.y)); }
Vec2 step(float e, const Vec2 &a) { return Vec2(step(e, a.x), step(e, a.y)); }

float min(const Vec2 &a) { return fminf(a.x, a.y); }
float max(const Vec2 &a) { return fmaxf(a.x, a.y); }

/*********************************************************************************************/

void Vec2::glVertex() const { glVertex2fv(v); }

/*********************************************************************************************/

Vec2 Vec2::random() { return Vec2(Random::unif(), Random::unif()); }
Vec2 Vec2::random(float min, float max) { return min + (max - min) * random(); }
Vec2 Vec2::random(Vec2  min, Vec2  max) { return min + (max - min) * random(); }

/*********************************************************************************************/

QDomElement Vec2::domElement(const QString &name, QDomDocument &document) const {
    QDomElement res = document.createElement(name);
    res.setAttribute("x0", QString::number(v[0]));
    res.setAttribute("x1", QString::number(v[1]));
    return res;
    }

void Vec2::initFromDOMElement(const QDomElement &element) {
    v[0] = element.attribute("x0", "0.0").toFloat();
    v[1] = element.attribute("x1", "0.0").toFloat();
    }

BABLIB_NAMESPACE_END

