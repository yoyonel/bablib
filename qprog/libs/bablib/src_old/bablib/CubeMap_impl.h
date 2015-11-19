#include "Dir.h"
//#include "ObjectList.h"
#include "Params.h"
#include "Message.h"

BABLIB_NAMESPACE_BEGIN

template<class Color> const QString CubeMapParam<Color>::defaultDir = GlobalParams::getPath("bablib.cubemaps.default.dir");
template<class Color> const QString CubeMapParam<Color>::axisNames[3] = { "x", "y", "z" };
template<class Color> const QString CubeMapParam<Color>::signNames[2] = { "neg", "pos" };
template<class Color> const GLenum  CubeMapParam<Color>::targetNames[6] = {
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

template<class Color>
CubeMapParam<Color>::CubeMapParam(QString dirName) {
    initialize(dirName);
    }

template<class Color>
void CubeMapParam<Color>::initialize(QString dirName) {
    Dir dir(defaultDir, dirName);
    
    int w=0, h=0;   // just to avoid compilation warning
    for (int i=0; i<3; i++) for (int j=0; j<2; j++) {
        QImage image = dir.loadImage(imageFileName(i, j));
        if (image.isNull()) { destroy(); return; }
        
        if (i==0 && j==0) {
            w = image.width();
            h = image.height();
            }
        
        if (image.width() != w || image.height() != h) {
            Message::error("les 6 faces d'une cube-map doivent avoir la même résolution");
            destroy();
            return;
            }

        face[i][j].initialize(image);
        if (!face[i][j].loaded()) { destroy(); return; }
        }
    
    loaded = true;
    }

template<class Color>
void CubeMapParam<Color>::destroy() {
    if (loaded)
        for (int i=0; i<3; i++) for (int j=0; j<2; j++)
            face[i][j].destroy();
    loaded = false;
    }


template<class Color>
void CubeMapParam<Color>::loadToGPU(GLint texFormat) const {
    if (!loaded) {
        Message::error("cube map pas encore chargee");
        return;
        }
    for (int i=0; i<3; i++) for (int j=0; j<2; j++)
        face[i][j].loadTexture2D(texFormat, targetName(i, j));
    }

template<class Color>
GLenum CubeMapParam<Color>::targetName(int i, int j) {
    int c = i + 3*j;
    return targetNames[c];
    }

template<class Color>
QString CubeMapParam<Color>::imageFileName(int i, int j) {
    return axisNames[i] + signNames[j];
    }

template<class Color>
QStringList CubeMapParam<Color>::namesList(QString first) {
    QStringList list = Dir(defaultDir).getDirList();
    const int i = list.indexOf(first);
    if (i != -1) list.push_front(list.takeAt(i));
    return list;
    }

/*********************************************************************************************/
// fonction d'émulation de l'échantillonnage de cube-maps OpenGL (cf spec OpenGL 2.0, page 168) :

template<class Color>
Color CubeMapParam<Color>::sample(Vec3 r) {
    const Vec3 a = abs(r);
    int i;  // indice de l'axe principal
    if (a.x > a.y) {
        if (a.x > a.z)
            i = 0;
        else
            i = 2;
        }
    else {
        if (a.y > a.z)
            i = 1;
        else
            i = 2;
        }
    const bool pos = r[i] > 0;
    float s, t;
    switch(i) {
        case 0 :
            s = pos ? -r[2] : r[2];
            t = -r[1];
            break;
        case 1 :
            s = r[0];
            t = pos ? r[2] : -r[2];
            break;
        case 2 :
        default :
            s = pos ? r[0] : -r[0];
            t = -r[1];
            break;
        }
    const float
        x = (s / a[i] + 1.0f) / 2.0f,
        y = (t / a[i] + 1.0f) / 2.0f;
    const int j = pos ? 1 : 0;
    return face[i][j].interp(x,y);
    }

BABLIB_NAMESPACE_END

