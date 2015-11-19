#ifndef __TRANSFO_GL__
#define __TRANSFO_GL__

#include "ProjTransfo.h"
#include "Vec4.h"
#include <QGLViewer/camera.h>

BABLIB_NAMESPACE_BEGIN

class TransfoGL {
    public:
        TransfoGL(bool getFromGL = true);
        TransfoGL(ProjTransfo modelView, ProjTransfo projection);
        TransfoGL(const qglviewer::Camera &camera);
        
        void load() const;
        void save();
        void pushAndLoad() const;
        void saveAndPop();
        static TransfoGL* get();
        
        static void push();
        static void pop();
        static void pushAndInit();  // met les matrices à l'identité
        // effet de bord de ces trois méthodes : le matrix-mode est GL_PROJECTION
        
        ProjTransfo transfo() const;
        Vec4 viewPoint() const;         // renvoie un point à l'infini si la projection est orthogonale
        float screenRatio() const;      // retourne le rapport hauteur/largeur de l'écran correspondant à cette transformation
        
    public:
        ProjTransfo modelView, projection;
    };

BABLIB_NAMESPACE_END

#endif

