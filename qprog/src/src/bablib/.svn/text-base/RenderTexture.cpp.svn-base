#include "RenderTexture.h"
#include "Message.h"
//#include <GL/glprocs.h>
BABLIB_NAMESPACE_USE

/*********************************************************************************************/
// RenderTexture :

RenderTexture::RenderTexture(Texture *texture, GLenum texMode, int level) : texture(texture), level(level) {
    if (texture->getMode() != texMode)
        Message::error(QString("la texture doit etre en mode %1").arg(modeName(texMode)));
    }

QString RenderTexture::modeName(GLenum texMode) {
    switch (texMode) {
        case GL_TEXTURE_1D : return "GL_TEXTURE_1D";
        case GL_TEXTURE_2D : return "GL_TEXTURE_2D";
        case GL_TEXTURE_3D : return "GL_TEXTURE_3D";
        case GL_TEXTURE_CUBE_MAP : default : return "GL_TEXTURE_CUBE_MAP";
        }
    }

/*********************************************************************************************/
// RenderTex1D :

void RenderTex1D::attach(GLenum attachment) { glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, texture->getID(), level); }
void RenderTex1D::detach(GLenum attachment) { glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, 0, level); }
Dim2D RenderTex1D::size() const { return texture->renderSize(level); }

/*********************************************************************************************/
// RenderTex2D :

void RenderTex2D::attach(GLenum attachment) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, texture->getID(), level); }
void RenderTex2D::detach(GLenum attachment) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, 0, level); }
Dim2D RenderTex2D::size() const { return texture->renderSize(level); }

/*********************************************************************************************/
// RenderTex3D :

void RenderTex3D::attach(GLenum attachment) { glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, texture->getID(), level, zOffset); }
void RenderTex3D::detach(GLenum attachment) { glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, 0, level, zOffset); }
Dim2D RenderTex3D::size() const { return texture->renderSize(level); }

/*********************************************************************************************/
// RenderTexCube :

void RenderTexCube::attach(GLenum attachment) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, cubeFace, texture->getID(), level); }
void RenderTexCube::detach(GLenum attachment) { glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, cubeFace, 0, level); }
Dim2D RenderTexCube::size() const { return texture->renderSize(cubeFace, level); }

/*********************************************************************************************/
// RenderTex2DArray :

void RenderTex2DArray::attach(GLenum attachment) { glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, attachment, texture->getID(), level, layer); }
void RenderTex2DArray::detach(GLenum attachment) { glFramebufferTextureLayerEXT(GL_FRAMEBUFFER_EXT, attachment, 0, level, layer); }
Dim2D RenderTex2DArray::size() const { return texture->renderSize(level); }


