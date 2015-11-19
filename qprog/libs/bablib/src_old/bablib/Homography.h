#ifndef __HOMOGRAPHY__
#define __HOMOGRAPHY__

#include "Vec2.h"
#include "Vec3.h"
#include "Matrix3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN

// representation of a 2D projective transform
class Homography {
    public:
        Homography(Matrix3 M);
        Homography(float d = 1.0f);                     // diagonal matrix with value <d> for diagonal coefs (default = identity)
        Homography(float m11, float m12, float m13,
                   float m21, float m22, float m23,
                   float m31, float m32, float m33);
        Homography(const float  M[9], bool deleteArray = false);  // column-wise : m_ij = M[i + 3*j]  (OpenGL order)
        Homography(const double M[9], bool deleteArray = false);  // column-wise : m_ij = M[i + 3*j]  (OpenGL order)
        Homography(Vec3 O, Vec3 X, Vec3 Y, bool relative=false);    // affine transform
        Homography(Vec2 O, Vec2 X, Vec2 Y, bool relative=false);    // affine transform
        Homography(float xmin, float xmax, float ymin, float ymax); // [xmin,xmax]*[ymin,ymax] --> [0,1]²
        
    // common transforms :
    //--------------------
        static Homography scaling(float sx, float sy);
        static Homography scaling(Vec2 s);
        static Homography scaling(float s);      // uniform scaling
        static Homography translation(float tx, float ty);
        static Homography translation(Vec2 t);
        static Homography translation(float t);    // translation uniforme
        static Homography translationAndScaling(float tx, float ty, float sx, float sy);         // translation, then scaling  (S * T)
        static Homography translationAndScaling(Vec2 t, Vec2 s);
        static Homography translationAndScaling(float t, float s);   // translation puis scaling uniformes
        static Homography scalingAndTranslation(float sx, float sy, float tx, float ty);         // scaling, then translation  (T * S)
        static Homography scalingAndTranslation(Vec2 s, Vec2 t);
        static Homography scalingAndTranslation(float s, float t);   // uniform scaling then uniform translation

        /*
        static Homography rotation(float theta, Vec3 n, bool isNormed = false);    // précondition : isNormed ou ||n||=1
        static Homography rotation(Vec3 d);    // rotation autour de d, d'angle ||d||
        static Homography rotation(Vec3 src, Vec3 dst, bool normalized = true);    // rotation d'angle minimal faisant tourner la direction de <src> sur celle de <dst>
        */
        static Homography U2C();   // unit [0,1]² --> centered [-1,1]²
        static Homography C2U();   // centered [-1,1]² --> unit [0,1]²
        
    // matrix operations :
    //--------------------
        Homography operator*(Homography P) const;
        Vec3 operator*(Vec3 v) const;
        Vec2 operator*(Vec2 v) const;
        
        Homography inv() const;
        Homography transposed() const;
        void transpose();
        
    // coefficients access :
    //----------------------
        inline const float* coefs() const { return M.coefs(); }
        
        inline float& coef(int i, int j)             { return M.coef(i,j); }
        inline float  coef(int i, int j) const       { return M.coef(i,j); }
        inline float& operator()(int i, int j)       { return M.coef(i,j); }
        inline float  operator()(int i, int j) const { return M.coef(i,j); }
        
        Vec3 column(int j) const;
        void setColumn(int j, Vec3 c);
        
    /*
    // interactions avec les matrices OpenGL :
    //----------------------------------------
        static Homography getGLModelView();
        static Homography getGLProjection();
        static Homography getGLTransform();
        void glLoadMatrix() const;
        void glLoadModelView() const;
        void glLoadProjection() const;
        void glMultMatrix() const;
        void glMultModelView() const;
        void glMultProjection() const;
        
    // récupération de transformations définies dans une qglviewer::Camera :
    //----------------------------------------------------------------------
        void getModelViewFrom(const qglviewer::Camera &camera);
        void getProjectionFrom(const qglviewer::Camera &camera);
    */
        
    /*
    // autres méthodes pratiques :
    //----------------------------
        // le rapport largeur/hauteur d'une caméra utilisant cette transfo :
        float screenRatio(float xMin = -1, float xMax = 1, float yMin = -1, float yMax = 1, float z = 0) const;
        // affichage de la transformation inverse d'une boîte rectangulaire :
        void drawBox(float xMin = 0, float xMax = 1, float yMin = 0, float yMax = 1, float zMin = 0, float zMax = 1) const;
    */
    
    // conversion au format XML :
    //---------------------------
        Homography(const QDomElement &element);
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);
        
    private:
        Matrix3 M;
    };

BABLIB_NAMESPACE_END


#endif

