#ifndef __ATTACHED_IMAGE__
#define __ATTACHED_IMAGE__

#include <OpenGL.h>
#include "FrameBufferAttachable.h"
#include "Dim2D.h"

BABLIB_NAMESPACE_BEGIN

// cette classe sert à représenter une association entre un 'attachment point' et une 'framebuffer-attachable image' :
class AttachedImage {
    public:
        AttachedImage(GLenum attachment, FrameBufferAttachable *image, bool ownImage = true) : attachment(attachment), image(image), ownImage(ownImage) {}
        void attach();
        void detach();
        void destroy();         // détruit (destroy et delete) l'image attachée ssi <ownImage> == true, la supprime seulement sinon (delete uniquement)
        Dim2D size() const;
    
    public:
        GLuint attachment;
        // valeurs possibles pour le paramètre <attachment> :
        //  - COLOR_ATTACHMENT<n>_EXT  avec  <n> dans [0, GL_MAX_COLOR_ATTACHMENTS_EXT]
        //  - DEPTH_ATTACHMENT_EXT
        //  - STENCIL_ATTACHMENT_EXT
        FrameBufferAttachable *image;
        bool ownImage;      // true ssi cet objet doit s'occuper du delete de l'image attachée
    };

BABLIB_NAMESPACE_END

#endif

