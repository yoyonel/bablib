#ifndef __MATRIX3__
#define __MATRIX3__

#include "Vec2.h"
#include "Vec3.h"
#include <QString>
#include <QGLViewer/quaternion.h>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN

class Matrix3 {
    public:
        Matrix3(float d = 1.0f);    // diagonal matrix (default = identity)
        Matrix3(float c11, float c12, float c13,
                float c21, float c22, float c23,
                float c31, float c32, float c33);
        Matrix3(Vec3 c0, Vec3 c1, Vec3 c2);     // column vectors
        Matrix3(const float  M[9]);  // column-wise : m_ij = M[i + 3*j]  (OpenGL order)
        Matrix3(const double M[9]);  // column-wise : m_ij = M[i + 3*j]  (OpenGL order)
                
        Matrix3 operator*(Matrix3 P) const;
        Vec3 operator*(Vec3 v) const;
        Vec2 operator*(Vec2 v) const;   // homogeneous coords (division by z)
        
        inline const float* coefs() const { return M; }
        inline float& coef(int i, int j)             { return M[i + 3*j]; }
        inline float  coef(int i, int j) const       { return M[i + 3*j]; }
        inline float& operator()(int i, int j)       { return coef(i,j); }
        inline float  operator()(int i, int j) const { return coef(i,j); }
                
        Vec3 column(int j) const;
        void setColumn(int j, Vec3 c);
        
        Matrix3 inv() const;
        Matrix3 transposed() const;
        void transpose();
        
        QString toQS() const;

        static Matrix3 rotation(float theta, Vec3 n, bool isNormed = false);    // assumes isNormed or ||n||=1
        static Matrix3 rotationX(float theta);
        static Matrix3 rotationY(float theta);
        static Matrix3 rotationZ(float theta);

        static Matrix3 rotation(Vec3 src, Vec3 dst, bool normalized = true);    // rotation d'angle minimal faisant tourner la direction de <src> sur celle de <dst>
        
    // interface with QGLViewer classes :
    //-----------------------------------
        qglviewer::Quaternion toQuat() const;
    
    // XML format conversions :
    //-------------------------
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);
        
    private:
        float M[9];
        // coefficients are ordered column-wise (OpenGL order) :
        // M = | M[0]  M[3]  M[6] |   
        //     | M[1]  M[4]  M[7] |
        //     | M[2]  M[5]  M[8] |
    };

BABLIB_NAMESPACE_END


#endif

