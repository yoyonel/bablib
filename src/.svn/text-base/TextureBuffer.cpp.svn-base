#include <OpenGL.h>
#include "TextureBuffer.h"
BABLIB_NAMESPACE_USE

TextureBuffer::TextureBuffer(const AbstractImage1D& source) : source(source), buffer(GL_TEXTURE_BUFFER_EXT, GL_STATIC_DRAW) {}

//@@@@@@@@@
//@ destructeur
//    void glDeleteBuffers( sizei n, const uint *buffers );
void TextureBuffer::destroy() {
    buffer.destroy();
    }

GLenum TextureBuffer::textureMode() const {
    return GL_TEXTURE_BUFFER_EXT;
    }

GLint TextureBuffer::defaultTexFormat() const {
    return source.defaultTexFormat();
    }

void TextureBuffer::loadToGPU(GLint texFormat) const {
    buffer.load(source.ptrData(), source.width() * source.sizeData());
    
    glTexBufferEXT(GL_TEXTURE_BUFFER_EXT, texFormat, buffer.getID());
    //@@@@@@ destroys..
    }

