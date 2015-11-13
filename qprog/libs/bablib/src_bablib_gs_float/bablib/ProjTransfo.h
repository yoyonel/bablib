#ifndef __PROJ_TRANSFO__
#define __PROJ_TRANSFO__

#include "Vec4.h"
#include "Vec3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>
//
#include <QGLViewer/camera.h>

namespace qglviewer {
    class Camera;
    }

BABLIB_NAMESPACE_BEGIN

class ProjTransfo {
    public:
        ProjTransfo(float d = 1);       // matrice diagonale avec la valeur 'd' sur la diagonale (l'identitÃÂ© par dÃÂ©faut)
        ProjTransfo(float m11, float m12, float m13, float m14,
                    float m21, float m22, float m23, float m24,
                    float m31, float m32, float m33, float m34,
                    float m41, float m42, float m43, float m44);
        ProjTransfo(const float  M[16], bool deleteArray = false);  // par colonnes : m_ij = M[i + 4*j]  (ordre OpenGL)
        ProjTransfo(const double M[16], bool deleteArray = false);  // par colonnes : m_ij = M[i + 4*j]  (ordre OpenGL)
        ProjTransfo(Vec4 O, Vec4 X, Vec4 Y, Vec4 Z, bool relative=false);    // transfo affine
        ProjTransfo(Vec3 O, Vec3 X, Vec3 Y, Vec3 Z, bool relative=false);    // transfo affine
        ProjTransfo(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax); // [xmin,xmax]*[ymin,ymax]*[zmin,zmax] --> [0,1]ÃÂ³
        
		//
		ProjTransfo( const qglviewer::Camera &camera );
		
    // transfomations courantes :
    //---------------------------
        static ProjTransfo scaling(float sx, float sy, float sz);
        static ProjTransfo scaling(Vec3 s);
        static ProjTransfo scaling(float s);      // scaling uniforme
        static ProjTransfo translation(float tx, float ty, float tz);
        static ProjTransfo translation(Vec3 t);
        static ProjTransfo translation(float t);    // translation uniforme
        static ProjTransfo translationAndScaling(float tx, float ty, float tz, float sx, float sy, float sz);         // translation, puis scaling  (S * T)
        static ProjTransfo translationAndScaling(Vec3 t, Vec3 s);
        static ProjTransfo translationAndScaling(float t, float s);   // translation puis scaling uniformes
        static ProjTransfo scalingAndTranslation(float sx, float sy, float sz, float tx, float ty, float tz);         // scaling, puis translation  (T * S)
        static ProjTransfo scalingAndTranslation(Vec3 s, Vec3 t);
        static ProjTransfo scalingAndTranslation(float s, float t);   // scaling puis translation uniformes
        static ProjTransfo rotation(float theta, Vec3 n, bool isNormed = false);    // prÃÂ©condition : isNormed ou ||n||=1
        static ProjTransfo rotation(Vec3 d);    // rotation autour de d, d'angle ||d||

        static ProjTransfo rotation(Vec3 src, Vec3 dst, bool normalized = true);    // rotation d'angle minimal faisant tourner la direction de <src> sur celle de <dst>
        
        static ProjTransfo U2C();   // unit [0,1]ÃÂ³ --> centered [-1,1]ÃÂ³
        static ProjTransfo C2U();   // centered [-1,1]ÃÂ³ --> unit [0,1]ÃÂ³
        
    // opÃÂ©rations matricielles :
    //--------------------------
        ProjTransfo operator*(ProjTransfo P) const;     // composition
        Vec4 operator*(Vec4 v) const;                   // application a un vecteur 4
        Vec3 operator*(Vec3 v) const;                   // application a un vecteur 3
        
        ProjTransfo inv() const;
        ProjTransfo transposed() const;
        void transpose();
        
    // accÃÂ¨s aux coefficients :
    //-------------------------
        inline const float* coefs() const { return M; }  // le tableau en mÃÂ©moire contentant les 16 coefficients, rangÃÂ©s par colonnes
        
        inline float& coef(int i, int j)             { return M[i + 4*j]; }
        inline float  coef(int i, int j) const       { return M[i + 4*j]; }
        inline float& operator()(int i, int j)       { return coef(i,j); }
        inline float  operator()(int i, int j) const { return coef(i,j); }
        
        Vec4 column(int j) const;
        void setColumn(int j, Vec4 c);
        
    // interactions avec les matrices OpenGL :
    //----------------------------------------
        static ProjTransfo getGLModelView();
        static ProjTransfo getGLProjection();
        static ProjTransfo getGLTransform();        // composÃÂ©e des deux prÃÂ©cÃÂ©dentes
        void glLoadMatrix() const;
        void glLoadModelView() const;
        void glLoadProjection() const;
        void glMultMatrix() const;
        void glMultModelView() const;
        void glMultProjection() const;
        
    // rÃÂ©cupÃÂ©ration de transformations dÃÂ©finies dans une qglviewer::Camera :
    //----------------------------------------------------------------------
		static ProjTransfo getModelView(const qglviewer::Camera &camera);
		static ProjTransfo getProjection(const qglviewer::Camera &camera);
		//
        void getModelViewFrom(const qglviewer::Camera &camera);
        void getProjectionFrom(const qglviewer::Camera &camera);
        
    // autres mÃÂ©thodes pratiques :
    //----------------------------
        // le rapport largeur/hauteur d'une camÃÂ©ra utilisant cette transfo :
        float screenRatio(float xMin = -1, float xMax = 1, float yMin = -1, float yMax = 1, float z = 0) const;
        // affichage de la transformation inverse d'une boÃÂ®te rectangulaire :
        void drawBox(float xMin = 0, float xMax = 1, float yMin = 0, float yMax = 1, float zMin = 0, float zMax = 1) const;
    
    // conversion au format XML :
    //---------------------------
        ProjTransfo(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);
        
    private:
        float M[16];
        // les coefficients de la matrice par colonnes (ordre OpenGL) :
        // M = | M[0]  M[4]  M[8]  M[12] |   
        //     | M[1]  M[5]  M[9]  M[13] |
        //     | M[2]  M[6]  M[10] M[14] |
        //     | M[3]  M[7]  M[11] M[15] |
    };

BABLIB_NAMESPACE_END

#endif

