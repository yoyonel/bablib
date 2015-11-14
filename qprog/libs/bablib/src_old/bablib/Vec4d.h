#ifndef __VEC4D__
#define __VEC4D__

//#include "Vec3d.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN
class Vec3d;

class Vec4d {
    public:
        Vec4d(double x, double y, double z, double w = 1);
        Vec4d(); // vecteur (0,0,0,1)
        Vec4d(double c); // vecteur (c,c,c,c)
        Vec4d(Vec3d v, double w = 1);
        Vec4d(const Vec4d &v);
        Vec4d(const double *v);     // tableau de 4 valeurs
        //Vec4d(qglviewer::Vec vec);
        Vec4d(const QDomElement &element);
        
        friend inline double dot(const Vec4d &a, const Vec4d &b);    // produit scalaire euclidien du R^4
        
    // conversions :
    //--------------
        // opérateurs de conversion automatique :
        inline operator const double* () const { return v; }     // cast en const double*
        inline operator       double* ()       { return v; }     // cast en double*
        /*
        // opérateurs de conversion automatique :
        inline double& operator[](int i) { return v[i]; }
        inline operator const double* () const { return v; }       // cast du Vec4d en const double*
        */
        // autres conversions :
        Vec3d   toVec3d()   const;
        QString toQString() const;
        // lecture / écriture d'un élément XML :
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);

    // opérations géométriques :
    //--------------------------
        friend Vec4d dir(const Vec4d &a, const Vec4d &b);    // vecteur représentant la direction de <a> à <b>
        void homogenize();      // division de toutes les composante par la dernière
        // coordonnées normalisées :
        double xn() const { return x / w; }
        double yn() const { return y / w; }
        double zn() const { return z / w; }
        
    // opérateurs arithmétiques :
    //---------------------------
        // opérateurs arithmétiques unaires :
        friend Vec4d operator-(const Vec4d &a);

        // opérateurs arithmétiques binaires :
        friend Vec4d operator+(const Vec4d &a, const Vec4d &b);
        friend Vec4d operator-(const Vec4d &a, const Vec4d &b);
        friend Vec4d operator*(const Vec4d &a, const Vec4d &b);
        friend Vec4d operator/(const Vec4d &a, const Vec4d &b);
            
        // opérations avec un scalaire :
        friend Vec4d operator*(double s, const Vec4d &a);
        friend Vec4d operator*(const Vec4d &a, double s);
        friend Vec4d operator/(double s, const Vec4d &a);
        friend Vec4d operator/(const Vec4d &a, double s);
        
        // opérateurs de modification :
        Vec4d& operator+=(const Vec4d &a);
        Vec4d& operator-=(const Vec4d &a);
        Vec4d& operator*=(const Vec4d &a);
        Vec4d& operator/=(const Vec4d &a);
        Vec4d& operator*=(double s);
        Vec4d& operator/=(double s);
    
    // opérateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec4d &a, const Vec4d &b);   // comparaison à 1e-10 près
        friend bool operator!=(const Vec4d &a, const Vec4d &b);   // comparaison à 1e-10 près
        // les comparaisons suivantes sont le <et> booléen des comparaisons sur les composantes :
        friend bool operator>=(const Vec4d &a, const Vec4d &b);
        friend bool operator<=(const Vec4d &a, const Vec4d &b);
        friend bool operator>(const Vec4d &a, const Vec4d &b);
        friend bool operator<(const Vec4d &a, const Vec4d &b);
        friend bool operator>=(const Vec4d &a, double b);
        friend bool operator<=(const Vec4d &a, double b);
        friend bool operator>(const Vec4d &a, double b);
        friend bool operator<(const Vec4d &a, double b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec4d abs(const Vec4d &a);
        friend Vec4d sign(const Vec4d &a);
        friend Vec4d floor(const Vec4d &a);
        friend Vec4d ceil(const Vec4d &a);
        friend Vec4d fract(const Vec4d &a);
        //friend Vec4d min(const Vec4d &a, const Vec4d &b);
        //friend Vec4d min(const Vec4d &a, double b);
        //friend Vec4d max(const Vec4d &a, const Vec4d &b);
        //friend Vec4d max(const Vec4d &a, double b);
        friend Vec4d clamp(const Vec4d &a, const Vec4d &min, const Vec4d &max);
        friend Vec4d clamp(const Vec4d &a, double min, double max);
        friend Vec4d mix(const Vec4d &a, const Vec4d &b, const Vec4d &alpha);     // (1-alpha) * a + alpha * b
        friend Vec4d mix(const Vec4d &a, const Vec4d &b, double alpha);           // (1-alpha) * a + alpha * b
        friend Vec4d step(const Vec4d &e, const Vec4d &a);
        friend Vec4d step(double e, const Vec4d &a);
        //friend double min(const Vec4d &a);    // minimum des composantes
        //friend double max(const Vec4d &a);    // maximum des composantes
    
    // opérateurs de swizzle :
    //------------------------
        Vec3d xyz() const;  // { return Vec3d(x,y,z); }
        
    // calcul de vecteurs aléatoires (variables uniformes) :
    //------------------------------------------------------
        static Vec4d random();                       // valeurs dans [0,1]
        static Vec4d random(double min, double max);   // valeurs dans [min,max]
        static Vec4d random(Vec4d min, Vec4d max);     // bornes par composantes

    // opérations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant
        
    private:
        union {
            struct { double x, y, z, w; };
            double v[4];
            };
    };

/*********************************************************************************************/
// inline functions :

#include "Vec3d.h"

inline Vec3d Vec4d::xyz() const { return Vec3d(x,y,z); }

double dot(const Vec4d &a, const Vec4d &b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

BABLIB_NAMESPACE_END


#endif

