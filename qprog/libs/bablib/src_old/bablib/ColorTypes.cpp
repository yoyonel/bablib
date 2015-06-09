#include <OpenGL.h>
#include "ColorTypes.h"
BABLIB_NAMESPACE_BEGIN

namespace {
    float toFloat(int c) {return c / 255.0f;}
    int toInt(float v) {return (int)(255 * v);}

    GLubyte mul(float a, GLubyte b) { return static_cast<GLubyte>(a * static_cast<float>(b)); }
    GLubyte div(float a, GLubyte b) { return static_cast<GLubyte>(a / static_cast<float>(b)); }
    GLubyte div(GLubyte a, float b) { return static_cast<GLubyte>(static_cast<float>(a) / b); }
    GLubyte toUByte(float c) { return static_cast<GLubyte>(255 * c); }

    GLuint mul(float a, GLuint b) { return static_cast<GLuint>(a * static_cast<float>(b)); }
    GLuint div(float a, GLuint b) { return static_cast<GLuint>(a / static_cast<float>(b)); }
    GLuint div(GLuint a, float b) { return static_cast<GLuint>(static_cast<float>(a) / b); }
    GLuint toUInt(float c) { return static_cast<GLuint>(255 * c); }
    }

/*********************************************************************************************/
// Float1 :

Float1::Float1(float x) : x(x) {}
Float1::Float1(const float *c) : x(c[0]) {}
Float1::Float1(QRgb c) : r(toFloat(qRed(c))) {}
QRgb Float1::toQRgb() const { return qRgba(toInt(r), toInt(r), toInt(r), toInt(1.0f)); }

Float1 operator-(const Float1 &a) {return Float1(-a.x);}
    
Float1 operator+(const Float1 &a, const Float1 &b) { return Float1(a.x + b.x); }
Float1 operator-(const Float1 &a, const Float1 &b) { return Float1(a.x - b.x); }
Float1 operator*(const Float1 &a, const Float1 &b) { return Float1(a.x * b.x); }
Float1 operator/(const Float1 &a, const Float1 &b) { return Float1(a.x / b.x); }

Float1 operator*(float s, const Float1 &a) { return Float1(s * a.x); }
Float1 operator*(const Float1 &a, float s) { return s * a; }
Float1 operator/(const Float1 &a, float s) { return Float1(a.x / s); }
Float1 operator/(float s, const Float1 &a) { return Float1(s / a.x); }

Float1 Float1::operator+=(const Float1 &a) { x+=a.x; return *this; }
Float1 Float1::operator-=(const Float1 &a) { x-=a.x; return *this; }
Float1 Float1::operator*=(const Float1 &a) { x*=a.x; return *this; }
Float1 Float1::operator/=(const Float1 &a) { x/=a.x; return *this; }
Float1 Float1::operator*=(float s) { x*=s; return *this; }
Float1 Float1::operator/=(float s) { x/=s; return *this; }

void Float1::reset(float r, float, float, float) { this->r=r; }
void Float1::setGL() const { glColor3f(r,r,r); }
        
/*********************************************************************************************/
// Float2 :

Float2::Float2(float x, float y) : x(x), y(y) {}
Float2::Float2(const float *c) : x(c[0]), y(c[1]) {}
Float2::Float2(QRgb c) : r(toFloat(qRed(c))), g(toFloat(qGreen(c))) {}
QRgb Float2::toQRgb() const { return qRgba(toInt(r), toInt(g), toInt(0.0f), toInt(1.0f)); }

Float2 operator-(const Float2 &a) {return Float2(-a.x, -a.y);}
    
Float2 operator+(const Float2 &a, const Float2 &b) { return Float2(a.x + b.x, a.y + b.y); }
Float2 operator-(const Float2 &a, const Float2 &b) { return Float2(a.x - b.x, a.y - b.y); }
Float2 operator*(const Float2 &a, const Float2 &b) { return Float2(a.x * b.x, a.y * b.y); }
Float2 operator/(const Float2 &a, const Float2 &b) { return Float2(a.x / b.x, a.y / b.y); }

Float2 operator*(float s, const Float2 &a) { return Float2(s * a.x, s * a.y); }
Float2 operator*(const Float2 &a, float s) { return s * a; }
Float2 operator/(const Float2 &a, float s) { return Float2(a.x / s, a.y / s); }
Float2 operator/(float s, const Float2 &a) { return Float2(s / a.x, s / a.y); }

