#include "Pix3DEvaluator.h"
#include "ColorAdder.h"
#include "ImageFunctions.h"
#include "Dir.h"
#include "Message.h"
#include "Params.h"
#include <math.h>

BABLIB_NAMESPACE_BEGIN

/*********************************************************************************************/
// Image3D :

template<class Color>
Image3D<Color>::Image3D(int w, int h, int d, Color* inputData) : AbstractImage3D(w, h, d), data(inputData), wrapMode(CLAMP_TO_EDGE) {
    if (w <= 0 || h <= 0 || d <= 0) {
        w = h = d = 0;
        data = NULL;
        }
    else if (inputData == NULL) {
        data = new Color[w * h * d];
        if (data == NULL) {
            Message::error("l'allocation memoire pour l'image a echoué");
            w = h = d = 0;
            }
        }
    }

template<class Color>
void Image3D<Color>::destroy() {
    if (data != NULL) delete[] data;
    data = NULL;
    w = h = d = 0;
    }

template<class Color>
Image3D<Color> Image3D<Color>::clone() const {
    if (data == NULL) return Image3D<Color>();
    Color *clonedData = new Color[w * h * d];
    if (clonedData == NULL) {
        Message::error("l'allocation memoire pour l'image a echoué");
        return Image3D<Color>();
        }
    for (int c=0; c<w*h*d; c++)
        clonedData[c] = data[c];
    return Image3D<Color>(w, h, d, clonedData);
    }

template<class Color>
void Image3D<Color>::setBorderColor(Color c) {
    borderColor = c;
    }

template<class Color>
void Image3D<Color>::setWrapMode(WrapMode mode) {
    wrapMode = mode;
    }

template<class Color>
void Image3D<Color>::setupBorder(WrapMode mode, Color c) {
    wrapMode = mode;
    borderColor = c;
    }

template<class Color>
Color Image3D<Color>::sample(int i, int j, int k) const {
    switch (wrapMode) {
        case CLAMP_TO_BORDER :  return contains(i, j, k) ? texel(i,j,k) : borderColor;
        case REPEAT :           return texel(modulo(i, w), modulo(j, h), modulo(k, d));
        case MIRRORED_REPEAT :  return texel(mirror(i,w), mirror(j,h), mirror(k,d));
        case CLAMP_TO_EDGE :
        default :               return texel(clamp(i, 0, w-1), clamp(j, 0, h-1), clamp(k, 0, d-1));
        };
    }

template<class Color>
Color Image3D<Color>::interp(float x, float y, float z) const {
    int i0 = (int)(floorf(x - 0.5)),
        j0 = (int)(floorf(y - 0.5)),
        k0 = (int)(floorf(z - 0.5));
    float a = x - 0.5 - i0,
          b = y - 0.5 - j0,
          c = z - 0.5 - k0;
    return (1-a) * (1-b) * (1-c) * sample(i0, j0  , k0  ) + a * (1-b) * (1-c) * sample(i0+1, j0  , k0  )
         + (1-a) * (1-b) *    c  * sample(i0, j0  , k0+1) + a * (1-b) *    c  * sample(i0+1, j0  , k0+1)
         + (1-a) *    b  * (1-c) * sample(i0, j0+1, k0  ) + a *    b  * (1-c) * sample(i0+1, j0+1, k0  )
         + (1-a) *    b  *    c  * sample(i0, j0+1, k0+1) + a *    b  *    c  * sample(i0+1, j0+1, k0+1);
    }

/*********************************************************************************************/
// fonction de chargement de texture :

template<class Color>
void Image3D<Color>::loadTexture3D(GLint texFormat, GLenum target) const {
    glTexImage3D(target, 0, texFormat, w, h, d, 0, Color::DATA_FORMAT, Color::DATA_TYPE, data);
    }

