#include "Pix1DEvaluator.h"
#include "ColorAdder.h"
#include "ImageFunctions.h"
#include "ViewportGL.h"
#include "Message.h"
#include <math.h>

BABLIB_NAMESPACE_BEGIN

/*********************************************************************************************/
// Image1D :

template<class Color>
Image1D<Color>::Image1D(int w, Color* inputData, WrapMode wrap, Color border) : AbstractImage1D(w), data(inputData), wrapMode(wrap), borderColor(border) {
    if (w <= 0) {
        w = 0;
        data = NULL;
        }
    else if (inputData == NULL) {
        data = new Color[w];
        if (data == NULL) {
            Message::error("l'allocation memoire pour l'image a echouÃÂ©");
            w = 0;
            }
        }
    }

template<class Color>
Image1D<Color>::Image1D(QImage image) : AbstractImage1D(0), data(NULL), wrapMode(CLAMP_TO_EDGE) {
    initialize(image);
    }

template<class Color>
Image1D<Color>::Image1D(QString fileName) : AbstractImage1D(0), data(NULL), wrapMode(CLAMP_TO_EDGE) {
    QImage image(fileName);
    if (image.isNull())
        Message::error(QString("probleme lors du chargement du fichier '%1'").arg(fileName));
    else
        initialize(image);
    }

template<class Color>
void Image1D<Color>::destroy() {
    if (data != NULL) delete[] data;
    data = NULL;
    w = 0;
    }

template<class Color>
void Image1D<Color>::initialize(QImage image) {
    destroy();
    if (image.isNull())
        Message::error("l'image fournie ne contient pas de donnÃÂ©es");
    else {
        w = image.width();
        data = new Color[w];
        if (data == NULL) {
            Message::error("l'allocation memoire pour l'image a echouÃÂ©");
            w = 0;
            }
        else
            for (int i=0; i<w; i++)
                texel(i) = Color(image.pixel(i, 0));
        }
    }

template<class Color>
QImage Image1D<Color>::toQImage() const {
    QImage image(w, 1, QImage::Format_ARGB32);
    for (int i=0; i<w; i++)
        image.setPixel(i, 0, texel(i).toQRgb());
    return image;
    }

template<class Color>
void Image1D<Color>::copy(const Image1D<Color> &image) {
    if (!image.loaded())
        Message::error("l'image fournie ne contient pas de donnÃÂ©es");
    else if (w < image.width())
        Message::error("l'image source est plus grande que l'image destination");
    else {
        for (int c=0; c<w; c++) data[c] = image.data[c];
        wrapMode = image.wrapMode;
        borderColor = image.borderColor;
        //@ voir : copie wrap parametrable par bool?
        }
    }

template<class Color>
Image1D<Color> Image1D<Color>::clone() const {
    if (data == NULL) return Image1D<Color>();
    Color *clonedData = new Color[w];
    if (clonedData == NULL) {
        Message::error("l'allocation memoire pour l'image a echouÃÂ©");
        return Image1D<Color>();
        }
    for (int c=0; c<w; c++)
        clonedData[c] = data[c];
    return Image1D<Color>(w, clonedData, wrapMode, borderColor);
    }

template<class Color>
void Image1D<Color>::setBorderColor(Color c) {
    borderColor = c;
    }

template<class Color>
void Image1D<Color>::setWrapMode(WrapMode mode) {
    wrapMode = mode;
    }

template<class Color>
void Image1D<Color>::setupBorder(WrapMode mode, Color c) {
    wrapMode = mode;
    borderColor = c;
    }

template<class Color>
Color Image1D<Color>::sample(int i) const {
    switch (wrapMode) {
        case CLAMP_TO_BORDER :  return contains(i) ? texel(i) : borderColor;
        case REPEAT :           return texel(modulo(i, w));
        case MIRRORED_REPEAT :  return texel(mirror(i, w));
        case CLAMP_TO_EDGE :
        default :               return texel(clamp(i, 0, w-1));
        };
    }

template<class Color>
Color Image1D<Color>::interp(float x) const {
    int i0 = (int)(floorf(x - 0.5));
    float a = x - 0.5 - i0;
    return (1 - a) * sample(i0) + a * sample(i0 + 1);
    }

/*********************************************************************************************/
// fonctions de lecture/ÃÂ©criture de buffer OpenGL :

