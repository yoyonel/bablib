#ifndef __VEC2__
#define __VEC2__

#include <QString>
#include <QDomElement>
#include <QDomDocument>

// #undef fminf
// #undef fmaxf
// #undef fmin
// #undef fmax

// #ifndef fminf
// #define fminf(_a, _b) (_a) < (_b) ? (_a) : (_b)
// #define fmaxf(_a, _b) fminf(_b, _a)
// #ifndef fmax
// #define fmax fmaxf
// #endif
// #ifndef fmin
// #define fmin fminf
// #endif
// #endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif

BABLIB_NAMESPACE_BEGIN
class Vec3;

class Vec2 {
    public:
        Vec2(float x, float y);
        Vec2(float c = 0);      // vecteur (c,c)
        Vec2(const float *v);   // tableau de 2 valeurs
        Vec2(const QDomElement &element);
        
    // opérateurs de conversion :
    //---------------------------
        // opérateurs de conversion automatique :
        inline operator const float* () const { return v; }     // cast en const float*
        inline operator       float* ()       { return v; }     // cast en float*
        // autres conversions :
        QString toQString() const;
        // lecture / écriture d'un élément XML :
        void initFromDOMElement(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;

    // opérations géométriques :
    //--------------------------
        float norm() const;         // norme
        float norm2() const;        // norme au carré
        void  normalize();          // normalise le vecteur
        Vec2  normalized() const;   // retourne le vecteur normalisé

        friend float dist(Vec2 v1, Vec2 v2);    // distance euclidienne
        
        friend float dot(const Vec2 &a, const Vec2 &b);     // produit scalaire
        friend Vec3  vec(const Vec2 &a, const Vec2 &b);     // produit vectoriel
        friend float vecz(const Vec2 &a, const Vec2 &b);    // composante z du produit vectoriel
        friend float operator|(const Vec2 &a, const Vec2 &b);   // produit scalaire
        friend Vec3  operator^(const Vec2 &a, const Vec2 &b);   // produit vectoriel

        float arg() const;      // retourne l'angle par rapport à l'axe des abscisses (dans ]-Pi,Pi])
        Vec2 toPolar() const;     // retourne les coordonnées polaires (r, théta)
        
        Vec2 ortho() const;     // rotation of +pi/2
        
        static Vec2 dir(float theta);     // vecteur unitaire d'angle theta (en radians)
        static Vec2 polar(float radius, float theta);
        static Vec2 polar(const Vec2 &p);
        
    // opérateurs arithmétiques :
    //---------------------------
        // opérateurs arithmétiques unaires :
        friend Vec2 operator-(const Vec2 &a);

        // opérateurs arithmétiques binaires :
        friend Vec2 operator+(const Vec2 &a, const Vec2 &b);
        friend Vec2 operator-(const Vec2 &a, const Vec2 &b);
        friend Vec2 operator*(const Vec2 &a, const Vec2 &b);
        friend Vec2 operator/(const Vec2 &a, const Vec2 &b);
            
        // opérations avec un scalaire :
        friend Vec2 operator*(float s, const Vec2 &a);
        friend Vec2 operator*(const Vec2 &a, float s);
        friend Vec2 operator/(float s, const Vec2 &a);
        friend Vec2 operator/(const Vec2 &a, float s);
        
        // opérateurs de modification :
        Vec2& operator+=(const Vec2 &a);
        Vec2& operator-=(const Vec2 &a);
        Vec2& operator*=(const Vec2 &a);
        Vec2& operator/=(const Vec2 &a);
        Vec2& operator*=(float s);
        Vec2& operator/=(float s);
    
    // opérateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec2 &a, const Vec2 &b);   // comparaison à 1e-10 près
        friend bool operator!=(const Vec2 &a, const Vec2 &b);   // comparaison à 1e-10 près
        // les comparaisons suivantes sont le <et> booléen des comparaisons sur les composantes :
        friend bool operator>=(const Vec2 &a, const Vec2 &b);
        friend bool operator<=(const Vec2 &a, const Vec2 &b);
        friend bool operator>(const Vec2 &a, const Vec2 &b);
        friend bool operator<(const Vec2 &a, const Vec2 &b);
        friend bool operator>=(const Vec2 &a, float b);
        friend bool operator<=(const Vec2 &a, float b);
        friend bool operator>(const Vec2 &a, float b);
        friend bool operator<(const Vec2 &a, float b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec2 abs(const Vec2 &a);
        friend Vec2 sign(const Vec2 &a);
        friend Vec2 floor(const Vec2 &a);
        friend Vec2 ceil(const Vec2 &a);
        friend Vec2 fract(const Vec2 &a);
        //friend Vec2 min(const Vec2 &a, const Vec2 &b);
        //friend Vec2 min(const Vec2 &a, float b);
        //friend Vec2 max(const Vec2 &a, const Vec2 &b);
        //friend Vec2 max(const Vec2 &a, float b);
        friend Vec2 clamp(const Vec2 &a, const Vec2 &min, const Vec2 &max);
        friend Vec2 clamp(const Vec2 &a, float min, float max);
        friend Vec2 mix(const Vec2 &a, const Vec2 &b, const Vec2 &alpha);     // (1-alpha) * a + alpha * b
        friend Vec2 mix(const Vec2 &a, const Vec2 &b, float alpha);           // (1-alpha) * a + alpha * b
        friend Vec2 step(const Vec2 &e, const Vec2 &a);
        friend Vec2 step(float e, const Vec2 &a);
        //friend float min(const Vec2 &a);    // minimum des composantes
        //friend float max(const Vec2 &a);    // maximum des composantes
    
    // useful constants :
    //-------------------
        static const Vec2 ZERO;
        static const Vec2 ONE;
        static const Vec2 X;
        static const Vec2 Y;

    // calcul de vecteurs aléatoires (variables uniformes) :
    //------------------------------------------------------
        static Vec2 random();                       // valeurs dans [0,1]
        static Vec2 random(float min, float max);   // valeurs dans [min,max]
        static Vec2 random(Vec2 min, Vec2 max);     // bornes par composantes

    // opérations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant

    public:
        union {
            struct { float x, y; };
            float v[2];
            };
    };

BABLIB_NAMESPACE_END

#endif