template<class Color>
Image3D<Color> Image3D<Color>::readTexture(Texture *tex) {
    int w = tex->getWidth(),
        h = tex->getHeight(),
        d = tex->getDepth();
    Image3D<Color> res(w, h, d, NULL);
    if (w < 0 || h < 0 || d < 0)
        Message::error("dimensions de la texture non definies");
    else {
        tex->bind();
        glGetTexImage(GL_TEXTURE_3D, 0, Color::DATA_FORMAT, Color::DATA_TYPE, res.data);
        }
    return res;
    }

/*********************************************************************************************/
// fonctions de chargement/sauvegarde de fichiers :

template<class Color>
const QString Image3D<Color>::defaultDirName = GlobalParams::getPath("bablib.images3D.default.dir");

template<class Color>
Image3D<Color>::Image3D(QString fileName) : AbstractImage3D(0,0,0), data(NULL), wrapMode(CLAMP_TO_EDGE) {
    //@@@ utiliser Dir
    Dir dir(defaultDirName, fileName);
    if (!dir.exists()) {
        Message::error(QString("le sous-répertoire '%1' n'existe pas").arg(fileName));
        return;
        }
    //dir.setNameFilters(QStringList("*.png"));
    //QStringList fileNamesList = dir.entryList(QDir::Files, QDir::Name);
    QStringList fileNamesList = dir.getFileList("*.png");
    
    d = fileNamesList.size();
    if (d == 0) {
        Message::error(QString("aucun fichier image dans le répertoire '%1'").arg(fileName));
        return;
        }
    
    QImage slice0(dir.filePath(fileNamesList[0]));
    w = slice0.width();
    h = slice0.height();
    data = new Color[w * h * d];
    if (data == NULL) {
        Message::error("memoire insuffisante pour charger l'image 3D en memoire");
        w = h = d = 0;
        return;
        }

    for (int k=0; k<d; k++) {
        QImage slice(dir.filePath(fileNamesList[k]));
        // on vérifie que les slices ont bien la même taille :
        if (slice.width() != w || slice.height() != h) {
            Message::error("les slices d'une image 3D doivent avoir toutes la même taille");
            destroy();
            return;
            }
        // on recopie la slice dans l'image 3D :
        for (int i=0; i<w; i++) for (int j=0; j<h; j++)
            texel(i,j,k) = Color(slice.pixel(i, h-1-j));    // inversion des ordonnées
        }
    }

template<class Color>
void Image3D<Color>::save(QString fileName) const {
    if (!loaded()) {
        Message::error("l'image ne contient pas de donnees");
        return;
        }
    
    Dir root(defaultDirName);
    Dir dir = root.writingDir(fileName, true, true);   // les éventuels fichiers que contient le sous-répertoire sont effacés
    if (!dir.isDefined()) return;
    
    for (int k=0; k<d; k++) {
        QImage slice(w, h, QImage::Format_ARGB32);
        for (int i=0; i<w; i++) for (int j=0; j<h; j++)
            slice.setPixel(i, h-1-j, texel(i,j,k).toQRgb());    // inversion des ordonnées
        dir.saveImage(slice, QString("slice%1.png").arg(k, 3, 10, QLatin1Char('0')));
        }
    }

/*********************************************************************************************/
// fonctions de traitement d'images :

template<class Color>
void Image3D<Color>::subSample(int N) {
    int w = width()  / N,
        h = height() / N,
        d = depth() / N;
    Image3D<Color> res(w, h, d);

    // sous-échantillonnage uniforme :
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++) {
        pix.reset();
        for (int a=0; a<N; a++) for (int b=0; b<N; b++) for (int c=0; c<N; c++)
            pix.add(texel(N*i+a, N*j+b, N*k+c));
        res(i,j,k) = pix.getColor();
        }
    
    destroy();
    *this = res;
    }

