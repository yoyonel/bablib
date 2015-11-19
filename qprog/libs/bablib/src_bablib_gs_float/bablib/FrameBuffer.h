#ifndef __FRAME_BUFFER__
#define __FRAME_BUFFER__

#include <OpenGL.h>
#include "RenderBuffer.h"
#include "DrawBufferList.h"
#include "AttachedImage.h"
#include "RenderTexture.h"
#include "FrameBufferAttachable.h"
#include "Texture.h"
#include "Dim2D.h"
#include "ViewportGL.h"
#include <QList>

BABLIB_NAMESPACE_BEGIN

class FrameBuffer {
    public:
        FrameBuffer() {}                            // pas de vérification sur les tailles des attachements
        FrameBuffer(int w, int h) : size(w, h) {}   // permet d'ajouter des renderbuffers/textures sans re-spécifier la taille
        FrameBuffer(Dim2D size) : size(size) {}     // permet d'ajouter des renderbuffers/textures sans re-spécifier la taille
        
        void create();
        void destroy();
        
        void activate() const;
        void deactivate() const;
        
        // précondition : <size> a du être spécifiée par le constructeur :
        int width()  const { return size.width();  }
        int height() const { return size.height(); }
        ViewportGL viewport() const { return ViewportGL(0, 0, size.width(), size.height()); }
        
        // précondition : les images attachées doivent avoir la taille <size> si elle a été définie par le constructeur :
        void attach(AttachedImage image);     // la suppression de <image> est prise en charge
        
        // à la destruction du framebuffer (méthode destroy()), un 'delete' de <renderImage> sera effectué (mais pas forcément de destroy())
        // par défaut la destruction (destroy()) des images attachées est prise en charge (paramètre <takeImage>) :
        void attach(GLenum attachment, FrameBufferAttachable *renderImage, bool takeImage = true);
        void attachRenderBuffer(GLenum attachment, GLenum format);      // crée automatiquement le renderbuffer (précondition : <size> est défini)
        // les méthodes suivantes permettent d'attacher une texture au frame-buffer :
        // le format de base des textures ('internalFormat') doit être GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT ou GL_STENCIL_INDEX (variantes 'sized' possibles)
        void attachTex1D(GLenum attachment, Texture *texture, int level = 0, bool takeImage = true);
        void attachTex2D(GLenum attachment, Texture *texture, int level = 0, bool takeImage = true);
        void attachTex3D(GLenum attachment, Texture *texture, int zOffset = 0, int level = 0, bool takeImage = true);
        void attachTexCube(GLenum attachment, Texture *texture, GLenum cubeFace, int level = 0, bool takeImage = true);
        
        void checkCompleteness(bool messageOnSuccess = true) const; // affiche les informations sur la complétude du framebuffer
        bool isComplete() const;                                    // condition à vérifier avant de pouvoir rendre dans le framebuffer
        
        // méthodes de création de framebuffers complets :
        static FrameBuffer create_RGBA8_Z(int w, int h, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24);
        static FrameBuffer create_RGBA8_Z24(int w, int h)     { return create_RGBA8_Z(w, h, GL_DEPTH_COMPONENT24); }
        static FrameBuffer create_RGBA8_Z32(int w, int h)     { return create_RGBA8_Z(w, h, GL_DEPTH_COMPONENT32); }
        static FrameBuffer create_RGBA16(int w, int h);         // frame-buffer sans depth-buffer 
        static FrameBuffer create_RGBA8(int w, int h);         // frame-buffer sans depth-buffer 
        static FrameBuffer create_Z(int w, int h, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24);
        // précondition : <texture> doit être un texture 2D valide (méthode 'isLoaded()')
        static FrameBuffer create_Tex2D_Z(Texture *texture, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24);
        static FrameBuffer create_Tex2D_Z24(Texture *texture) { return create_Tex2D_Z(texture, GL_DEPTH_COMPONENT24); }
        static FrameBuffer create_Tex2D_Z32(Texture *texture) { return create_Tex2D_Z(texture, GL_DEPTH_COMPONENT32); }
        static FrameBuffer create_Tex2D(Texture *texture);      // frame-buffer sans depth-buffer
        //
        static FrameBuffer create_TexDepth( Texture *texture, bool deactiveDrawReadBuffer = true );
        
    private:
        static void bind(GLenum id);
        static GLuint currentBinding();
        
    private:
        GLuint id;
        QList<AttachedImage> attachments;
        DrawBufferList drawBuffers;
        Dim2D size;     // les dimensions du framebuffer (définies uniquement si elles sont spécifiées dans l'appel du constructeur)
    };

BABLIB_NAMESPACE_END

#endif