Float2 Float2::operator+=(const Float2 &a) { x+=a.x; y+=a.y; return *this; }
Float2 Float2::operator-=(const Float2 &a) { x-=a.x; y-=a.y; return *this; }
Float2 Float2::operator*=(const Float2 &a) { x*=a.x; y*=a.y; return *this; }
Float2 Float2::operator/=(const Float2 &a) { x/=a.x; y/=a.y; return *this; }
Float2 Float2::operator*=(float s) { x*=s; y*=s; return *this; }
Float2 Float2::operator/=(float s) { x/=s; y/=s; return *this; }

void Float2::reset(float r, float g, float, float) { this->r=r; this->g=g; }
void Float2::setGL() const { glColor3f(r,g,0); }

/*********************************************************************************************/
// Float3 :

Float3::Float3(float x, float y, float z) : x(x), y(y), z(z) {}
Float3::Float3(const float *c) : x(c[0]), y(c[1]), z(c[2]) {}
Float3::Float3(QRgb c) : r(toFloat(qRed(c))), g(toFloat(qGreen(c))), b(toFloat(qBlue(c))) {}
QRgb Float3::toQRgb() const { return qRgba(toInt(r), toInt(g), toInt(b), toInt(1.0f)); }

Float3 operator-(const Float3 &a) {return Float3(-a.x, -a.y, -a.z);}
    
Float3 operator+(const Float3 &a, const Float3 &b) { return Float3(a.x + b.x, a.y + b.y, a.z + b.z); }
Float3 operator-(const Float3 &a, const Float3 &b) { return Float3(a.x - b.x, a.y - b.y, a.z - b.z); }
Float3 operator*(const Float3 &a, const Float3 &b) { return Float3(a.x * b.x, a.y * b.y, a.z * b.z); }
Float3 operator/(const Float3 &a, const Float3 &b) { return Float3(a.x / b.x, a.y / b.y, a.z / b.z); }

Float3 operator*(float s, const Float3 &a) { return Float3(s * a.x, s * a.y, s * a.z); }
Float3 operator*(const Float3 &a, float s) { return s * a; }
Float3 operator/(const Float3 &a, float s) { return Float3(a.x / s, a.y / s, a.z / s); }
Float3 operator/(float s, const Float3 &a) { return Float3(s / a.x, s / a.y, s / a.z); }

