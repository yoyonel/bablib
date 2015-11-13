#ifndef __VEC4__
#define __VEC4__

//#include "Vec3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN
class Vec3;

class Vec4 {
    public:
        Vec4(float x, float y, float z, float w = 1);
        Vec4(); // vecteur (0,0,0,1)
        Vec4(float c); // vecteur (c,c,c,c)
        Vec4(Vec3 v, float w = 1);
        Vec4(const float *v);     // tableau de 4 valeurs
        //Vec4(qglviewer::Vec vec);
        Vec4(const QDomElement &element);
        
        friend inline float dot(const Vec4 &a, const Vec4 &b);    // produit scalaire euclidien du R^4
        
    // conversions :
    //--------------
        // opÃÂ©rateurs de conversion automatique :
        inline operator const float* () const { return v; }     // cast en const float*
        inline operator       float* ()       { return v; }     // cast en float*
        /*
        // opÃÂ©rateurs de conversion automatique :
        inline float& operator[](int i) { return v[i]; }
        inline operator const float* () const { return v; }       // cast du Vec4 en const float*
        */
        // autres conversions :
        Vec3    toVec3()    const;
        QString toQString() const;
        // lecture / ÃÂ©criture d'un ÃÂ©lÃÂ©ment XML :
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);

    // opÃÂ©rations gÃÂ©omÃÂ©triques :
    //--------------------------
        friend Vec4 dir(const Vec4 &a, const Vec4 &b);    // vecteur reprÃÂ©sentant la direction de <a> ÃÂ  <b>
        void homogenize();      // division de toutes les composante par la derniÃÂ¨re
        // coordonnÃÂ©es normalisÃÂ©es :
        float xn() const { return x / w; }
        float yn() const { return y / w; }
        float zn() const { return z / w; }
        
    // opÃÂ©rateurs arithmÃÂ©tiques :
    //---------------------------
        // opÃÂ©rateurs arithmÃÂ©tiques unaires :
        friend Vec4 operator-(const Vec4 &a);

        // opÃÂ©rateurs arithmÃÂ©tiques binaires :
        friend Vec4 operator+(const Vec4 &a, const Vec4 &b);
        friend Vec4 operator-(const Vec4 &a, const Vec4 &b);
        friend Vec4 operator*(const Vec4 &a, const Vec4 &b);
        friend Vec4 operator/(const Vec4 &a, const Vec4 &b);
            
        // opÃÂ©rations avec un scalaire :
        friend Vec4 operator*(float s, const Vec4 &a);
        friend Vec4 operator*(const Vec4 &a, float s);
        friend Vec4 operator/(float s, const Vec4 &a);
        friend Vec4 operator/(const Vec4 &a, float s);
        
        // opÃÂ©rateurs de modification :
        Vec4& operator+=(const Vec4 &a);
        Vec4& operator-=(const Vec4 &a);
        Vec4& operator*=(const Vec4 &a);
        Vec4& operator/=(const Vec4 &a);
        Vec4& operator*=(float s);
        Vec4& operator/=(float s);
    
    // opÃÂ©rateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec4 &a, const Vec4 &b);   // comparaison ÃÂ  1e-10 prÃÂ¨s
        friend bool operator!=(const Vec4 &a, const Vec4 &b);   // comparaison ÃÂ  1e-10 prÃÂ¨s
        // les comparaisons suivantes sont le <et> boolÃÂ©en des comparaisons sur les composantes :
        friend bool operator>=(const Vec4 &a, const Vec4 &b);
        friend bool operator<=(const Vec4 &a, const Vec4 &b);
        friend bool operator>(const Vec4 &a, const Vec4 &b);
        friend bool operator<(const Vec4 &a, const Vec4 &b);
        friend bool operator>=(const Vec4 &a, float b);
        friend bool operator<=(const Vec4 &a, float b);
        friend bool operator>(const Vec4 &a, float b);
        friend bool operator<(const Vec4 &a, float b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec4 abs(const Vec4 &a);
        friend Vec4 sign(const Vec4 &a);
        friend Vec4 floor(const Vec4 &a);
        friend Vec4 ceil(const Vec4 &a);
        friend Vec4 fract(const Vec4 &a);
        //friend Vec4 min(const Vec4 &a, const Vec4 &b);
        //friend Vec4 min(const Vec4 &a, float b);
        //friend Vec4 max(const Vec4 &a, const Vec4 &b);
        //friend Vec4 max(const Vec4 &a, float b);
        friend Vec4 clamp(const Vec4 &a, const Vec4 &min, const Vec4 &max);
        friend Vec4 clamp(const Vec4 &a, float min, float max);
        friend Vec4 mix(const Vec4 &a, const Vec4 &b, const Vec4 &alpha);     // (1-alpha) * a + alpha * b
        friend Vec4 mix(const Vec4 &a, const Vec4 &b, float alpha);           // (1-alpha) * a + alpha * b
        friend Vec4 step(const Vec4 &e, const Vec4 &a);
        friend Vec4 step(float e, const Vec4 &a);
        //friend float min(const Vec4 &a);    // minimum des composantes
        //friend float max(const Vec4 &a);    // maximum des composantes
    
    // opÃÂ©rateurs de swizzle :
    //------------------------
        //Vec3 xyz() const { return Vec3(x,y,z); }
        Vec3 xyz() const; //{ return Vec3(x,y,z); }
        
    // calcul de vecteurs alÃÂ©atoires (variables uniformes) :
    //------------------------------------------------------
        static Vec4 random();                       // valeurs dans [0,1]
        static Vec4 random(float min, float max);   // valeurs dans [min,max]
        static Vec4 random(Vec4 min, Vec4 max);     // bornes par composantes

    // opÃÂ©rations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant
        
    private:
        union {
            struct { float x, y, z, w; };
            float v[4];
            };
    };

/*********************************************************************************************/
// inline functions :

#include "Vec3.h"
inline Vec3 Vec4::xyz() const { return Vec3(x,y,z); }

float dot(const Vec4 &a, const Vec4 &b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }


BABLIB_NAMESPACE_END

#endif

