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
        FrameBuffer() {}                            // pas de v�rification sur les tailles des attachements
        FrameBuffer(int w, int h) : size(w, h) {}   // permet d'ajouter des renderbuffers/textures sans re-sp�cifier la taille
        FrameBuffer(Dim2D size) : size(size) {}     // permet d'ajouter des renderbuffers/textures sans re-sp�cifier la taille
        
        void create();
        void destroy();
        
        void activate() const;
        void deactivate() const;
        
        // pr�condition : <size> a du �tre sp�cifi�e par le constructeur :
        int width()  const { return size.width();  }
        int height() const { return size.height(); }
        ViewportGL viewport() const { return ViewportGL(0, 0, size.width(), size.height()); }
        
        // pr�condition : les images attach�es doivent avoir la taille <size> si elle a �t� d�finie par le constructeur :
        void attach(AttachedImage image);     // la suppression de <image> est prise en charge
        
        // � la destruction du framebuffer (m�thode destroy()), un 'delete' de <renderImage> sera effectu� (mais pas forc�ment de destroy())
        // par d�faut la destruction (destroy()) des images attach�es est prise en charge (param�tre <takeImage>) :
        void attach(GLenum attachment, FrameBufferAttachable *renderImage, bool takeImage = true);
        void attachRenderBuffer(GLenum attachment, GLenum format);      // cr�e automatiquement le renderbuffer (pr�condition : <size> est d�fini)
        // les m�thodes suivantes permettent d'attacher une texture au frame-buffer :
        // le format de base des textures ('internalFormat') doit �tre GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT ou GL_STENCIL_INDEX (variantes 'sized' possibles)
        void attachTex1D(GLenum attachment, Texture *texture, int level = 0, bool takeImage = true);
        void attachTex2D(GLenum attachment, Texture *texture, int level = 0, bool takeImage = true);
        void attachTex3D(GLenum attachment, Texture *texture, int zOffset = 0, int level = 0, bool takeImage = true);
        void attachTexCube(GLenum attachment, Texture *texture, GLenum cubeFace, int level = 0, bool takeImage = true);
        
        void checkCompleteness(bool messageOnSuccess = true) const; // affiche les informations sur la compl�tude du framebuffer
        bool isComplete() const;                                    // condition � v�rifier avant de pouvoir rendre dans le framebuffer
        
        // m�thodes de cr�ation de framebuffers complets :
        static FrameBuffer create_Color_Z(int w, int h, GLenum colorBufferFormat = GL_RGBA8, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24);
        static FrameBuffer create_Color(int w, int h, GLenum colorBufferFormat = GL_RGBA8);         // frame-buffer without depth-buffer 
        static FrameBuffer create_Z(int w, int h, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24); // frame-buffer without color buffer
        static FrameBuffer create_RGBA8_Z(int w, int h, GLenum depthBufferFormat = GL_DEPTH_COMPONENT24) { return create_Color_Z(w, h, GL_RGBA8, depthBufferFormat); }
        static FrameBuffer create_RGBA8_Z24(int w, int h)    { return create_RGBA8_Z(w, h, GL_DEPTH_COMPONENT24); }
        static FrameBuffer create_RGBA8_Z32(int w, int h)   { return create_RGBA8_Z(w, h, GL_DEPTH_COMPONENT32); }
        static FrameBuffer create_RGBA16(int w, int h)      { return create_Color(w, h, GL_RGBA16); }
        static FrameBuffer create_RGBA8(int w, int h)       { return create_Color(w, h, GL_RGBA8); }
        // pr�condition : <texture> doit �tre un texture 2D valide (m�thode 'isLoaded()')
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
        Dim2D size;     // les dimensions du framebuffer (d�finies uniquement si elles sont sp�cifi�es dans l'appel du constructeur)
    };

BABLIB_NAMESPACE_END

#endif

