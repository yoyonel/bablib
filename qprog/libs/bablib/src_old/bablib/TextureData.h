#ifndef __TEXTURE_DATA__
#define __TEXTURE_DATA__

//#include <OpenGL.h>

BABLIB_NAMESPACE_BEGIN

class TextureData {
    public:
        virtual ~TextureData() {}
        virtual GLenum textureMode() const = 0;
        virtual GLint defaultTexFormat() const = 0;
        virtual void loadToGPU(GLint texFormat) const = 0;
        // <texFormat> correspond au format de la texture sur le GPU (= <internalFormat> de la spec OpenGL)
        // valeurs possibles : 1,2,3,4 ou tables 3.15 et 3.16 de la spec OpenGL 2.0
    };

BABLIB_NAMESPACE_END

#endif