Float3 Float3::operator+=(const Float3 &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
Float3 Float3::operator-=(const Float3 &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
Float3 Float3::operator*=(const Float3 &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
Float3 Float3::operator/=(const Float3 &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
Float3 Float3::operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
Float3 Float3::operator/=(float s) { x/=s; y/=s; z/=s; return *this; }

void Float3::reset(float r, float g, float b, float) { this->r=r; this->g=g; this->b=b; }
void Float3::setGL() const { glColor3f(r,g,b); }

/*********************************************************************************************/
// Float4 :

Float4::Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
Float4::Float4(const float *xy, const float *zw) : x(xy[0]), y(xy[1]), z(zw[0]), w(zw[1]) {}
Float4::Float4(const float *c) : x(c[0]), y(c[1]), z(c[2]), w(c[3]) {}
Float4::Float4(QRgb c) : r(toFloat(qRed(c))), g(toFloat(qGreen(c))), b(toFloat(qBlue(c))), a(toFloat(qAlpha(c))) {}
QRgb Float4::toQRgb() const { return qRgba(toInt(r), toInt(g), toInt(b), toInt(a)); }

Float4 operator-(const Float4 &a) {return Float4(-a.x, -a.y, -a.z, -a.w);}
    
Float4 operator+(const Float4 &a, const Float4 &b) { return Float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
Float4 operator-(const Float4 &a, const Float4 &b) { return Float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
Float4 operator*(const Float4 &a, const Float4 &b) { return Float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
Float4 operator/(const Float4 &a, const Float4 &b) { return Float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

Float4 operator*(float s, const Float4 &a) { return Float4(s * a.x, s * a.y, s * a.z, s * a.w); }
Float4 operator*(const Float4 &a, float s) { return s * a; }
Float4 operator/(const Float4 &a, float s) { return Float4(a.x / s, a.y / s, a.z / s, a.w / s); }
Float4 operator/(float s, const Float4 &a) { return Float4(s / a.x, s / a.y, s / a.z, s / a.w); }

Float4 Float4::operator+=(const Float4 &a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
Float4 Float4::operator-=(const Float4 &a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
Float4 Float4::operator*=(const Float4 &a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
Float4 Float4::operator/=(const Float4 &a) { x/=a.x; y/=a.y; z/=a.z; w/=a.w; return *this; }
Float4 Float4::operator*=(float s) { x*=s; y*=s; z*=s; w*=s; return *this; }
Float4 Float4::operator/=(float s) { x/=s; y/=s; z/=s; w/=s; return *this; }

void Float4::reset(float r, float g, float b, float a) { this->r=r; this->g=g; this->b=b; this->a=a; }
void Float4::setAlpha(float a) { this->a = a; }
void Float4::setGL() const { glColor4f(r,g,b,a); }

/*********************************************************************************************/
// UByte1 :

//UByte1::UByte1(GLubyte x) : x(x) {}
UByte1::UByte1(const GLubyte *c) : x(c[0]) {}
UByte1::UByte1(QRgb c) : r(qRed(c)) {}
QRgb UByte1::toQRgb() const { return qRgba(r,toInt(0.0f),toInt(0.0f),toInt(1.0f)); }

UByte1 operator+(const UByte1 &a, const UByte1 &b) { return UByte1(a.x + b.x); }
UByte1 operator-(const UByte1 &a, const UByte1 &b) { return UByte1(a.x - b.x); }
UByte1 operator*(const UByte1 &a, const UByte1 &b) { return UByte1(a.x * b.x); }
UByte1 operator/(const UByte1 &a, const UByte1 &b) { return UByte1(a.x / b.x); }

UByte1 operator*(float s, const UByte1 &a) { return UByte1(mul(s,a.x)); }
UByte1 operator*(const UByte1 &a, float s) { return s * a; }
UByte1 operator/(const UByte1 &a, float s) { return UByte1(div(a.x,s)); }
UByte1 operator/(float s, const UByte1 &a) { return UByte1(div(s,a.x)); }

UByte1 UByte1::operator+=(const UByte1 &a) { x+=a.x; return *this; }
UByte1 UByte1::operator-=(const UByte1 &a) { x-=a.x; return *this; }
UByte1 UByte1::operator*=(const UByte1 &a) { x*=a.x; return *this; }
UByte1 UByte1::operator/=(const UByte1 &a) { x/=a.x; return *this; }
UByte1 UByte1::operator*=(float s) { x=mul(s,x); return *this; }
UByte1 UByte1::operator/=(float s) { x=div(x,s); return *this; }

void UByte1::reset(float r, float , float , float) { this->r=toUByte(r); }

//UInt3 UByte1::toUInt3() const { return UInt3(r, g, b); }
//void UByte1::setGL() const { glColor3ub(r,g,b); }

/*********************************************************************************************/
// UByte3 :

UByte3::UByte3(GLubyte x, GLubyte y, GLubyte z) : x(x), y(y), z(z) {}
UByte3::UByte3(const GLubyte *c) : x(c[0]), y(c[1]), z(c[2]) {}
UByte3::UByte3(QRgb c) : r(qRed(c)), g(qGreen(c)), b(qBlue(c)) {}
QRgb UByte3::toQRgb() const { return qRgba(r,g,b,toInt(1.0f)); }

UByte3 operator+(const UByte3 &a, const UByte3 &b) { return UByte3(a.x + b.x, a.y + b.y, a.z + b.z); }
UByte3 operator-(const UByte3 &a, const UByte3 &b) { return UByte3(a.x - b.x, a.y - b.y, a.z - b.z); }
UByte3 operator*(const UByte3 &a, const UByte3 &b) { return UByte3(a.x * b.x, a.y * b.y, a.z * b.z); }
UByte3 operator/(const UByte3 &a, const UByte3 &b) { return UByte3(a.x / b.x, a.y / b.y, a.z / b.z); }

UByte3 operator*(float s, const UByte3 &a) { return UByte3(mul(s,a.x), mul(s,a.y), mul(s,a.z)); }
UByte3 operator*(const UByte3 &a, float s) { return s * a; }
UByte3 operator/(const UByte3 &a, float s) { return UByte3(div(a.x,s), div(a.y,s), div(a.z,s)); }
UByte3 operator/(float s, const UByte3 &a) { return UByte3(div(s,a.x), div(s,a.y), div(s,a.z)); }

UByte3 UByte3::operator+=(const UByte3 &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
UByte3 UByte3::operator-=(const UByte3 &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
UByte3 UByte3::operator*=(const UByte3 &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
UByte3 UByte3::operator/=(const UByte3 &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
UByte3 UByte3::operator*=(float s) { x=mul(s,x); y=mul(s,y); z=mul(s,z); return *this; }
UByte3 UByte3::operator/=(float s) { x=div(x,s); y=div(y,s); z=div(z,s); return *this; }

void UByte3::reset(float r, float g, float b, float) { this->r=toUByte(r); this->g=toUByte(g); this->b=toUByte(b); }

UInt3 UByte3::toUInt3() const { return UInt3(r, g, b); }
void UByte3::setGL() const { glColor3ub(r,g,b); }

/*********************************************************************************************/
// UByte4 :

UByte4::UByte4(GLubyte x, GLubyte y, GLubyte z, GLubyte w) : x(x), y(y), z(z), w(w) {}
UByte4::UByte4(const GLubyte *c) : x(c[0]), y(c[1]), z(c[2]), w(c[3]) {}
UByte4::UByte4(QRgb c) : r(qRed(c)), g(qGreen(c)), b(qBlue(c)), a(qAlpha(c)) {}
QRgb UByte4::toQRgb() const { return qRgba(r,g,b,a); }

UByte4 operator+(const UByte4 &a, const UByte4 &b) { return UByte4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
UByte4 operator-(const UByte4 &a, const UByte4 &b) { return UByte4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
UByte4 operator*(const UByte4 &a, const UByte4 &b) { return UByte4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
UByte4 operator/(const UByte4 &a, const UByte4 &b) { return UByte4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

UByte4 operator*(float s, const UByte4 &a) { return UByte4(mul(s,a.x), mul(s,a.y), mul(s,a.z), mul(s,a.w)); }
UByte4 operator*(const UByte4 &a, float s) { return s * a; }
UByte4 operator/(const UByte4 &a, float s) { return UByte4(div(a.x,s), div(a.y,s), div(a.z,s), div(a.w,s)); }
UByte4 operator/(float s, const UByte4 &a) { return UByte4(div(s,a.x), div(s,a.y), div(s,a.z), div(s,a.w)); }

UByte4 UByte4::operator+=(const UByte4 &a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
UByte4 UByte4::operator-=(const UByte4 &a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
UByte4 UByte4::operator*=(const UByte4 &a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
UByte4 UByte4::operator/=(const UByte4 &a) { x/=a.x; y/=a.y; z/=a.z; w/=a.w; return *this; }
UByte4 UByte4::operator*=(float s) { x=mul(s,x); y=mul(s,y); z=mul(s,z); w=mul(s,w); return *this; }
UByte4 UByte4::operator/=(float s) { x=div(x,s); y=div(y,s); z=div(z,s); w=div(w,s); return *this; }

void UByte4::reset(float r, float g, float b, float a) { this->r=toUByte(r); this->g=toUByte(g); this->b=toUByte(b); this->a=toUByte(a); }
void UByte4::setAlpha(float a) { this->a = toUByte(a); }

UInt4 UByte4::toUInt4() const { return UInt4(r, g, b, a); }
void UByte4::setGL() const { glColor4ub(r,g,b,a); }

/*********************************************************************************************/
// UInt1 :

UInt1::UInt1(GLuint x) : x(x) {}
UInt1::UInt1(const GLuint *c) : x(c[0]) {}
/*UInt1::UInt1(QRgb c) : r(qRed(c)) {}*/
QRgb UInt1::toQRgb() const { return qRgba(r,toInt(0.0f),toInt(0.0f),toInt(1.0f)); }

UInt1 operator+(const UInt1 &a, const UInt1 &b) { return UInt1(a.x + b.x); }
UInt1 operator-(const UInt1 &a, const UInt1 &b) { return UInt1(a.x - b.x); }
UInt1 operator*(const UInt1 &a, const UInt1 &b) { return UInt1(a.x * b.x); }
UInt1 operator/(const UInt1 &a, const UInt1 &b) { return UInt1(a.x / b.x); }

UInt1 operator*(float s, const UInt1 &a) { return UInt1(mul(s,a.x)); }
UInt1 operator*(const UInt1 &a, float s) { return s * a; }
UInt1 operator/(const UInt1 &a, float s) { return UInt1(div(a.x,s)); }
UInt1 operator/(float s, const UInt1 &a) { return UInt1(div(s,a.x)); }

UInt1 UInt1::operator+=(const UInt1 &a) { x+=a.x; return *this; }
UInt1 UInt1::operator-=(const UInt1 &a) { x-=a.x; return *this; }
UInt1 UInt1::operator*=(const UInt1 &a) { x*=a.x; return *this; }
UInt1 UInt1::operator/=(const UInt1 &a) { x/=a.x; return *this; }
UInt1 UInt1::operator*=(float s) { x=mul(s,x); return *this; }
UInt1 UInt1::operator/=(float s) { x=div(x,s); return *this; }

void UInt1::reset(float r, float, float, float) { this->r=toUInt(r); }
        
//UByte1 UInt1::toUByte1() const { return UByte1(r); }
void UInt1::setGL() const { glColor3ui(r,r,r); }

/*********************************************************************************************/
// UInt2 :

UInt2::UInt2(GLuint x, GLuint y) : x(x), y(y) {}
UInt2::UInt2(const GLuint *c) : x(c[0]), y(c[1]) {}
//UInt2::UInt2(QRgb c) : r(toFloat(qRed(c))), g(toFloat(qGreen(c))) {}
UInt2::UInt2(QRgb c) : r(qRed(c)), g(qGreen(c)) {}
QRgb UInt2::toQRgb() const { return qRgba(r,g, toInt(0.0f), toInt(1.0f)); }

UInt2 operator+(const UInt2 &a, const UInt2 &b) { return UInt2(a.x + b.x, a.y + b.y); }
UInt2 operator-(const UInt2 &a, const UInt2 &b) { return UInt2(a.x - b.x, a.y - b.y); }
UInt2 operator*(const UInt2 &a, const UInt2 &b) { return UInt2(a.x * b.x, a.y * b.y); }
UInt2 operator/(const UInt2 &a, const UInt2 &b) { return UInt2(a.x / b.x, a.y / b.y); }

UInt2 operator*(float s, const UInt2 &a) { return UInt2(mul(s,a.x), mul(s,a.y)); }
UInt2 operator*(const UInt2 &a, float s) { return s * a; }
UInt2 operator/(const UInt2 &a, float s) { return UInt2(div(a.x,s), div(a.y,s)); }
UInt2 operator/(float s, const UInt2 &a) { return UInt2(div(s,a.x), div(s,a.y)); }

UInt2 UInt2::operator+=(const UInt2 &a) { x+=a.x; y+=a.y; return *this; }
UInt2 UInt2::operator-=(const UInt2 &a) { x-=a.x; y-=a.y; return *this; }
UInt2 UInt2::operator*=(const UInt2 &a) { x*=a.x; y*=a.y; return *this; }
UInt2 UInt2::operator/=(const UInt2 &a) { x/=a.x; y/=a.y; return *this; }
UInt2 UInt2::operator*=(float s) { x=mul(s,x); y=mul(s,y); return *this; }
UInt2 UInt2::operator/=(float s) { x=div(x,s); y=div(y,s); return *this; }

void UInt2::reset(float r, float g, float, float) { this->r=toUInt(r); this->g=toUInt(g); }
void UInt2::setGL() const { glColor3ui(r,g,0); }

/*********************************************************************************************/
// UInt3 :

UInt3::UInt3(GLuint x, GLuint y, GLuint z) : x(x), y(y), z(z) {}
UInt3::UInt3(const GLuint *c) : x(c[0]), y(c[1]), z(c[2]) {}
UInt3::UInt3(QRgb c) : r(qRed(c)), g(qGreen(c)), b(qBlue(c)) {}
QRgb UInt3::toQRgb() const { return qRgba(r,g,b,toInt(1.0f)); }

UInt3 operator+(const UInt3 &a, const UInt3 &b) { return UInt3(a.x + b.x, a.y + b.y, a.z + b.z); }
UInt3 operator-(const UInt3 &a, const UInt3 &b) { return UInt3(a.x - b.x, a.y - b.y, a.z - b.z); }
UInt3 operator*(const UInt3 &a, const UInt3 &b) { return UInt3(a.x * b.x, a.y * b.y, a.z * b.z); }
UInt3 operator/(const UInt3 &a, const UInt3 &b) { return UInt3(a.x / b.x, a.y / b.y, a.z / b.z); }

UInt3 operator*(float s, const UInt3 &a) { return UInt3(mul(s,a.x), mul(s,a.y), mul(s,a.z)); }
UInt3 operator*(const UInt3 &a, float s) { return s * a; }
UInt3 operator/(const UInt3 &a, float s) { return UInt3(div(a.x,s), div(a.y,s), div(a.z,s)); }
UInt3 operator/(float s, const UInt3 &a) { return UInt3(div(s,a.x), div(s,a.y), div(s,a.z)); }

UInt3 UInt3::operator+=(const UInt3 &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
UInt3 UInt3::operator-=(const UInt3 &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
UInt3 UInt3::operator*=(const UInt3 &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
UInt3 UInt3::operator/=(const UInt3 &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
UInt3 UInt3::operator*=(float s) { x=mul(s,x); y=mul(s,y); z=mul(s,z); return *this; }
UInt3 UInt3::operator/=(float s) { x=div(x,s); y=div(y,s); z=div(z,s); return *this; }

void UInt3::reset(float r, float g, float b, float) { this->r=toUInt(r); this->g=toUInt(g); this->b=toUInt(b); }
        
UByte3 UInt3::toUByte3() const { return UByte3(r, g, b); }
void UInt3::setGL() const { glColor3ui(r,g,b); }

/*********************************************************************************************/
// UInt4 :

UInt4::UInt4(GLuint x, GLuint y, GLuint z, GLuint w) : x(x), y(y), z(z), w(w) {}
UInt4::UInt4(const GLuint *c) : x(c[0]), y(c[1]), z(c[2]), w(c[3]) {}
UInt4::UInt4(QRgb c) : r(qRed(c)), g(qGreen(c)), b(qBlue(c)), a(qAlpha(c)) {}
QRgb UInt4::toQRgb() const { return qRgba(r,g,b,a); }

UInt4 operator+(const UInt4 &a, const UInt4 &b) { return UInt4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
UInt4 operator-(const UInt4 &a, const UInt4 &b) { return UInt4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
UInt4 operator*(const UInt4 &a, const UInt4 &b) { return UInt4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
UInt4 operator/(const UInt4 &a, const UInt4 &b) { return UInt4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

UInt4 operator*(float s, const UInt4 &a) { return UInt4(mul(s,a.x), mul(s,a.y), mul(s,a.z), mul(s,a.w)); }
UInt4 operator*(const UInt4 &a, float s) { return s * a; }
UInt4 operator/(const UInt4 &a, float s) { return UInt4(div(a.x,s), div(a.y,s), div(a.z,s), div(a.w,s)); }
UInt4 operator/(float s, const UInt4 &a) { return UInt4(div(s,a.x), div(s,a.y), div(s,a.z), div(s,a.w)); }

UInt4 UInt4::operator+=(const UInt4 &a) { x+=a.x; y+=a.y; z+=a.z; w+=a.w; return *this; }
UInt4 UInt4::operator-=(const UInt4 &a) { x-=a.x; y-=a.y; z-=a.z; w-=a.w; return *this; }
UInt4 UInt4::operator*=(const UInt4 &a) { x*=a.x; y*=a.y; z*=a.z; w*=a.w; return *this; }
UInt4 UInt4::operator/=(const UInt4 &a) { x/=a.x; y/=a.y; z/=a.z; w/=a.w; return *this; }
UInt4 UInt4::operator*=(float s) { x=mul(s,x); y=mul(s,y); z=mul(s,z); w=mul(s,w); return *this; }
UInt4 UInt4::operator/=(float s) { x=div(x,s); y=div(y,s); z=div(z,s); w=div(w,s); return *this; }

void UInt4::reset(float r, float g, float b, float a) { this->r=toUInt(r); this->g=toUInt(g); this->b=toUInt(b); this->a=toUInt(a); }
void UInt4::setAlpha(float a) { this->a = toUInt(a); }
        
UByte4 UInt4::toUByte4() const { return UByte4(r, g, b, a); }
void UInt4::setGL() const { glColor4ui(r,g,b,a); }

BABLIB_NAMESPACE_END

