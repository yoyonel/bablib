#ifndef __IMAGE2D__
#define __IMAGE2D__

//#include <OpenGL.h>
#include "TextureData.h"
#include "ColorFilter.h"
#include "ColorTypes.h"
#include "Kernel2D.h"
#include "Texture.h"
#include "Image1D.h"
#include <QImage>

BABLIB_NAMESPACE_BEGIN
class Pix2DEvaluator;

// systÃÂ¨me de coordonnÃÂ©es : comme OpenGL :
//----------------------------------------
//  - i : abscisse, de gauche ÃÂ  droite
//  - j : ordonnÃÂ©e, de bas en haut
//  - origine en bas ÃÂ  gauche
// ==> attention : systÃÂ¨me inversÃÂ© par rapport ÃÂ  QImage (origine en haut ÃÂ  gauche)

#define for_all_pixels for (int i=0; i<w; i++) for (int j=0; j<h; j++)
#define for_image2D(image_name, index1_name, index2_name) \
    for (int index1_name=0; index1_name<image_name.width(); index1_name++)  \
    for (int index2_name=0; index2_name<image_name.height(); index2_name++)

class AbstractImage2D : public TextureData {
    public:
        AbstractImage2D(int w=0, int h=0) : w(w), h(h) {}
        virtual ~AbstractImage2D() {}

        virtual bool loaded() const = 0;    // true ssi l'image contient des donnÃÂ©es
        virtual void destroy() = 0;         // libÃÂ¨re la mÃÂ©moire ÃÂ©ventuellement occupÃÂ©e par l'image
        
        virtual QImage toQImage() const = 0;
        virtual void initialize(QImage image) = 0;      // initialise l'image ÃÂ  partir de <image>
        
        void save(QString fileName) const;              // sauvegarde l'image dans un format donnÃÂ© par l'extension du nom de fichier

        int width()  const { return w; }
        int height() const { return h; }
        bool contains(int i, int j)     const { return (i >= 0) && (i < w) && (j >= 0) && (j < h); }
        bool contains(float x, float y) const { return (x >= 0) && (x < w) && (y >= 0) && (y < h); }
        
        virtual GLenum textureMode() const { return GL_TEXTURE_2D; }
        virtual GLint defaultTexFormat() const = 0;
        virtual void loadToGPU(GLint texFormat) const { loadTexture2D(texFormat, GL_TEXTURE_2D); }
        virtual void loadTexture2D(GLenum texFormat, GLenum target = GL_TEXTURE_2D) const = 0;

        enum WrapMode { CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT };
        
    protected:
        static float* loadPNG16Data(QString fileName, int channels, int &w, int &h);
        static void savePNG16Data(QString fileName, int channels, int w, int h, const float *data);
        
    protected:
        int w, h;
    };

/////////////////////////////////////////////////

template<class Color>
class Image2D : public AbstractImage2D {
    public:
        Image2D(int w=0, int h=0, Color* data=NULL);    //@ params wrap
        Image2D(QImage image);
        Image2D(QString fileName);
        virtual ~Image2D() {}

        static Image2D<Color> loadPNG16(QString fileName); // seulement pour Color = Float*, PNG 1,2,4,8,16 bits, 1,2,3,4 canaux, format PNG_COLOR_TYPE_PALETTE non supportÃÂ©
        void savePNG16(QString fileName) const;            // seulement pour Color = Float*
        
        virtual bool loaded() const { return data != NULL; }
        virtual void destroy();
        void clear(Color c = Color());
        
        virtual QImage toQImage() const;
        virtual void initialize(QImage image);      // initialise l'image ÃÂ  partir de <image>
        Image2D<Color> clone() const;               // duplique l'image en une nouvelle image
        Image2D<Color> subImage(int i, int j, int w, int h) const;  // prÃÂ©condition : i >= 0 && i+w <= width() && j >= 0 && j+h <= height()
        Image2D<Color> supImage(int i, int j, int w, int h) const;  // prÃÂ©condition : i >= 0 && i+width() <= w && j >= 0 && j+height() <= h
        Image2D<Color> boundingPowerOfTwo() const;  // retourne l'image englobant l'image courante, ayant des dimensions puissances de 2
        
