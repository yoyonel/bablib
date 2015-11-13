#include "Texture2DArray.h"
BABLIB_NAMESPACE_USE

Texture2DArray::Texture2DArray(const AbstractImage3D& source) : source(source) {}

GLenum Texture2DArray::textureMode() const {
    return GL_TEXTURE_2D_ARRAY_EXT;
    }

GLint Texture2DArray::defaultTexFormat() const {
    return source.defaultTexFormat();
    }

void Texture2DArray::loadToGPU(GLint texFormat) const {
    //glTexImage3D(textureMode(), 0, texFormat, w, h, d, 0, Color::DATA_FORMAT, Color::DATA_TYPE, source.data);
    source.loadTexture3D(texFormat, textureMode());
    }

