#include "Pix2DEvaluator.h"
#include "ColorAdder.h"
#include "ImageFunctions.h"
#include "ViewportGL.h"
#include "Message.h"
#include <math.h>

BABLIB_NAMESPACE_BEGIN

/*********************************************************************************************/
// Image2D :

template<class Color>
Image2D<Color>::Image2D(int w, int h, Color* inputData) : AbstractImage2D(w, h), data(inputData), wrapMode(CLAMP_TO_EDGE) {
    if (w <= 0 || h <= 0) {
        w = h = 0;
        data = NULL;
        }
    else if (inputData == NULL) {
        data = new Color[w * h];
        if (data == NULL) {
            Message::error("l'allocation memoire pour l'image a echouÃÂ©");
            w = h = 0;
            }
        }
    }

template<class Color>
Image2D<Color>::Image2D(QImage image) : AbstractImage2D(0,0), data(NULL), wrapMode(CLAMP_TO_EDGE) {
    initialize(image);
    }

template<class Color>
Image2D<Color>::Image2D(QString fileName) : AbstractImage2D(0,0), data(NULL), wrapMode(CLAMP_TO_EDGE) {
    QImage image(fileName);
    if (image.isNull())
        Message::error(QString("probleme lors du chargement du fichier '%1'").arg(fileName));
    else
        initialize(image);
    }

template<class Color>
void Image2D<Color>::destroy() {
    if (data != NULL) delete[] data;
    data = NULL;
    w = h = 0;
    }

template<class Color>
void Image2D<Color>::initialize(QImage image) {
    destroy();
    if (image.isNull())
        Message::error("l'image fournie ne contient pas de donnÃÂ©es");
    else {
        w = image.width();
        h = image.height();
        data = new Color[w * h];
        if (data == NULL) {
            Message::error("l'allocation memoire pour l'image a echouÃÂ©");
            w = h = 0;
            }
        else
            for (int i=0; i<w; i++)
                for (int j=0; j<h; j++)
                    texel(i,j) = Color(image.pixel(i, h-1-j));  // inversion des ordonnÃÂ©es
        }
    }

template<class Color>
QImage Image2D<Color>::toQImage() const {
    QImage image(w, h, QImage::Format_ARGB32);
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
            image.setPixel(i, j, texel(i,h-1-j).toQRgb());      // inversion des ordonnÃÂ©es
    return image;
    }

template<class Color>
Image2D<Color> Image2D<Color>::loadPNG16(QString fileName) {
    int w, h;
    float *data = loadPNG16Data(fileName, Color::SIZE, w, h);
    return (data == NULL) ? Image2D<Color>() : Image2D<Color>(w, h, (Color*)data);
    }

template<class Color>
void Image2D<Color>::savePNG16(QString fileName) const {
    savePNG16Data(fileName, Color::SIZE, w, h, reinterpret_cast<float*>(data));
    }


template<class Color>
Image2D<Color> Image2D<Color>::clone() const {
    if (data == NULL) return Image2D<Color>();
    Color *clonedData = new Color[w * h];
    if (clonedData == NULL) {
        Message::error("l'allocation memoire pour l'image a echouÃÂ©");
        return Image2D<Color>();
        }
    for (int c=0; c<w*h; c++)
        clonedData[c] = data[c];
    return Image2D<Color>(w, h, clonedData);
    }

template<class Color>
Image2D<Color> Image2D<Color>::subImage(int i0, int j0, int ws, int hs) const {
    Image2D<Color> res(ws, hs);
    for (int i=0; i<ws; i++)
        for (int j=0; j<hs; j++)
            res.texel(i,j) = texel(i0+i, j0+j);
    return res;
    }

template<class Color>
Image2D<Color> Image2D<Color>::supImage(int i0, int j0, int ws, int hs) const {
    Image2D<Color> res(ws, hs);
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
            res.texel(i0+i, j0+j) = texel(i, j);
    return res;
    }

template<class Color>
Image2D<Color> Image2D<Color>::boundingPowerOfTwo() const {
    int ws, hs;
    for (ws = 1; ws < w; ws *= 2) ;
    for (hs = 1; hs < h; hs *= 2) ;
    return supImage(0,0,ws,hs);
    }

template<class Color>
void Image2D<Color>::setBorderColor(Color c) {
    borderColor = c;
    }

template<class Color>
void Image2D<Color>::setWrapMode(WrapMode mode) {
    wrapMode = mode;
    }

