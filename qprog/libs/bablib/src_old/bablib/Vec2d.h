#ifndef __VEC2D__
#define __VEC2D__

#include "Vec2.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN
class Vec3d;

class Vec2d {
    public:
        Vec2d(double x, double y);
        Vec2d(double c = 0);      // vecteur (c,c)
        Vec2d(const Vec2 &v);
        Vec2d(const double *v);   // tableau de 2 valeurs
        Vec2d(const QDomElement &element);
        
    // op�rateurs de conversion :
    //---------------------------
        // op�rateurs de conversion automatique :
        inline operator const double* () const { return v; }     // cast en const double*
        inline operator       double* ()       { return v; }     // cast en double*
        // autres conversions :
        QString toQString() const;
        // lecture / �criture d'un �l�ment XML :
        void initFromDOMElement(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;

        Vec2 toVec2f() const;
        
    // op�rations g�om�triques :
    //--------------------------
        double norm() const;         // norme
        double norm2() const;        // norme au carr�
        void  normalize();          // normalise le vecteur
        Vec2d  normalized() const;   // retourne le vecteur normalis�

        friend double dist(Vec2d v1, Vec2d v2);    // distance euclidienne
        
        friend double dot(const Vec2d &a, const Vec2d &b);          // produit scalaire
        friend Vec3d  vec(const Vec2d &a, const Vec2d &b);          // produit vectoriel
        friend double vecz(const Vec2d &a, const Vec2d &b);         // composante z du produit vectoriel
        friend double operator|(const Vec2d &a, const Vec2d &b);    // produit scalaire
        friend Vec3d  operator^(const Vec2d &a, const Vec2d &b);    // produit vectoriel

        double arg() const;      // retourne l'angle par rapport � l'axe des abscisses (dans ]-Pi,Pi])
        Vec2d polar() const;     // retourne les coordonn�es polaires (r, th�ta)
        
        Vec2d ortho() const;     // rotation of +pi/2
        
        static Vec2d dir(double theta);     // vecteur unitaire d'angle theta (en radians)
        
    // op�rateurs arithm�tiques :
    //---------------------------
        // op�rateurs arithm�tiques unaires :
        friend Vec2d operator-(const Vec2d &a);

        // op�rateurs arithm�tiques binaires :
        friend Vec2d operator+(const Vec2d &a, const Vec2d &b);
        friend Vec2d operator-(const Vec2d &a, const Vec2d &b);
        friend Vec2d operator*(const Vec2d &a, const Vec2d &b);
        friend Vec2d operator/(const Vec2d &a, const Vec2d &b);
            
        // op�rations avec un scalaire :
        friend Vec2d operator*(double s, const Vec2d &a);
        friend Vec2d operator*(const Vec2d &a, double s);
        friend Vec2d operator/(double s, const Vec2d &a);
        friend Vec2d operator/(const Vec2d &a, double s);
        
        // op�rateurs de modification :
        Vec2d& operator+=(const Vec2d &a);
        Vec2d& operator-=(const Vec2d &a);
        Vec2d& operator*=(const Vec2d &a);
        Vec2d& operator/=(const Vec2d &a);
        Vec2d& operator*=(double s);
        Vec2d& operator/=(double s);
    
    // op�rateurs de comparaison :
    //----------------------------
        friend bool operator==(const Vec2d &a, const Vec2d &b);   // comparaison � 1e-10 pr�s
        friend bool operator!=(const Vec2d &a, const Vec2d &b);   // comparaison � 1e-10 pr�s
        // les comparaisons suivantes sont le <et> bool�en des comparaisons sur les composantes :
        friend bool operator>=(const Vec2d &a, const Vec2d &b);
        friend bool operator<=(const Vec2d &a, const Vec2d &b);
        friend bool operator>(const Vec2d &a, const Vec2d &b);
        friend bool operator<(const Vec2d &a, const Vec2d &b);
        friend bool operator>=(const Vec2d &a, double b);
        friend bool operator<=(const Vec2d &a, double b);
        friend bool operator>(const Vec2d &a, double b);
        friend bool operator<(const Vec2d &a, double b);
    
    // autres fonctions pratiques :
    //-----------------------------
        friend Vec2d abs(const Vec2d &a);
        friend Vec2d sign(const Vec2d &a);
        friend Vec2d floor(const Vec2d &a);
        friend Vec2d ceil(const Vec2d &a);
        friend Vec2d fract(const Vec2d &a);
        //friend Vec2d min(const Vec2d &a, const Vec2d &b);
        //friend Vec2d min(const Vec2d &a, double b);
        //friend Vec2d max(const Vec2d &a, const Vec2d &b);
        //friend Vec2d max(const Vec2d &a, double b);
        friend Vec2d clamp(const Vec2d &a, const Vec2d &min, const Vec2d &max);
        friend Vec2d clamp(const Vec2d &a, double min, double max);
        friend Vec2d mix(const Vec2d &a, const Vec2d &b, const Vec2d &alpha);     // (1-alpha) * a + alpha * b
        friend Vec2d mix(const Vec2d &a, const Vec2d &b, double alpha);           // (1-alpha) * a + alpha * b
        friend Vec2d step(const Vec2d &e, const Vec2d &a);
        friend Vec2d step(double e, const Vec2d &a);
        //friend double min(const Vec2d &a);    // minimum des composantes
        //friend double max(const Vec2d &a);    // maximum des composantes
    
    // calcul de vecteurs al�atoires (variables uniformes) :
    //------------------------------------------------------
        static Vec2d random();                       // valeurs dans [0,1]
        static Vec2d random(double min, double max);   // valeurs dans [min,max]
        static Vec2d random(Vec2d min, Vec2d max);     // bornes par composantes

    // op�rations OpenGL :
    //--------------------
        void glVertex() const;    // effectue le glVertex correspondant

    public:
        union {
            struct { double x, y; };
            double v[2];
            };
    };

BABLIB_NAMESPACE_END

#endif
