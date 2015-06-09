#ifndef __MATRIX3_D__
#define __MATRIX3_D__

#include "Vec2d.h"
#include "Vec3d.h"
#include "Matrix3.h"
#include <QString>
#include <QDomElement>
#include <QDomDocument>
#include <QGLViewer/quaternion.h>

BABLIB_NAMESPACE_BEGIN

class Matrix3d {
    public:
        Matrix3d(double d = 1.0);    // diagonal matrix (default = identity)
        Matrix3d(double c11, double c12, double c13,
                 double c21, double c22, double c23,
                 double c31, double c32, double c33);
        Matrix3d(Vec3d c0, Vec3d c1, Vec3d c2);     // column vectors
        Matrix3d(const double M[9]);  // column-wise : m_ij = M[i + 3*j]  (OpenGL order)
                
        Matrix3d operator*(Matrix3d M) const;
        Vec3d operator*(Vec3d v) const;
        Vec2d operator*(Vec2d v) const;   // homogeneous coords (division by z)
        
        inline const double* coefs() const { return M; }
        inline double& coef(int i, int j)             { return M[i + 3*j]; }
        inline double  coef(int i, int j) const       { return M[i + 3*j]; }
        inline double& operator()(int i, int j)       { return coef(i,j); }
        inline double  operator()(int i, int j) const { return coef(i,j); }
                
        Vec3d column(int j) const;
        void setColumn(int j, Vec3d c);
        
        Matrix3d inv() const;
        Matrix3d transposed() const;
        void transpose();
        
        QString toQS() const;
        
        Matrix3 toMatrix3f() const;
        
        static Matrix3d rotation(double theta, Vec3 n, bool isNormed = false);    // assumes isNormed or ||n||=1
        static Matrix3d rotationX(double theta);
        static Matrix3d rotationY(double theta);
        static Matrix3d rotationZ(double theta);
        
    // interface with QGLViewer classes :
    //-----------------------------------
        qglviewer::Quaternion toQuat() const;
    
    // XML format conversions :
    //-------------------------
        QDomElement domElement(const QString &name, QDomDocument &document) const;
        void initFromDOMElement(const QDomElement &element);
        
    private:
        double M[9];
        // coefficients are ordered column-wise (OpenGL order) :
        // M = | M[0]  M[3]  M[6] |   
        //     | M[1]  M[4]  M[7] |
        //     | M[2]  M[5]  M[8] |
    };

BABLIB_NAMESPACE_END


#endif