template<class Color>
void Image2D<Color>::setupBorder(WrapMode mode, Color c) {
    wrapMode = mode;
    borderColor = c;
    }

template<class Color>
Color Image2D<Color>::sample(int i, int j) const {
    switch (wrapMode) {
        case CLAMP_TO_BORDER :  return contains(i, j) ? texel(i,j) : borderColor;
        case REPEAT :           return texel(modulo(i, w), modulo(j, h));
        case MIRRORED_REPEAT :  return texel(mirror(i,w), mirror(j,h));
        case CLAMP_TO_EDGE :
        default :               return texel(clamp(i, 0, w-1), clamp(j, 0, h-1));
        };
    }

template<class Color>
Color Image2D<Color>::interp(float x, float y) const {
    int i0 = (int)(floorf(x - 0.5)),
        j0 = (int)(floorf(y - 0.5));
    float a = x - 0.5 - i0,
          b = y - 0.5 - j0;
    return (1-a) * (1-b) * sample(i0, j0  ) + a * (1-b) * sample(i0+1, j0  )
         + (1-a) *    b  * sample(i0, j0+1) + a *    b  * sample(i0+1, j0+1);
    }

template<class Color>
void Image2D<Color>::clear(Color c) {
    for (int i=0; i<w*h; i++) data[i] = c;
    }


/*********************************************************************************************/
// fonctions de lecture/ÃÂ©criture de buffer OpenGL :
template<class Color>
void Image2D<Color>::loadTexture2D(GLenum texFormat, GLenum target) const {
    glTexImage2D(target, 0, texFormat, w, h, 0, Color::DATA_FORMAT, Color::DATA_TYPE, data);
    }

template<class Color>
Image2D<Color> Image2D<Color>::getTexture(const Texture *tex) {
    int w = tex->getWidth(),
        h = tex->getHeight();
    Image2D<Color> res(w, h, NULL);
    if (w < 0 || h < 0)
        Message::error("dimensions de la texture non definies");
    else {
        tex->bind();
        glGetTexImage(GL_TEXTURE_2D, 0, Color::DATA_FORMAT, Color::DATA_TYPE, res.data);
        }
    return res;
    }

template<class Color>
void Image2D<Color>::readTexture(const Texture *tex) {
    int wTex = tex->getWidth(),
        hTex = tex->getHeight();
    if (wTex < 0 || hTex < 0)
        Message::error("dimensions de la texture non definies");
    else if
        (wTex != w && hTex != h)
        Message::error("dimensions de la texture differentes de celles de l'image");
    else {
        tex->bind();
        glGetTexImage(GL_TEXTURE_2D, 0, Color::DATA_FORMAT, Color::DATA_TYPE, data);
        }
    }

template<class Color>
Image2D<Color> Image2D<Color>::getColorBuffer() {
    ViewportGL view;    // rÃÂ©cupÃÂ¨re le viewport courant
    Image2D<Color> res(view.w, view.h, NULL);
    glReadPixels(0, 0, view.w, view.h, Color::DATA_FORMAT, Color::DATA_TYPE, res.data);
    return res;
    }

template<class Color>
Image2D<Color> Image2D<Color>::getDepthBuffer() {
    ViewportGL view;    // rÃÂ©cupÃÂ¨re le viewport courant
    Image2D<Color> res(view.w, view.h, NULL);
    glReadPixels(0, 0, view.w, view.h, GL_DEPTH_COMPONENT, Color::DATA_TYPE, res.data);
    return res;
    }

template<class Color>
void Image2D<Color>::readColorBuffer(int i, int j) {
    glReadPixels(i, j, w, h, Color::DATA_FORMAT, Color::DATA_TYPE, data);
    }

template<class Color>
void Image2D<Color>::readDepthBuffer(int i, int j) {
    glReadPixels(i, j, w, h, GL_DEPTH_COMPONENT, Color::DATA_TYPE, data);
    }

template<class Color>
Color Image2D<Color>::readPixelColor(int i, int j) {
    Color pix;
    glReadPixels(i, j, 1, 1, Color::DATA_FORMAT, Color::DATA_TYPE, &pix);
    return pix;
    }
        
template<class Color>
void Image2D<Color>::drawColorBuffer(int i, int j) const {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ViewportGL view;
    view.pushScreenMatrices();
        glRasterPos2i(i, j);
        glDisable(GL_DEPTH_TEST);
        glDrawPixels(w, h, Color::DATA_FORMAT, Color::DATA_TYPE, data);
    view.popScreenMatrices();
    glPopAttrib();
    }

