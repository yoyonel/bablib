#include "FrameBuffer.h"
#include "Message.h"
BABLIB_NAMESPACE_USE

void FrameBuffer::create() {
    glGenFramebuffersEXT(1, &id);
    }

void FrameBuffer::destroy() {
    GLuint oldBinding = currentBinding();
    if (oldBinding != id) bind(id);     
    foreach (AttachedImage a, attachments) a.destroy();
    attachments.clear();
    bind(oldBinding);
    glDeleteFramebuffersEXT(1, &id);
    id = 0;
    drawBuffers.clear();
    }

void FrameBuffer::activate() const {
    bind(id);
    drawBuffers.activate();
    }

void FrameBuffer::deactivate() const {
    bind(0);
    }

void FrameBuffer::bind(GLenum id) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
    }

GLuint FrameBuffer::currentBinding() {
    GLint res;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &res);
    return static_cast<GLuint>(res);
    }

void FrameBuffer::attach(AttachedImage image) {
    drawBuffers.add(image.attachment);
    attachments.push_back(image);
    activate();
    image.attach();
    deactivate();
    
    if (size.isDefined() && image.size().isDefined() && image.size() != size) {
        Message::warning("l'image attachÃÂ©e n'a pas la taille spÃÂ©cifiÃÂ©e pour le framebuffer");
        }
    }

void FrameBuffer::attach(GLenum attachment, FrameBufferAttachable *renderImage, bool keepImage) {
    attach(AttachedImage(attachment, renderImage, keepImage));
    }

void FrameBuffer::attachTex1D(GLenum attachment, Texture *texture, int level, bool takeImage) {
    attach(attachment, new RenderTex1D(texture, level), takeImage);
    }

void FrameBuffer::attachTex2D(GLenum attachment, Texture *texture, int level, bool takeImage) {
    attach(attachment, new RenderTex2D(texture, level), takeImage);
    }

void FrameBuffer::attachTex3D(GLenum attachment, Texture *texture, int zOffset, int level, bool takeImage) {
    attach(attachment, new RenderTex3D(texture, zOffset, level), takeImage);
    }

void FrameBuffer::attachTexCube(GLenum attachment, Texture *texture, GLenum cubeFace, int level, bool takeImage) {
    attach(attachment, new RenderTexCube(texture, cubeFace, level), takeImage);
    }

void FrameBuffer::attachRenderBuffer(GLenum attachment, GLenum format) {
    if (size.isDefined())
        attach(attachment, new RenderBuffer(size, format), true);
    else
        Message::error("dimensions du framebuffer indÃÂ©finies");
    }

void FrameBuffer::checkCompleteness(bool messageOnSuccess) const {
    GLuint oldBinding = currentBinding();
    if (oldBinding != id) bind(id);
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT :
            if (messageOnSuccess) Message::info("> frambuffer complet");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT :
            Message::error("un des attachements est incomplet");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT :
            Message::error("aucune image n'est attachÃÂ©e au framebuffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT :
            Message::error("les images attachÃÂ©es n'ont pas toutes la mÃÂªme dimension");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT :
            Message::error("les images de couleurs n'ont pas toutes le mÃÂªme format interne");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT :
            Message::error("un des DRAW_BUFFER<i> n'a pas d'image attachÃÂ©e");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT :
            Message::error("aucune image attachÃÂ©e au point dÃÂ©signÃÂ© par READ_BUFFER");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT :
            Message::error("combinaison de formats non supportÃÂ©e par l'implÃÂ©mentation");
            break;
        }
    if (oldBinding != id) bind(oldBinding);
    }

bool FrameBuffer::isComplete() const {
    GLuint oldBinding = currentBinding();
    if (oldBinding != id) bind(id);
    bool res = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT;
    if (oldBinding != id) bind(oldBinding);
    return res;
    }

FrameBuffer FrameBuffer::create_RGBA8_Z(int w, int h, GLenum depthBufferFormat) {
    FrameBuffer fb(w, h);
    fb.create();
    fb.attachRenderBuffer(GL_COLOR_ATTACHMENT0_EXT, GL_RGBA8);
    fb.attachRenderBuffer(GL_DEPTH_ATTACHMENT_EXT,  depthBufferFormat);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_RGBA16(int w, int h) {
    FrameBuffer fb(w, h);
    fb.create();
    fb.attachRenderBuffer(GL_COLOR_ATTACHMENT0_EXT, GL_RGBA16);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_RGBA8(int w, int h) {
    FrameBuffer fb(w, h);
    fb.create();
    fb.attachRenderBuffer(GL_COLOR_ATTACHMENT0_EXT, GL_RGBA8);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_Z(int w, int h, GLenum depthBufferFormat) {
    FrameBuffer fb(w, h);
    fb.create();
    fb.attachRenderBuffer(GL_DEPTH_ATTACHMENT_EXT,  depthBufferFormat);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_Tex2D_Z(Texture *texture, GLenum depthBufferFormat) {
    if (texture->getMode() != GL_TEXTURE_2D) {
        Message::error("la texture n'est pas une texture 2D");
        return FrameBuffer();
        }
    if (!texture->isLoaded()) {
        Message::error("la texture n'a pas de contenu (taille non dÃÂ©finie)");
        return FrameBuffer();
        }
    FrameBuffer fb(texture->renderSize());
    fb.create();
    fb.attachTex2D(GL_COLOR_ATTACHMENT0_EXT, texture, 0, false);
    fb.attachRenderBuffer(GL_DEPTH_ATTACHMENT_EXT, depthBufferFormat);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_Tex2D(Texture *texture) {
    if (texture->getMode() != GL_TEXTURE_2D) {
        Message::error("la texture n'est pas une texture 2D");
        return FrameBuffer();
        }
    if (!texture->isLoaded()) {
        Message::error("la texture n'a pas de contenu (taille non dÃÂ©finie)");
        return FrameBuffer();
        }
    FrameBuffer fb(texture->renderSize());
    fb.create();
    fb.attachTex2D(GL_COLOR_ATTACHMENT0_EXT, texture, 0, false);
    fb.checkCompleteness(false);
    return fb;
    }

FrameBuffer FrameBuffer::create_TexDepth(Texture *texture, bool deactiveDrawReadBuffer ) {
	if (texture->getMode() != GL_TEXTURE_2D) {
		Message::error("la texture n'est pas une texture 2D");
		return FrameBuffer();
		}
		
	if (!texture->isLoaded()) {
		Message::error("la texture n'a pas de contenu (taille non dÃÂ©finie)");
		return FrameBuffer();
		}
		
	FrameBuffer fb(texture->renderSize());
	fb.create();
	fb.attachTex2D(GL_DEPTH_ATTACHMENT_EXT, texture, 0, false);
	//
	fb.checkCompleteness(true);

	if (deactiveDrawReadBuffer) {
		fb.activate();
		// -- desactivate colorbuffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		fb.deactivate();
		}

	return fb;
	}

