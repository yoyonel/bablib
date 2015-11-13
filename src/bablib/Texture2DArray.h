#ifndef __TEXTURE_2D_ARRAY__
#define __TEXTURE_2D_ARRAY__

#include "Image3D.h"
#include <OpenGL.h>

BABLIB_NAMESPACE_BEGIN

class Texture2DArray : public TextureData {
    public:
        Texture2DArray(const AbstractImage3D& source);
        virtual GLenum textureMode() const;
        virtual GLint defaultTexFormat() const;
        virtual void loadToGPU(GLint texFormat) const;
    
    private:
        const AbstractImage3D& source;
    };

BABLIB_NAMESPACE_END

#endif

