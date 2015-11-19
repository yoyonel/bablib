#ifndef __VEC3__
#define __VEC3__

//#include "Vec2.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>
#include <math.h>

BABLIB_NAMESPACE_BEGIN
class Vec2;
class Vec4;

class Vec3 {
    public:
        Vec3(float x, float y, float z);
        Vec3(float c = 0);  // vecteur (c,c,c)
        Vec3(Vec2 v, float z = 0);
        Vec3(float x, Vec2 yz);
        Vec3(const float *v);   // tableau de 3 valeurs
        Vec3(Vec4 vec);         // fait la division par la 4ème composante avant d'affecter les composantes (x,y,z)
        Vec3(const QDomElement &element);
        
    // opérateurs de conversion :
    //---------------------------
        // opérateurs de conversion automatique :
        inline operator const float* () const { return v; }     // cast en const float*
        inline operator       float* ()       { return v; }     // cast en float*
        // autres conversions :
        QString toQString() const;
        Vec4    toVec4()    const;
        // lecture / écriture d'un élément XML :
        void initFromDOMElement(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;

    // opérations géométriques :
    //--------------------------
        float norm() const;         // norme
        float norm2() const;        // norme au carré
        void  normalize();          // normalise le vecteur
        Vec3  normalized() const;   // retourne le vecteur normalisé

        friend Vec3  vec(const Vec3 &a, const Vec3 &b);    // produit vectoriel
        friend float dot(const Vec3 &a, const Vec3 &b);    // produit scalaire
        friend Vec3  operator^(const Vec3 &a, const Vec3 &b) { return vec(a,b); }
        friend float operator|(const Vec3 &a, const Vec3 &b) { return dot(a,b); }
        
        friend float dist(Vec3 v1, Vec3 v2);    // distance euclidienne
        friend Vec3 triangleNormal(const Vec3 &a, const Vec3 &b, const Vec3 &c);    // normale du triangle (a,b,c), dans le sens trigo

        Vec3 ortho() const;     // retourne un vecteur orthogonal à celui-ci

    // opérateurs arithmétiques :
    //---------------------------
        // opérateurs arithmétiques unaires :
        friend Vec3 operator-(const Vec3 &a);

        // opérateurs arithmétiques binaires :
        friend Vec3 operator+(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator-(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator*(const Vec3 &a, const Vec3 &b);
        friend Vec3 operator/(const Vec3 &a, const Vec3 &b);
            
        // opérations avec un scalaire :
        friend Vec3 operator*(float s, const Vec3 &a);
        friend Vec3 operator*(const Vec3 &a, float s);
        friend Vec3 operator/(float s, const Vec3 &a);
        friend Vec3 operator/(const Vec3 &a, float s);
        
        // opérateurs de modification :
        Vec3& operator+=(const Vec3 &a);
        Vec3& operator-=(const Vec3 &a);
        Vec3& operator*=(const Vec3 &a);
        Vec3& operator/=(const Vec3 &a);
        Vec3& operator*=(float s);
        Vec3& operator/=(float s);
    
    // opérateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec3 &a, const Vec3 &b);   // comparaison à 1e-10 près
        friend bool operator!=(const Vec3 &a, const Vec3 &b);   // comparaison à 1e-10 près
        // les comparaisons suivantes sont le <et> booléen des comparaisons sur les composantes :
        friend bool operator>=(const Vec3 &a, const Vec3 &b);
        friend bool operator<=(const Vec3 &a, const Vec3 &b);
        friend bool operator>(const Vec3 &a, const Vec3 &b);
        friend bool operator<(const Vec3 &a, const Vec3 &b);
        friend bool operator>=(const Vec3 &a, float b);
        friend bool operator<=(const Vec3 &a, float b);
        friend bool operator>(const Vec3 &a, float b);
        friend bool operator<(const Vec3 &a, float b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec3 abs(const Vec3 &a);
        friend Vec3 sign(const Vec3 &a);
        friend Vec3 floor(const Vec3 &a);
        friend Vec3 ceil(const Vec3 &a);
        friend Vec3 fract(const Vec3 &a);
        friend Vec3 min(const Vec3 &a, const Vec3 &b);
        friend Vec3 min(const Vec3 &a, float b);
        friend Vec3 max(const Vec3 &a, const Vec3 &b);
        friend Vec3 max(const Vec3 &a, float b);
        friend Vec3 clamp(const Vec3 &a, const Vec3 &min, const Vec3 &max);
        friend Vec3 clamp(const Vec3 &a, float min, float max);
        friend Vec3 mix(const Vec3 &a, const Vec3 &b, const Vec3 &alpha);     // (1-alpha) * a + alpha * b
        friend Vec3 mix(const Vec3 &a, const Vec3 &b, float alpha);           // (1-alpha) * a + alpha * b
        friend Vec3 step(const Vec3 &e, const Vec3 &a);
        friend Vec3 step(float e, const Vec3 &a);
        friend float min(const Vec3 &a);    // minimum des composantes
        friend float max(const Vec3 &a);    // maximum des composantes
    
    // opérateurs de swizzle :
    //------------------------
        Vec2 xx() const; //{ return Vec2(x,x); }
        Vec2 xy() const; //{ return Vec2(x,y); }
        Vec2 xz() const; //{ return Vec2(x,z); }
        Vec2 yx() const; //{ return Vec2(y,x); }
        Vec2 yy() const; //{ return Vec2(y,y); }
        Vec2 yz() const; //{ return Vec2(y,z); }
        Vec2 zx() const; //{ return Vec2(z,x); }
        Vec2 zy() const; //{ return Vec2(z,y); }
        Vec2 zz() const; //{ return Vec2(z,z); }
    
        Vec3 yzx() const; // { return Vec3(y,z,x); }
        Vec3 zxy() const; // { return Vec3(z,x,y); }
        
    // calcul de vecteurs aléatoires (variables uniformes) :
    //------------------------------------------------------
        static Vec3 random();                       // valeurs dans [0,1]
        static Vec3 random(float min, float max);   // valeurs dans [min,max]
        static Vec3 random(Vec3 min, Vec3 max);     // bornes par composantes
        static Vec3 randomDir();                    // retourne un point aléatoire de la sphère unité
    

    // opérations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant
        void glNormal() const;    // effectue le glNormal correspondant
        
    public:
        union {
            struct { float x, y, z; };
            float v[3];
            };
    };

/*********************************************************************************************/
// inline functions :

#include "Vec2.h"
#include "Vec4.h"

inline Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
inline Vec3::Vec3(float c) : x(c), y(c), z(c) {}
inline Vec3::Vec3(Vec2 v, float z) : x(v.x), y(v.y), z(z) {}
inline Vec3::Vec3(float x, Vec2 yz) : x(x), y(yz.x), z(yz.y) {}
inline Vec3::Vec3(Vec4 vec) : x(vec.xn()), y(vec.yn()), z(vec.zn()) {}
inline Vec3::Vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}
inline Vec3::Vec3(const QDomElement &element) { initFromDOMElement(element); }

inline Vec3 operator-(const Vec3 &a) { return Vec3(-a.x, -a.y, -a.z); }
    
/*********************************************************************************************/
    
inline Vec3 operator+(const Vec3 &a, const Vec3 &b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vec3 operator-(const Vec3 &a, const Vec3 &b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec3 operator*(const Vec3 &a, const Vec3 &b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vec3 operator/(const Vec3 &a, const Vec3 &b) { return Vec3(a.x / b.x, a.y / b.y, a.z / b.z); }

/*********************************************************************************************/

inline Vec3 operator*(float s, const Vec3 &a) { return Vec3(s * a.x, s * a.y, s * a.z); }
inline Vec3 operator*(const Vec3 &a, float s) { return s * a; }
inline Vec3 operator/(const Vec3 &a, float s) { return Vec3(a.x / s, a.y / s, a.z / s); }
inline Vec3 operator/(float s, const Vec3 &a) { return Vec3(s / a.x, s / a.y, s / a.z); }

/*********************************************************************************************/

inline Vec3& Vec3::operator+=(const Vec3 &a) { x+=a.x; y+=a.y; z+=a.z; return *this; }
inline Vec3& Vec3::operator-=(const Vec3 &a) { x-=a.x; y-=a.y; z-=a.z; return *this; }
inline Vec3& Vec3::operator*=(const Vec3 &a) { x*=a.x; y*=a.y; z*=a.z; return *this; }
inline Vec3& Vec3::operator/=(const Vec3 &a) { x/=a.x; y/=a.y; z/=a.z; return *this; }
inline Vec3& Vec3::operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
inline Vec3& Vec3::operator/=(float s) { x/=s; y/=s; z/=s; return *this; }

/*********************************************************************************************/

inline bool operator==(const Vec3 &a, const Vec3 &b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool operator!=(const Vec3 &a, const Vec3 &b) { return a.x != b.x || a.y != b.y || a.z != b.z; }
inline bool operator>=(const Vec3 &a, const Vec3 &b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z; }
inline bool operator<=(const Vec3 &a, const Vec3 &b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z; }
inline bool operator> (const Vec3 &a, const Vec3 &b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z; }
inline bool operator< (const Vec3 &a, const Vec3 &b) { return a.x <  b.x && a.y <  b.y && a.z <  b.z; }
inline bool operator>=(const Vec3 &a, float  b) { return a.x >= b && a.y >= b && a.z >= b; }
inline bool operator<=(const Vec3 &a, float  b) { return a.x <= b && a.y <= b && a.z <= b; }
inline bool operator> (const Vec3 &a, float  b) { return a.x >  b && a.y >  b && a.z >  b; }
inline bool operator< (const Vec3 &a, float  b) { return a.x <  b && a.y <  b && a.z <  b; }

/*********************************************************************************************/

inline Vec2 Vec3::xx() const { return Vec2(x,x); }
inline Vec2 Vec3::xy() const { return Vec2(x,y); }
inline Vec2 Vec3::xz() const { return Vec2(x,z); }
inline Vec2 Vec3::yx() const { return Vec2(y,x); }
inline Vec2 Vec3::yy() const { return Vec2(y,y); }
inline Vec2 Vec3::yz() const { return Vec2(y,z); }
inline Vec2 Vec3::zx() const { return Vec2(z,x); }
inline Vec2 Vec3::zy() const { return Vec2(z,y); }
inline Vec2 Vec3::zz() const { return Vec2(z,z); }

inline Vec3 Vec3::yzx() const { return Vec3(y,z,x); }
inline Vec3 Vec3::zxy() const { return Vec3(z,x,y); }

/*********************************************************************************************/

inline float Vec3::norm2()      const { return x*x + y*y + z*z; }
inline float Vec3::norm()       const { return sqrtf(norm2()); }
inline Vec3  Vec3::normalized() const { return (*this) / norm(); }
inline void  Vec3::normalize()        { (*this) /= norm(); }

inline Vec3 vec(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
        );
    }

inline float dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline float dist(Vec3 v1, Vec3 v2) { return (v2 - v1).norm(); }

inline Vec3 triangleNormal(const Vec3 &a, const Vec3 &b, const Vec3 &c) { return ((b - a) ^ (c - a)).normalized(); }    //@@ attention : pas robuste si le prod vectoriel s'annule (ex: si a==b ou a==c)

/*********************************************************************************************/

BABLIB_NAMESPACE_END

#endif

