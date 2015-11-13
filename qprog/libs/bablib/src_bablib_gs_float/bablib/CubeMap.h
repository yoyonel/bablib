#ifndef __CUBE_MAP__
#define __CUBE_MAP__

#include <OpenGL.h>
#include "TextureData.h"
#include "Image2D.h"
#include "ColorTypes.h"
#include "Vec3.h"
#include <QString>
#include <QStringList>

BABLIB_NAMESPACE_BEGIN

template<class Color>
class CubeMapParam : public TextureData {
    public:
        CubeMapParam() : loaded(false) {}
        CubeMapParam(QString dirName);
    
        void initialize(QString dirName);
        void destroy();
        
        // ÃÂ©mulation de l'ÃÂ©chantillonnage de cube-maps OpenGL :
        Color sample(float rx, float ry, float rz) { return sample(Vec3(rx,ry,rz)); }
        Color sample(Vec3 r);
        
        virtual GLenum textureMode() const { return GL_TEXTURE_CUBE_MAP; }
        virtual GLint defaultTexFormat() const { return face[0][0].defaultTexFormat(); }
        virtual void loadToGPU(GLint texFormat) const;
        
        static QStringList namesList(QString first = QString::null);     // liste des cubemaps dans le rÃÂ©pertoire <defaultDir>, avec ÃÂ©ventuellement <first> en premier
        
    private:
        static GLenum targetName(int i, int j);       // i:{x=0,y=1,z=2}, j:{neg=0,pos=1}
        static QString imageFileName(int i, int j);   // i:{x=0,y=1,z=2}, j:{neg=0,pos=1}
        
    private:
        bool loaded;
        Image2D<Color> face[3][2];
        
        static const QString defaultDir;
        static const QString axisNames[3];      // x, y, z
        static const QString signNames[2];      // neg, pos
        
    public:
        static const GLenum targetNames[6];
    };

typedef CubeMapParam<Float4> CubeMap;
typedef CubeMapParam<UByte4> CubeMapUByte4;
typedef CubeMapParam<Float1> CubeMapFloat;
typedef CubeMapParam<Float2> CubeMapFloat2;
typedef CubeMapParam<Float3> CubeMapFloat3;
typedef CubeMapParam<Float4> CubeMapFloat4;

BABLIB_NAMESPACE_END

#include "CubeMap_impl.h"

#endif