/*********************************************************************************************/

template<class Color>
Texture Image2D<Color>::createTex2D(QString fileName, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Image2D<Color> image(fileName);
    Texture tex(&image, interpMode, wrapMode);
    tex.load(internalFormat);
    image.destroy();
    return tex;
    }
        
/***********************************************************************************************/
// fonctions de traitement d'images :

template<class Color>
void Image2D<Color>::subSample(int N) {
    int w = width()  / N,
        h = height() / N;
    Image2D<Color> res(w, h);

    // sous-ÃÂ©chantillonnage uniforme :
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        pix.reset();
        for (int a=0; a<N; a++) for (int b=0; b<N; b++)
            pix.add(texel(N*i+a, N*j+b));
        res(i,j) = pix.getColor();
        }
    
    destroy();
    *this = res;
    }

template<class Color>
void Image2D<Color>::subSample(int N, Pix2DEvaluator &pixeval) {
    int w = width()  / N,
        h = height() / N;
    Image2D<Color> res(w, h);
    
    // sous-ÃÂ©chantillonnage en sÃÂ©parant les pixels de l'image en diffÃÂ©rentes classes :
    int M = pixeval.valuesNumber();
    ColorAdder<Color> *pix = new ColorAdder<Color>[M];
    
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        for (int k=0; k<M; k++)
            pix[k].reset();
        
        for (int a=0; a<N; a++) for (int b=0; b<N; b++) {
            int k = pixeval.evaluate(N*i + a, N*j + b);
            pix[k].add(texel(N*i+a, N*j+b));
            }

        int kMax = 0;
        for (int k=0; k<M; k++) if (pix[k].n > pix[kMax].n) kMax = k;   // la classe la plus reprÃÂ©sentÃÂ©e
        res(i,j) = pix[kMax].getColor();
        }
    
    delete pix;

    destroy();
    *this = res;
    }


template<class Color>
void Image2D<Color>::growClass(Pix2DEvaluator &pixeval, int value) {
    Image2D<Color> res(w, h);
    
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        int a1 = i > 0   ? -1 : 0,
            a2 = i < w-1 ?  1 : 0,
            b1 = j > 0   ? -1 : 0,
            b2 = j < h-1 ?  1 : 0;
    
        pix.reset();
        if (pixeval.evaluate(i,j) != value) {
            // on ajoute les ÃÂ©ventuels pixels du voisinage qui s'ÃÂ©valuent ÃÂ  'value' :
            for (int a=a1; a<=a2; a++) for (int b=b1; b<=b2; b++)
                if (pixeval.evaluate(i+a,j+b) == value)
                    pix.add(texel(i+a, j+b));
            }
        if (pix.n == 0) pix.add(texel(i, j));
        
        res(i,j) = pix.getColor();
        }

    destroy();
    *this = res;
    }

template<class Color>
void Image2D<Color>::applyFilter(ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++)
        filter.apply(texel(i,j));
    }

template<class Color>
void Image2D<Color>::applyFilter(Pix2DEvaluator &pixeval, int value, ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++)
        if (pixeval.evaluate(i,j) == value)
            filter.apply(texel(i,j));
    }

template<class Color>
void Image2D<Color>::applyFilter(Pix2DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++)
        (pixeval.evaluate(i,j) == value ? filter1 : filter2).apply(texel(i,j));
    }

template<class Color>
void Image2D<Color>::applyKernel(Kernel2D kernel, bool ignoreBorders) {
    Image2D<Color> res(w, h);
    ColorAdder<Color> pix;
    int R = kernel.radius();
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        pix.reset();
        for (int a=-R; a<=R; a++) for (int b=-R; b<=R; b++) {
            int i1 = i + a,
                j1 = j + b;
            if (!ignoreBorders || contains(i1, j1))
                pix.add(texel(i1, j1), kernel(a, b));
            }
        //res(i,j) = pix.getColor();
        res(i,j) = pix.getSum();
        //@@ a voir: gestion des bords (plus simplement gerÃÂ© automatiquement par adder)
        }
    destroy();
    *this = res;
    }

template<class Color>
int* Image2D<Color>::histogram(Pix2DEvaluator &pixeval) const {
    const int N = pixeval.valuesNumber();
    int* res = new int[N];
    for (int k=0; k<N; k++) res[k] = 0;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++)
        res[pixeval.evaluate(i,j)] ++;
    return res;
    }

BABLIB_NAMESPACE_END

