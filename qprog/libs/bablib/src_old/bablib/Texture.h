#ifndef __TEXTURE__
#define __TEXTURE__

#include <OpenGL.h>
#include "Dim2D.h"
#include "TextureData.h"

BABLIB_NAMESPACE_BEGIN

class Texture {
    public:
        // le constructeur peut etre appele hors d'un contexte GL :
        Texture() : id(0), texData(NULL), texMode(GL_TEXTURE_2D), loaded(false) {}
        Texture(GLuint id, GLenum texMode) : id(id), texData(NULL), texMode(texMode), loaded(hasContent()) {}
        Texture(const TextureData *texData, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        Texture(GLenum texMode, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);     // alloue un id de texture et initialize les params (interp, wrap)
        
        // chargement de la texture en mémoire (doit etre appele dans un contexte GL) :
        void load();                        // charge <texData> en mémoire GPU en utilisant le format par défaut de <texData>
        void load(GLint internalFormat);    // de même mais avec le format précisé
        bool isLoaded() const { return loaded; }
        
        // utilisation de la texture :
        void bind() const;      // attache cette texture comme texture courante (associée à TEX_MODE)
        void destroy();         // pour supprimer le contenu de la texture :
        
        // pour appliquer la texture au rendu (sans shader):
        void activate(GLint mode = GL_REPLACE) const;   // modes possibles: GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE
        void deactivate() const;
        
        // utilisation des unités de texture :
        static void setActiveUnit(int unit);        // activation de l'unité <unit>
        static int maxUnits();                      // nombre maximum d'unités accessibles par un shader
        
        // réglage des paramètres de la texture (un contexte GL doit etre actif) :
        // la texture doit avoir été créée (méthode load())
        void setInterpMode(GLenum interpMode);
        void setWrapMode(GLenum wrapMode);
        void setBorderColor(const float *color) const;
        void setBorderColor(float r, float g, float b, float a = 1) const;
        
        GLuint getID()   const { return id; }
        GLenum getMode() const { return texMode; }
        
        // méthodes de création de textures vides :
        // <internalFormat> : tables 3.15 et 3.16 de la spec OpenGL 2.0
        static Texture createTex1D(int w,                    GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        static Texture createTex2D(int w, int h,             GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        static Texture createTex3D(int w, int h, int d,      GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        static Texture createTex2DArray(int w, int h, int d, GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        static Texture createTexCube(int n,                  GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        
        // méthodes pratiques de création de textures :
        static Texture createTex1D(QString imageFileName, GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);
        static Texture createTex2D(QString imageFileName, GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);

        void copyReadBuffer(GLenum internalFormat, int w, int h, int x=0, int y=0);    // copie le contenu du render-buffer courant dans la texture
        
        // méthodes utiles :
        int getParam(GLenum paramName) const;
        int getLevelParam(GLenum paramName, int level = 0) const { return getLevelParam(texMode, paramName, level); }
        int getLevelParam(int target, GLenum paramName, int level = 0) const;
        int getWidth() const  { return getLevelParam(GL_TEXTURE_WIDTH);  }
        int getHeight() const { return getLevelParam(GL_TEXTURE_HEIGHT); }
        int getDepth() const  { return getLevelParam(GL_TEXTURE_DEPTH);  }
        // effets de bord : texture bindée
        
        // les méthodes suivantes calculent la taille 2D de l'image qui peut etre rendue dans cette texture avec un frame-buffer :
        Dim2D renderSize(int level = 0) const { return renderSize(texMode, level); }
        Dim2D renderSize(GLenum target, int level = 0) const;
        
		// MIPMAP
		// ATTENTION: faire attention sur ATI/NVIDIA à la valeur de base de basel_level (0 ou 1)
        void initAutoMipMapGeneration( 
			GLenum _min_filter = GL_NEAREST_MIPMAP_NEAREST, 
			GLenum _mag_filter = GL_NEAREST, 
			GLint	_max_levels = -1, 
			GLuint	_base_level = 0, 
			GLenum	_hint_mode = GL_DONT_CARE 
		);
        inline void generateMipMap() {  glGenerateMipmapEXT( texMode ); }

		inline GLuint getTextureMaxLevel() const { return m_n_mipmaps; };

    private:
        void generateID();              // alloue un nouvel identifiant. précondition : id <= 0
        void init() const;              // initialise les paramètres de la texture (effet de bord : la texture est bindée)
        void updateInterpMode() const;
        void updateWrapMode() const;
        bool hasContent() const;    // effet de bord : texture bindée
        
    private:
        GLuint id;
        const TextureData *texData;
        mutable GLenum texMode,     // GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
                                    // + GL_TEXTURE_BUFFER_EXT, GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_RECTANGLE_ARB
               interpMode,  // GL_NEAREST, GL_LINEAR
               wrapMode;    // GL_REPEAT, GL_CLAMP, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT
        bool loaded;    // true ssi le contenu de la texture est défini
		GLuint m_n_mipmaps;
    };

BABLIB_NAMESPACE_END

#endif

