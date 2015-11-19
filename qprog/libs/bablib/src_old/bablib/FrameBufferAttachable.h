#ifndef __FRAME_BUFFER_ATTACHABLE__
#define __FRAME_BUFFER_ATTACHABLE__

#include <OpenGL.h>
#include "Dim2D.h"

BABLIB_NAMESPACE_BEGIN

// cette interface représente les types d'images attachables à un frame-buffer (render-buffers et textures) :
class FrameBufferAttachable {
    public:
        virtual ~FrameBufferAttachable() {}     // pour éviter les warnings
        virtual void attach(GLenum attachment) = 0;
        virtual void detach(GLenum attachment) = 0;
        virtual Dim2D size() const = 0;
        virtual void destroy() = 0;
    };

BABLIB_NAMESPACE_END

#endif

