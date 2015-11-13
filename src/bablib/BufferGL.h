#ifndef __BUFFER_GL__
#define __BUFFER_GL__

#include <OpenGL.h>

BABLIB_NAMESPACE_BEGIN

class BufferGL {
    public:
        BufferGL(GLenum target, GLenum usage = GL_STATIC_DRAW) : id(0), target(target), usage(usage) {}
        // usage = GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}

        void destroy() { glDeleteBuffers(1, &id); }
        
        void bind()   const { glBindBuffer(target, id); }
        void unbind() const { glBindBuffer(target, 0); }
        
        void load(const void *data, int size) {
            //@ verifs
            glGenBuffers(1, &id);
            bind();
            glBufferData(target, static_cast<GLsizeiptr>(size), data, usage);
            unbind();
            }
        
        // pour mapping direct :
        //void* res = glMapBuffer(TEXTURE_BUFFER_EXT, WRITE_ONLY);    // access = for the client
        
        GLuint getID() const { return id; }
            
    protected:
        GLuint id;
        GLenum target;
        GLenum usage;
    };

BABLIB_NAMESPACE_END

#endif

