#ifndef __PROJ3_D__
#define __PROJ3_D__

#include "Vec4d.h"
#include "Vec3d.h"
#include "ProjTransfo.h"
//#include "Matrix3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>
class Matrix3d;

namespace qglviewer {
    class Camera;
    }

BABLIB_NAMESPACE_BEGIN

class Proj3d {
    public:
        Proj3d(double d = 1);       // matrice diagonale avec la valeur 'd' sur la diagonale (l'identité par défaut)
        Proj3d(double m11, double m12, double m13, double m14,
               double m21, double m22, double m23, double m24,
               double m31, double m32, double m33, double m34,
               double m41, double m42, double m43, double m44);
        Proj3d(const double M[16], bool deleteArray = false);  // par colonnes : m_ij = M[i + 4*j]  (ordre OpenGL)
        Proj3d(Vec4d O, Vec4d X, Vec4d Y, Vec4d Z, bool relative=false);    // transfo affine
        Proj3d(Vec3d O, Vec3d X, Vec3d Y, Vec3d Z, bool relative=false);    // transfo affine
        Proj3d(const Matrix3d &R, const Vec3d &T);      // affine transfo : translation * rotation
        Proj3d(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax); // [xmin,xmax]*[ymin,ymax]*[zmin,zmax] --> [0,1]³
        Proj3d(const ProjTransfo &proj);
        
    // transfomations courantes :
    //---------------------------
        static Proj3d scaling(double sx, double sy, double sz);
        static Proj3d scaling(Vec3d s);
        static Proj3d scaling(double s);      // scaling uniforme
        static Proj3d translation(double tx, double ty, double tz);
        static Proj3d translation(Vec3d t);
        static Proj3d translation(double t);    // translation uniforme
        static Proj3d translationAndScaling(double tx, double ty, double tz, double sx, double sy, double sz);         // translation, puis scaling  (S * T)
        static Proj3d translationAndScaling(Vec3d t, Vec3d s);
        static Proj3d translationAndScaling(double t, double s);   // translation puis scaling uniformes
        static Proj3d scalingAndTranslation(double sx, double sy, double sz, double tx, double ty, double tz);         // scaling, puis translation  (T * S)
        static Proj3d scalingAndTranslation(Vec3d s, Vec3d t);
        static Proj3d scalingAndTranslation(double s, double t);   // scaling puis translation uniformes
        static Proj3d rotation(double theta, Vec3d n, bool isNormed = false);    // précondition : isNormed ou ||n||=1
        static Proj3d rotation(Vec3d d);    // rotation autour de d, d'angle ||d||
        static Proj3d rotation(Vec3d src, Vec3d dst, bool normalized = true);    // rotation d'angle minimal faisant tourner la direction de <src> sur celle de <dst>
        static Proj3d rotationX(double theta);  // rotation around X axis
        static Proj3d rotationY(double theta);  // rotation around Y axis
        static Proj3d rotationZ(double theta);  // rotation around Z axis
        
        static Proj3d U2C();   // unit [0,1]³ --> centered [-1,1]³
        static Proj3d C2U();   // centered [-1,1]³ --> unit [0,1]³
        
    // opérations matricielles :
    //--------------------------
        Proj3d operator*(Proj3d P) const;   // composition
        Vec4d operator*(Vec4d v) const;     // application à un vecteur 4
        Vec3d operator*(Vec3d v) const;     // application à un vecteur 3
        
        Proj3d inv() const;
        Proj3d transposed() const;
        void transpose();
        
    // accès aux coefficients :
    //-------------------------
        inline const double* coefs() const { return M; }  // le tableau en mémoire contentant les 16 coefficients, rangés par colonnes
        inline       double* coefs()       { return M; }  // le tableau en mémoire contentant les 16 coefficients, rangés par colonnes
        
        inline double& coef(int i, int j)             { return M[i + 4*j]; }
        inline double  coef(int i, int j) const       { return M[i + 4*j]; }
        inline double& operator()(int i, int j)       { return coef(i,j); }
        inline double  operator()(int i, int j) const { return coef(i,j); }
        
        Vec4d column(int j) const;
        void setColumn(int j, Vec4d c);
        
        //Matrix3 subMatrix3() const;      // upper left sub-matrix 3x3
        
    // interactions avec les matrices OpenGL :
    //----------------------------------------
        static Proj3d getModelViewGL();
        static Proj3d getProjectionGL();
        static Proj3d getTransformGL();        // composée des deux précédentes
        void loadMatrixGL() const;
        void loadModelViewGL() const;
        void loadProjectionGL() const;
        void multMatrixGL() const;
        void multModelViewGL() const;
        void multProjectionGL() const;
        
    // récupération de transformations définies dans une qglviewer::Camera :
    //----------------------------------------------------------------------
        void getModelViewFrom(const qglviewer::Camera &camera);
        void getProjectionFrom(const qglviewer::Camera &camera);
        
    // autres méthodes pratiques :
    //----------------------------
        // le rapport largeur/hauteur d'une caméra utilisant cette transfo :
        double screenRatio(double xMin = -1, double xMax = 1, double yMin = -1, double yMax = 1, double z = 0) const;
        // affichage de la transformation inverse d'une boîte rectangulaire :
        void drawBox(double xMin = 0, double xMax = 1, double yMin = 0, double yMax = 1, double zMin = 0, double zMax = 1) const;
    
        ProjTransfo toProj3f() const;
        
    // conversion au format XML :
    //---------------------------
        Proj3d(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);
        
    private:
        double M[16];
        // les coefficients de la matrice par colonnes (ordre OpenGL) :
        // M = | M[0]  M[4]  M[8]  M[12] |   
        //     | M[1]  M[5]  M[9]  M[13] |
        //     | M[2]  M[6]  M[10] M[14] |
        //     | M[3]  M[7]  M[11] M[15] |
    };

BABLIB_NAMESPACE_END

#endif

