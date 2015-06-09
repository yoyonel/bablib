#include <OpenGL.h>
#include "TextureRectangle.h"
BABLIB_NAMESPACE_USE

TextureRectangle::TextureRectangle(const AbstractImage2D& source) : source(source) {}

GLenum TextureRectangle::textureMode() const {
    return GL_TEXTURE_RECTANGLE_ARB;
    }

GLint TextureRectangle::defaultTexFormat() const {
    return source.defaultTexFormat();
    }

void TextureRectangle::loadToGPU(GLint texFormat) const {
    source.loadTexture2D(texFormat, textureMode());
    }

