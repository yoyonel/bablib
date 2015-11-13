#ifndef __TEXTURE_RECTANGLE__
#define __TEXTURE_RECTANGLE__

#include "Image2D.h"
#include <OpenGL.h>

BABLIB_NAMESPACE_BEGIN

class TextureRectangle : public TextureData {
    public:
        TextureRectangle(const AbstractImage2D& source);
        virtual GLenum textureMode() const;
        virtual GLint defaultTexFormat() const;
        virtual void loadToGPU(GLint texFormat) const;
    
    private:
        const AbstractImage2D& source;
    };

BABLIB_NAMESPACE_END

#endif