template<class Color>
void Image1D<Color>::loadTexture1D(GLint texFormat, GLenum target) const {
    glTexImage1D(target, 0, texFormat, w, 0, Color::DATA_FORMAT, Color::DATA_TYPE, data);
    }

template<class Color>
Image1D<Color> Image1D<Color>::readTexture(Texture *tex) {
    int w = tex->getWidth();
    Image1D<Color> res(w, NULL);
    if (w < 0)
        Message::error("dimensions de la texture non definies");
    else {
        tex->bind();
        glGetTexImage(GL_TEXTURE_1D, 0, Color::DATA_FORMAT, Color::DATA_TYPE, res.data);
        }
    return res;
    }


/*********************************************************************************************/
// fonctions de traitement d'images :

template<class Color>
void Image1D<Color>::subSample(int N) {
    int w = width() / N;
    Image1D<Color> res(w);

    // sous-ÃÂ©chantillonnage uniforme :
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) {
        pix.reset();
        for (int a=0; a<N; a++)
            pix.add(texel(N*i+a));
        res(i) = pix.getColor();
        }
    
    destroy();
    *this = res;
    }

template<class Color>
void Image1D<Color>::subSample(int N, Pix1DEvaluator &pixeval) {
    int w = width()  / N;
    Image1D<Color> res(w);
    
    // sous-ÃÂ©chantillonnage en sÃÂ©parant les pixels de l'image en diffÃÂ©rentes classes :
    int M = pixeval.valuesNumber();
    ColorAdder<Color> *pix = new ColorAdder<Color>[M];
    
    for (int i=0; i<w; i++) {
        for (int k=0; k<M; k++)
            pix[k].reset();
        
        for (int a=0; a<N; a++) {
            int k = pixeval.evaluate(N*i + a);
            pix[k].add(texel(N*i+a));
            }

        int kMax = 0;
        for (int k=0; k<M; k++) if (pix[k].n > pix[kMax].n) kMax = k;   // la classe la plus reprÃÂ©sentÃÂ©e
        res(i) = pix[kMax].getColor();
        }
    
    delete pix;

    destroy();
    *this = res;
    }


template<class Color>
void Image1D<Color>::growClass(Pix1DEvaluator &pixeval, int value) {
    Image1D<Color> res(w);
    
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) {
        int a1 = i > 0   ? -1 : 0,
            a2 = i < w-1 ?  1 : 0;
    
        pix.reset();
        if (pixeval.evaluate(i) != value) {
            // on ajoute les ÃÂ©ventuels pixels du voisinage qui s'ÃÂ©valuent ÃÂ  <value> :
            for (int a=a1; a<=a2; a++)
                if (pixeval.evaluate(i+a) == value)
                    pix.add(texel(i+a));
            }
        if (pix.n == 0) pix.add(texel(i));
        
        res(i) = pix.getColor();
        }

    destroy();
    *this = res;
    }

template<class Color>
void Image1D<Color>::applyFilter(ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++)
        filter.apply(texel(i));
    }

template<class Color>
void Image1D<Color>::applyFilter(Pix1DEvaluator &pixeval, int value, ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++)
        if (pixeval.evaluate(i) == value)
            filter.apply(texel(i));
    }

template<class Color>
void Image1D<Color>::applyFilter(Pix1DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2) {
    for (int i=0; i<w; i++)
        (pixeval.evaluate(i) == value ? filter1 : filter2).apply(texel(i));
    }

template<class Color>
void Image1D<Color>::applyKernel(Kernel1D kernel, bool ignoreBorders) {
    Image1D<Color> res(w);
    ColorAdder<Color> pix;
    int R = kernel.radius();
    for (int i=0; i<w; i++) {
        pix.reset();
        for (int a=-R; a<=R; a++) {
            int i1 = i + a;
            if (!ignoreBorders || contains(i1))
                pix.add(texel(i1), kernel(a));
            }
        //res(i) = pix.getColor();
        res(i) = pix.getSum();
        }
    destroy();
    *this = res;
    }

template<class Color>
int* Image1D<Color>::histogram(Pix1DEvaluator &pixeval) const {
    const int N = pixeval.valuesNumber();
    int* res = new int[N];
    for (int k=0; k<N; k++) res[k] = 0;
    for (int i=0; i<w; i++)
        res[pixeval.evaluate(i)] ++;
    return res;
    }

BABLIB_NAMESPACE_END

