#ifndef __RENDER_TEXTURE__
#define __RENDER_TEXTURE__

#include <OpenGL.h>
#include "FrameBufferAttachable.h"
#include "Texture.h"
#include <QString>

BABLIB_NAMESPACE_BEGIN

// cette classe représente une texture attachable à un frame-buffer (avec les paramètres nécessaires) :
class RenderTexture : public FrameBufferAttachable {
    public:
        RenderTexture(Texture *texture, GLenum texMode, int level = 0);
        virtual void attach(GLenum attachment) = 0;
        virtual void detach(GLenum attachment) = 0;
        virtual Dim2D size() const = 0;
        virtual void destroy() { texture->destroy(); delete texture; }
    private:
        static QString modeName(GLenum texMode);
    protected:
        Texture *texture;
        int level;
    };

/////////////////////////////////////////////////////

class RenderTex1D : public RenderTexture {
    public:
        RenderTex1D(Texture *texture, int level = 0) : RenderTexture(texture, GL_TEXTURE_1D, level) {}
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        virtual Dim2D size() const;
    };

class RenderTex2D : public RenderTexture {
    public:
        RenderTex2D(Texture *texture, int level = 0) : RenderTexture(texture, GL_TEXTURE_2D, level) {}
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        virtual Dim2D size() const;
    };

class RenderTex3D : public RenderTexture {
    public:
        RenderTex3D(Texture *texture, int zOffset, int level = 0) : RenderTexture(texture, GL_TEXTURE_3D, level), zOffset(zOffset) {}
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        virtual Dim2D size() const;
    private:
        int zOffset;
    };

class RenderTexCube : public RenderTexture {
    public:
        RenderTexCube(Texture *texture, GLenum cubeFace, int level = 0) : RenderTexture(texture, GL_TEXTURE_CUBE_MAP, level), cubeFace(cubeFace) {}
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        virtual Dim2D size() const;
    private:
        GLenum cubeFace;  // valeurs possibles : GL_TEXTURE_CUBE_MAP_{NEGATIVE,POSITIVE}_{X,Y,Z}
    };

class RenderTex2DArray : public RenderTexture {
    public:
        RenderTex2DArray(Texture *texture, int layer, int level = 0) : RenderTexture(texture, GL_TEXTURE_2D_ARRAY_EXT, level), layer(layer) {}
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        virtual Dim2D size() const;
    private:
        int layer;
    };

BABLIB_NAMESPACE_END

#endif

