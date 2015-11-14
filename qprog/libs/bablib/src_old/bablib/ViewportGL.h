#ifndef __VIEWPORT_GL__
#define __VIEWPORT_GL__

#include "ObjectXML.h"

BABLIB_NAMESPACE_BEGIN

//@@ a voir : permettre a un viewport d'avoir un viewport pere --> hierarchie de viewports
class ViewportGL {
    public:
        ViewportGL();   // prend la valeur du viewport du contexte OpenGL courant --> précondition : contexte OpenGL créé
        ViewportGL(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
        
        OBJXML_METHODS
         
        void getGL();           // actualise les paramètres du viewport à partir de l'état OpenGL
        void setGL() const;     // actualise l'état OpenGL à partir des paramètres du viewport

        ViewportGL subViewport(int wSub, int hSub);             // retourne le sous-viewport centré de taille (wSub, hSub)
        ViewportGL subViewport(int margin, float ratio);        // retourne le sous-viewport centré avec une marge 'margin' et le rapport wSub/hSub = ratio
        ViewportGL subViewport(int margin);                     // retourne le sous-viewport une marge 'margin' aux quatre bords

        ViewportGL subViewport(ViewportGL relativeView) const;
        ViewportGL relativeViewport(ViewportGL view) const;
        
        void setUnitSquare(ViewportGL subView) const;   // règle la PROJECTION matrix pour que le carré unité se mappe sur 'subView'.
        
        void pushScreenMatrices() const;
        void popScreenMatrices() const;
        
        void drawScreenQuad(bool filled = true, float z = 0) const;    // z en coordonnées écran (dans [-1,1])
        
        // rapport hauteur / largeur du viewport :
        float ratio() const { return float(h) / float(w); }
        
        // ces méthodes servent à passer dans un repère 2D orthonormé contenant [xmin,xmax]*[ymin,ymax] :
        static void pushFrame2D(float xmin, float xmax, float ymin, float ymax);
        static void popFrame2D();
        
        static void push(); // met le viewport courant sur la pile d'attributs OpenGL
        static void pop();  // enleve le viewport de la pile d'attributs OpenGL
        
    public:
        int x, y, w, h;
    };

BABLIB_NAMESPACE_END

#endif