template<class Color>
void Image3D<Color>::subSample(int N, Pix3DEvaluator &pixeval) {
    int w = width()  / N,
        h = height() / N,
        d = depth() / N;
    Image3D<Color> res(w, h, d);
    
    // sous-échantillonnage en séparant les pixels de l'image en différentes classes :
    int M = pixeval.valuesNumber();
    ColorAdder<Color> *pix = new ColorAdder<Color>[M];
    
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++) {
        for (int m=0; m<M; m++)
            pix[m].reset();
        
        for (int a=0; a<N; a++) for (int b=0; b<N; b++) for (int c=0; c<N; c++) {
            int m = pixeval.evaluate(N*i + a, N*j + b, N*k + c);
            pix[m].add(texel(N*i+a, N*j+b, N*k+c));
            }

        int mMax = 0;
        for (int m=0; m<M; m++) if (pix[m].n > pix[mMax].n) mMax = m;   // la classe la plus représentée
        res(i,j,k) = pix[mMax].getColor();
        }
    
    delete pix;

    destroy();
    *this = res;
    }


template<class Color>
void Image3D<Color>::growClass(Pix3DEvaluator &pixeval, int value) {
    Image3D<Color> res(w, h, d);
    
    ColorAdder<Color> pix;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++) {
        int a1 = i > 0   ? -1 : 0,
            a2 = i < w-1 ?  1 : 0,
            b1 = j > 0   ? -1 : 0,
            b2 = j < h-1 ?  1 : 0,
            c1 = k > 0   ? -1 : 0,
            c2 = k < d-1 ?  1 : 0;
    
        pix.reset();
        if (pixeval.evaluate(i,j,k) != value) {
            // on ajoute les éventuels pixels du voisinage qui s'évaluent à 'value' :
            for (int a=a1; a<=a2; a++) for (int b=b1; b<=b2; b++) for (int c=c1; c<=c2; c++)
                if (pixeval.evaluate(i+a,j+b,k+c) == value)
                    pix.add(texel(i+a, j+b, k+c));
            }
        if (pix.n == 0) pix.add(texel(i, j, k));
        
        res(i,j,k) = pix.getColor();
        }

    destroy();
    *this = res;
    }

template<class Color>
void Image3D<Color>::applyFilter(ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++)
        filter.apply(texel(i,j,k));
    }

template<class Color>
void Image3D<Color>::applyFilter(Pix3DEvaluator &pixeval, int value, ColorFilter<Color> &filter) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++)
        if (pixeval.evaluate(i,j,k) == value)
            filter.apply(texel(i,j,k));
    }

template<class Color>
void Image3D<Color>::applyFilter(Pix3DEvaluator &pixeval, int value, ColorFilter<Color> &filter1, ColorFilter<Color> &filter2) {
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++)
        (pixeval.evaluate(i,j,k) == value ? filter1 : filter2).apply(texel(i,j,k));
    }

template<class Color>
void Image3D<Color>::applyKernel(Kernel3D kernel, bool ignoreBorders) {
    Image3D<Color> res(w, h, d);
    ColorAdder<Color> pix;
    int R = kernel.radius();
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++) {
        pix.reset();
        for (int a=-R; a<=R; a++) for (int b=-R; b<=R; b++) for (int c=-R; c<=R; c++) {
            int i1 = i + a,
                j1 = j + b,
                k1 = k + c;
            if (!ignoreBorders || contains(i1, j1, k1))
                pix.add(texel(i1, j1, k1), kernel(a, b, c));
            }
        //res(i,j,k) = pix.getColor();
        res(i,j,k) = pix.getSum();
        }
    destroy();
    *this = res;
    }

template<class Color>
int* Image3D<Color>::histogram(Pix3DEvaluator &pixeval) const {
    const int N = pixeval.valuesNumber();
    int* res = new int[N];
    for (int m=0; m<N; m++) res[m] = 0;
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) for (int k=0; k<d; k++)
        res[pixeval.evaluate(i,j,k)] ++;
    return res;
    }

BABLIB_NAMESPACE_END