        Image1D<Color> line(int j) const { return Image1D<Color>(w, data + j*w); }     // retourne l'Image1D rÃÂ©fÃÂ©renÃÂ§ant la ligne <j>
        
        virtual GLint defaultTexFormat() const { return Color::TEX_FORMAT; }
        virtual void loadTexture2D(GLenum texFormat, GLenum target = GL_TEXTURE_2D) const;

        // lecture/ÃÂ©criture du buffer de couleurs OpenGL :
        static Image2D<Color> getColorBuffer();
        static Image2D<Color> getDepthBuffer();         // ne marche correctement que si le format <Color> a une seule composante
        void readColorBuffer(int i=0, int j=0);         // prÃÂ©condition : loaded() == true et le buffer contient le viewport (i,j,w,h)
        void readDepthBuffer(int i=0, int j=0);         // de mÃÂªme mais le format <Color> doit avoir une seule composante
        void drawColorBuffer(int i=0, int j=0) const;   // point (i,j) en unitÃÂ©s de pixels, origine en bas ÃÂ  gauche
        static Color readPixelColor(int i, int j);
        
        // acquisition d'une texture 2D :
        static Image2D<Color> getTexture(const Texture *tex);
        void readTexture(const Texture *tex);           // prÃÂ©condition : l'image a la mÃÂªme taille que la texture
        
        // chargment direct d'une texture :
        static Texture createTex2D(QString fileName, GLint internalFormat = GL_RGBA, GLenum interpMode = GL_NEAREST, GLenum wrapMode = GL_CLAMP);

        // dÃÂ©finition de l'ÃÂ©chantillonnage aux bords de l'image :
        void setBorderColor(Color border);
        void setWrapMode(WrapMode wrapMode);
        void setupBorder(WrapMode wrapMode, Color border);
        
        // fonctions d'acces aux pixels :
        //-------------------------------
        inline       Color& texel(int i, int j)       { return data[i + j*w]; }    // prÃÂ©condition : this->contains(i,j)
        inline const Color& texel(int i, int j) const { return data[i + j*w]; }    // prÃÂ©condition : this->contains(i,j)
        inline       Color& operator()(int i, int j)       { return texel(i,j); }
        inline const Color& operator()(int i, int j) const { return texel(i,j); }
        Color sample(int i, int j) const;               // si !this->contains(i,j), le rÃÂ©sultat dÃÂ©pend de <borderColor> et de <wrapMode>
        Color interp(float x, float y) const;           // interpolation bilinÃÂ©aire : image mappÃÂ©e sur [0,w[*[0,h[
        inline Color operator()(float x, float y) const { return interp(x,y); }
        inline const Color* mem() const { return data; }
        inline       Color* mem()       { return data; }
        
        // fonctions de traitement de l'image :
        //-------------------------------------
        void subSample(int factor);                             // prÃÂ©condition : <factor> divise 'width()' et 'height()'
        void subSample(int factor, Pix2DEvaluator &pixeval);    // prÃÂ©condition : <factor> divise 'width()' et 'height()'
        void growClass(Pix2DEvaluator &pixeval, int value);     // ÃÂ©tend la classe de pixels <value>
        void applyFilter(ColorFilter<Color> &filter);                                                  // applique le filtre <filter> ÃÂ  tous les pixels
        void applyFilter(Pix2DEvaluator &pixeval, int value, ColorFilter<Color> &filter);              // de mÃÂªme mais uniquement sur les pixels de classe <value>
        void applyFilter(Pix2DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2);   // de mÃÂªme avec <filter2> appliquÃÂ© sur les pixels restants
        void applyKernel(Kernel2D kernel, bool ignoreBorders = true);   // si <ignoreBorders>==false, les valeurs de bords sont spÃÂ©cifiÃÂ©es par le <wrapMode>
        int* histogram(Pix2DEvaluator &pixeval) const;                  // retourne un tableau de taille pixeval->valuesNumber()

    private:
        Color *data;
        Color borderColor;
        WrapMode wrapMode;
    };

typedef Image2D<UByte4> Image2DUByte4;
typedef Image2D<Float1> Image2DFloat;
typedef Image2D<Float2> Image2DFloat2;
typedef Image2D<Float3> Image2DFloat3;
typedef Image2D<Float4> Image2DFloat4;

BABLIB_NAMESPACE_END

#include "Image2D_impl.h"

#endif

