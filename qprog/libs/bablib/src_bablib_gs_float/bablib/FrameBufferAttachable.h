#ifndef __FRAME_BUFFER_ATTACHABLE__
#define __FRAME_BUFFER_ATTACHABLE__

#include <OpenGL.h>
#include "Dim2D.h"

BABLIB_NAMESPACE_BEGIN

// cette interface repr�sente les types d'images attachables � un frame-buffer (render-buffers et textures) :
class FrameBufferAttachable {
    public:
        virtual ~FrameBufferAttachable() {}     // pour �viter les warnings
        virtual void attach(GLenum attachment) = 0;
        virtual void detach(GLenum attachment) = 0;
        virtual Dim2D size() const = 0;
        virtual void destroy() = 0;
    };

BABLIB_NAMESPACE_END

#endif

