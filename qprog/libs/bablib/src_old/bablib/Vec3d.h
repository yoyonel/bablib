#ifndef __VEC3D__
#define __VEC3D__

#include "Vec2d.h"
#include "Vec3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN
class Vec4d;

class Vec3d {
    public:
        Vec3d(double x, double y, double z);
        Vec3d(double c = 0);  // vecteur (c,c,c)
        Vec3d(Vec2d v, double z = 0);
        Vec3d(double x, Vec2d yz);
        Vec3d(const Vec3 &v);
        Vec3d(const double *v);   // tableau de 3 valeurs
        Vec3d(Vec4d vec);         // fait la division par la 4ème composante avant d'affecter les composantes (x,y,z)
        Vec3d(const QDomElement &element);
        
    // opérateurs de conversion :
    //---------------------------
        // opérateurs de conversion automatique :
        inline operator const double* () const { return v; }     // cast en const double*
        inline operator       double* ()       { return v; }     // cast en double*
        // autres conversions :
        QString toQString() const;
        Vec4d   toVec4d()   const;
        // lecture / écriture d'un élément XML :
        void initFromDOMElement(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;

        Vec3 toVec3f() const;

        
    // opérations géométriques :
    //--------------------------
        double norm() const;         // norme
        double norm2() const;        // norme au carré
        void  normalize();          // normalise le vecteur
        Vec3d  normalized() const;   // retourne le vecteur normalisé

        friend inline Vec3d  vec(const Vec3d &a, const Vec3d &b);    // produit vectoriel
        friend inline double dot(const Vec3d &a, const Vec3d &b);    // produit scalaire
        friend inline Vec3d  operator^(const Vec3d &a, const Vec3d &b) { return vec(a,b); }
        friend inline double operator|(const Vec3d &a, const Vec3d &b) { return dot(a,b); }
        
        friend double dist(Vec3d v1, Vec3d v2);    // distance euclidienne
        friend Vec3d triangleNormal(const Vec3d &a, const Vec3d &b, const Vec3d &c);    // normale du triangle (a,b,c), dans le sens trigo

        Vec3d ortho() const;     // retourne un vecteur orthogonal à celui-ci

    // opérateurs arithmétiques :
    //---------------------------
        // opérateurs arithmétiques unaires :
        friend Vec3d operator-(const Vec3d &a);

        // opérateurs arithmétiques binaires :
        friend Vec3d operator+(const Vec3d &a, const Vec3d &b);
        friend Vec3d operator-(const Vec3d &a, const Vec3d &b);
        friend Vec3d operator*(const Vec3d &a, const Vec3d &b);
        friend Vec3d operator/(const Vec3d &a, const Vec3d &b);
            
        // opérations avec un scalaire :
        friend Vec3d operator*(double s, const Vec3d &a);
        friend Vec3d operator*(const Vec3d &a, double s);
        friend Vec3d operator/(double s, const Vec3d &a);
        friend Vec3d operator/(const Vec3d &a, double s);
        
        // opérateurs de modification :
        Vec3d& operator+=(const Vec3d &a);
        Vec3d& operator-=(const Vec3d &a);
        Vec3d& operator*=(const Vec3d &a);
        Vec3d& operator/=(const Vec3d &a);
        Vec3d& operator*=(double s);
        Vec3d& operator/=(double s);
    
    // opérateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec3d &a, const Vec3d &b);   // comparaison à 1e-10 près
        friend bool operator!=(const Vec3d &a, const Vec3d &b);   // comparaison à 1e-10 près
        // les comparaisons suivantes sont le <et> booléen des comparaisons sur les composantes :
        friend bool operator>=(const Vec3d &a, const Vec3d &b);
        friend bool operator<=(const Vec3d &a, const Vec3d &b);
        friend bool operator>(const Vec3d &a, const Vec3d &b);
        friend bool operator<(const Vec3d &a, const Vec3d &b);
        friend bool operator>=(const Vec3d &a, double b);
        friend bool operator<=(const Vec3d &a, double b);
        friend bool operator>(const Vec3d &a, double b);
        friend bool operator<(const Vec3d &a, double b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec3d abs(const Vec3d &a);
        friend Vec3d sign(const Vec3d &a);
        friend Vec3d floor(const Vec3d &a);
        friend Vec3d ceil(const Vec3d &a);
        friend Vec3d fract(const Vec3d &a);
        //friend Vec3d min(const Vec3d &a, const Vec3d &b);
        //friend Vec3d min(const Vec3d &a, double b);
        //friend Vec3d max(const Vec3d &a, const Vec3d &b);
        //friend Vec3d max(const Vec3d &a, double b);
        friend Vec3d clamp(const Vec3d &a, const Vec3d &min, const Vec3d &max);
        friend Vec3d clamp(const Vec3d &a, double min, double max);
        friend Vec3d mix(const Vec3d &a, const Vec3d &b, const Vec3d &alpha);     // (1-alpha) * a + alpha * b
        friend Vec3d mix(const Vec3d &a, const Vec3d &b, double alpha);           // (1-alpha) * a + alpha * b
        friend Vec3d step(const Vec3d &e, const Vec3d &a);
        friend Vec3d step(double e, const Vec3d &a);
        //friend double min(const Vec3d &a);    // minimum des composantes
        //friend double max(const Vec3d &a);    // maximum des composantes
    
    // opérateurs de swizzle :
    //------------------------
        Vec2d xx() const { return Vec2d(x,x); }
        Vec2d xy() const { return Vec2d(x,y); }
        Vec2d xz() const { return Vec2d(x,z); }
        Vec2d yx() const { return Vec2d(y,x); }
        Vec2d yy() const { return Vec2d(y,y); }
        Vec2d yz() const { return Vec2d(y,z); }
        Vec2d zx() const { return Vec2d(z,x); }
        Vec2d zy() const { return Vec2d(z,y); }
        Vec2d zz() const { return Vec2d(z,z); }
    
        Vec3d yzx() const { return Vec3d(y,z,x); }
        Vec3d zxy() const { return Vec3d(z,x,y); }
        
    // useful constants :
    //-------------------
        static const Vec3d ZERO;
        static const Vec3d ONE;
        static const Vec3d X;
        static const Vec3d Y;
        static const Vec3d Z;
    
    // calcul de vecteurs aléatoires (variables uniformes) :
    //------------------------------------------------------
        static Vec3d random();                       // valeurs dans [0,1]
        static Vec3d random(double min, double max);   // valeurs dans [min,max]
        static Vec3d random(Vec3d min, Vec3d max);     // bornes par composantes
        static Vec3d randomDir();                    // retourne un point aléatoire de la sphère unité    

    // opérations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant
        void glNormal() const;    // effectue le glNormal correspondant
        
    public:
        union {
            struct { double x, y, z; };
            double v[3];
            };
    };

/*********************************************************************************************/
// inline functions :

#include "Vec4d.h"

inline Vec3d::Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
inline Vec3d::Vec3d(double c) : x(c), y(c), z(c) {}
inline Vec3d::Vec3d(Vec2d v, double z) : x(v.x), y(v.y), z(z) {}
inline Vec3d::Vec3d(double x, Vec2d yz) : x(x), y(yz.x), z(yz.y) {}
inline Vec3d::Vec3d(Vec4d vec) : x(vec.xn()), y(vec.yn()), z(vec.zn()) {}
inline Vec3d::Vec3d(const Vec3 &v) : x(v.x), y(v.y), z(v.z) {}
inline Vec3d::Vec3d(const double *v) : x(v[0]), y(v[1]), z(v[2]) {}
inline Vec3d::Vec3d(const QDomElement &element) { initFromDOMElement(element); }

inline Vec3 Vec3d::toVec3f() const { return Vec3(float(x), float(y), float(z)); }
        
Vec3d vec(const Vec3d &a, const Vec3d &b) {
    return Vec3d(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
        );
    }

double dot(const Vec3d &a, const Vec3d &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

BABLIB_NAMESPACE_END

#endif

