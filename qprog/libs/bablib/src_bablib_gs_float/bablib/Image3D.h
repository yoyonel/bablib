#ifndef __IMAGE3D__
#define __IMAGE3D__

#include <OpenGL.h>
#include <QImage>
#include "TextureData.h"
#include "ColorFilter.h"
#include "ColorTypes.h"
#include "Kernel3D.h"
#include "Texture.h"

BABLIB_NAMESPACE_BEGIN
class Pix3DEvaluator;

class AbstractImage3D : public TextureData {
    public:
        AbstractImage3D(int w=0, int h=0, int d=0) : w(w), h(h), d(d) {}
        virtual ~AbstractImage3D() {}

        virtual bool loaded() const = 0;
        virtual void destroy() = 0;

        int width()  const { return w; }
        int height() const { return h; }
        int depth()  const { return d; }
        bool contains(int i, int j, int k)       const { return (i >= 0) && (i < w) && (j >= 0) && (j < h) && (k >= 0) && (k < d); }
        bool contains(float x, float y, float z) const { return (x >= 0) && (x < w) && (y >= 0) && (y < h) && (z >= 0) && (z < d); }
        
        virtual GLint defaultTexFormat() const = 0;
        virtual GLenum textureMode() const { return GL_TEXTURE_3D; }
        virtual void loadToGPU(GLint texFormat) const { loadTexture3D(texFormat, GL_TEXTURE_3D); }
        virtual void loadTexture3D(GLint texFormat, GLenum target = GL_TEXTURE_3D) const = 0;

        enum WrapMode { CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT };
        
    protected:
        int w, h, d;
    };

/////////////////////////////////////////////////

template<class Color>
class Image3D : public AbstractImage3D {
    public:
        Image3D(int w=0, int h=0, int d=0, Color* data=NULL);    //@ params wrap
        Image3D(QString fileName);
        virtual ~Image3D() {}

        virtual bool loaded() const { return data != NULL; }
        virtual void destroy();
        Image3D<Color> clone() const;
        void save(QString fileName) const;
        
        virtual GLint defaultTexFormat() const { return Color::TEX_FORMAT; }
        virtual void loadTexture3D(GLint texFormat, GLenum target = GL_TEXTURE_3D) const;

        // acquisition d'une texture 3D :
        static Image3D<Color> readTexture(Texture *tex);
        
        // définition de l'échantillonnage aux bords de l'image :
        void setBorderColor(Color border);
        void setWrapMode(WrapMode wrapMode);
        void setupBorder(WrapMode wrapMode, Color border);
        
        // fonctions d'acces aux pixels :
        //-------------------------------
        inline       Color& texel(int i, int j, int k)       { return data[i + (j + k*h)*w]; }    // précondition : this->contains(i,j,k)
        inline const Color& texel(int i, int j, int k) const { return data[i + (j + k*h)*w]; }    // précondition : this->contains(i,j,k)
        inline       Color& operator()(int i, int j, int k)       { return texel(i,j,k); }
        inline const Color& operator()(int i, int j, int k) const { return texel(i,j,k); }
        Color sample(int i, int j, int k) const;                // si !this->contains(i,j,k), le résultat dépend de <borderColor> et de <wrapMode>
        Color interp(float x, float y, float z) const;          // interpolation trilinéaire : image mappée sur [0,w[*[0,h[*[0,d[
        Color operator()(float x, float y, float z) const { return interp(x,y,z); }
        
        // fonctions de traitement de l'image :
        //-------------------------------------
        void subSample(int factor);                             // précondition : <factor> divise 'width()', 'height()' et 'depth()',
        void subSample(int factor, Pix3DEvaluator &pixeval);    // précondition : <factor> divise 'width()', 'height()' et 'depth()'
        void growClass(Pix3DEvaluator &pixeval, int value);     // étend la classe de pixels <value>
        void applyFilter(ColorFilter<Color> &filter);                                                  // applique le filtre <filter> à tous les pixels
        void applyFilter(Pix3DEvaluator &pixeval, int value, ColorFilter<Color> &filter);              // de même mais uniquement sur les pixels de classe <value>
        void applyFilter(Pix3DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2);   // de même avec <filter2> appliqué sur les pixels restants
        void applyKernel(Kernel3D kernel, bool ignoreBorders = true);   // si <ignoreBorders>==false, les valeurs de bords sont spécifiées par le <wrapMode>
        int* histogram(Pix3DEvaluator &pixeval) const;                  // retourne un tableau de taille pixeval->valuesNumber()

    private:
        Color *data;
        Color borderColor;
        WrapMode wrapMode;
        
        static const QString defaultDirName;    // répertoire par défaut pour les images 3D
    };

typedef Image3D<UByte4> Image3DUByte4;
typedef Image3D<Float1> Image3DFloat;
typedef Image3D<Float3> Image3DFloat3;
typedef Image3D<Float4> Image3DFloat4;

BABLIB_NAMESPACE_END

#include "Image3D_impl.h"

#endif

