#ifndef __IMAGE1D__
#define __IMAGE1D__

//#include <OpenGL.h>
#include "TextureData.h"
#include "ColorFilter.h"
#include "ColorTypes.h"
#include "Kernel1D.h"
#include "Texture.h"
#include <QImage>

BABLIB_NAMESPACE_BEGIN
class Pix1DEvaluator;

class AbstractImage1D : public TextureData {
    public:
        AbstractImage1D(int w=0) : w(w) {}
        virtual ~AbstractImage1D() {}

        virtual bool loaded() const = 0;
        virtual void destroy() = 0;

        virtual QImage toQImage() const = 0;
        virtual void initialize(QImage image) = 0;       // initialise l'image à partir de <image>

        void save(QString fileName) const;              // sauvegarde l'image dans un format donné par l'extension du nom de fichier
        
        int width() const { return w; }
        //int height() const { return h; }
        bool contains(int i)   const { return (i >= 0) && (i < w); }
        bool contains(float x) const { return (x >= 0) && (x < w); }
        
        virtual GLint defaultTexFormat() const = 0;
        virtual GLenum textureMode() const { return GL_TEXTURE_1D; }
        virtual void loadToGPU(GLint texFormat) const { loadTexture1D(texFormat, GL_TEXTURE_1D); }
        virtual void loadTexture1D(GLint texFormat, GLenum target = GL_TEXTURE_1D) const = 0;

        enum WrapMode { CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT };
        
        virtual int         sizeData() const = 0;
        virtual const void* ptrData()  const = 0;
        virtual void*       ptrData() = 0;
        
    protected:
        int w;  //, h;
    };

/////////////////////////////////////////////////

template<class Color>
class Image1D : public AbstractImage1D {
    public:
        Image1D(int w=0, Color* data=NULL, WrapMode wrap=CLAMP_TO_EDGE, Color border=Color());
        Image1D(QImage image);
        Image1D(QString fileName);
        virtual ~Image1D() {}

        virtual bool loaded() const { return data != NULL; }
        virtual void destroy();
        
        virtual QImage toQImage() const;
        virtual void initialize(QImage image);      // initialise l'image à partir de <image>
        Image1D<Color> clone() const;               // duplique l'image en une nouvelle image
        void copy(const Image1D<Color> &image);     // initialise l'image à partir de <image>  (précondition : width() >= image.width())
        
        virtual GLint defaultTexFormat() const { return Color::TEX_FORMAT; }
        virtual void loadTexture1D(GLint texFormat, GLenum target = GL_TEXTURE_1D) const;

        // acquisition d'une texture 1D :
        static Image1D<Color> readTexture(Texture *tex);

        // définition de l'échantillonnage aux bords de l'image :
        void setBorderColor(Color border);
        void setWrapMode(WrapMode wrapMode);
        void setupBorder(WrapMode wrapMode, Color border);
        
        // fonctions d'acces aux pixels :
        //-------------------------------
        inline       Color& texel(int i)       { return data[i]; }          // précondition : this->contains(i)
        inline const Color& texel(int i) const { return data[i]; }          // précondition : this->contains(i)
        inline       Color& operator()(int i)       { return texel(i); }
        inline const Color& operator()(int i) const { return texel(i); }
        Color sample(int i) const;                                  // si !this->contains(i), le résultat dépend de <borderColor> et de <wrapMode>
        Color interp(float x) const;                                // interpolation linéaire : image mappée sur [0,w[
        Color interpUnit(float x) const { return interp(x * w); }   // interpolation linéaire : image mappée sur [0,1[
        Color operator()(float x) const { return interp(x); }
        inline const Color* mem() const { return data; }
        inline       Color* mem()       { return data; }
        
        virtual int         sizeData() const { return sizeof(Color); }
        virtual const void* ptrData()  const { return data; }
        virtual void*       ptrData()        { return data; }

        // fonctions de traitement de l'image :
        //-------------------------------------
        void subSample(int factor);                             // précondition : <factor> divise 'width()'
        void subSample(int factor, Pix1DEvaluator &pixeval);    // précondition : <factor> divise 'width()' et 'height()'
        void growClass(Pix1DEvaluator &pixeval, int value);     // étend la classe de pixels <value>
        void applyFilter(ColorFilter<Color> &filter);                                           // applique le filtre <filter> à tous les pixels
        void applyFilter(Pix1DEvaluator &pixeval, int value, ColorFilter<Color> &filter);       // de même mais uniquement sur les pixels de classe <value>
        void applyFilter(Pix1DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2);   // de même avec <filter2> appliqué sur les pixels restants
        void applyKernel(Kernel1D kernel, bool ignoreBorders = true);   // si <ignoreBorders>==false, les valeurs de bords sont spécifiées par le <wrapMode>
        int* histogram(Pix1DEvaluator &pixeval) const;                  // retourne un tableau de taille pixeval->valuesNumber()
        
    private:
        Color *data;
        WrapMode wrapMode;
        Color borderColor;
    };

typedef Image1D<UByte4> Image1DUByte4;
typedef Image1D<Float1> Image1DFloat;
typedef Image1D<Float2> Image1DFloat2;
typedef Image1D<Float3> Image1DFloat3;
typedef Image1D<Float4> Image1DFloat4;

BABLIB_NAMESPACE_END

#include "Image1D_impl.h"

#endif

