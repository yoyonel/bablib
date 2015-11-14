#ifndef __RENDER_BUFFER__
#define __RENDER_BUFFER__

#include <OpenGL.h>
#include "FrameBufferAttachable.h"

BABLIB_NAMESPACE_BEGIN

class RenderBuffer : public FrameBufferAttachable {
    public:
        RenderBuffer(int w = -1, int h = -1, GLenum format = GL_RGBA8);
        RenderBuffer(Dim2D dim, GLenum format = GL_RGBA8);
        
        bool create();
        bool create(int w, int h, GLenum format);
        virtual void destroy();
        
        bool created() const { return id != 0; }
        virtual Dim2D size() const { return dim; }
        
        // méthodes de FrameBufferAttachable :
        virtual void attach(GLenum attachment);
        virtual void detach(GLenum attachment);
        
    private:
        void bind() const;
    
    private:
        GLuint id;
        Dim2D dim;
        GLenum format;  // valeur de la table 3.16 de la spec OpenGL 2.0 ayant un des formats de base suivants : RGB, RGBA, DEPTH_COMPONENT
                        // ou STENCIL_INDEX_{1,4,8,16}_EXT
    };

BABLIB_NAMESPACE_END

#endif

