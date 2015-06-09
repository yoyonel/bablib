#include "RenderBuffer.h"
//#include <GL/glprocs.h>
#include "Message.h"
BABLIB_NAMESPACE_USE

RenderBuffer::RenderBuffer(int w, int h, GLenum format)
    : id(0), dim(w, h), format(format) {
    }

RenderBuffer::RenderBuffer(Dim2D dim, GLenum format)
    : id(0), dim(dim), format(format) {
    }

bool RenderBuffer::create() {
    return create(dim.width(), dim.height(), format);
    }

bool RenderBuffer::create(int w, int h, GLenum format) {
    if (id == 0) {
        dim = Dim2D(w, h);
        this->format = format;

        // verification des dimensions :
        int MAX_SIZE;   // dimension maximale pour un renderbuffer
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &MAX_SIZE);
        if (!dim.isBetween(1, MAX_SIZE)) {
            Message::error(QString("les dimensions du renderbuffer doivent etre comprises entre 1 et %1").arg(MAX_SIZE));
            return false;
            }
        
        // generation du renderbuffer ;
        glGenRenderbuffersEXT(1, &id);
        bind();
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, format, dim.width(), dim.height());
        
        //@ checker les éventuelles erreurs GL
        return true;
        }
    else {
        Message::error("renderbuffer deja cree");
        return false;
        }
    }

void RenderBuffer::destroy() {
    if (id != 0) {
        glDeleteRenderbuffersEXT(1, &id);
        id = 0;
        }
    else
        Message::error("le renderbuffer n'a pas ete cree");
    }

void RenderBuffer::bind() const {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id);
    }

/************************************************************************************************************/
// méthodes de FrameBufferAttachable :

void RenderBuffer::attach(GLenum attachment) {
    if (!created() && !create())
        Message::error("impossible de créer le renderbuffer pour l'attacher");
    else
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, id);
    }

void RenderBuffer::detach(GLenum attachment) {
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, 0);
    }

