#ifndef __DIM_2D__
#define __DIM_2D__

#include <QString>

BABLIB_NAMESPACE_BEGIN

class Dim2D {
    public:
        Dim2D() : defined(false), w(UNDEFINED), h(UNDEFINED) {}
        //Dim2D(int w, int h) : defined(w > UNDEFINED && h > UNDEFINED), w(w), h(h) {}
        Dim2D(int w, int h) : defined(w > UNDEFINED && h > UNDEFINED), w(defined ? w : UNDEFINED), h(defined ? h : UNDEFINED) {}
        
        int width() const { return w; }
        int height() const { return h; }
        bool isDefined() const { return defined; }
        bool isBetween(int min, int max) const { return w >= min && h >= min && w <= max && h <= max; }
        bool contains(int x, int y) const { return x >= 0 && x < w && y >= 0 && y < h; }
        
        bool operator==(Dim2D s) const { return w == s.w && h == s.h; }
        bool operator!=(Dim2D s) const { return w != s.w || h != s.h; }
        
        QString toQString() const { return defined ? QString("(%1,%2)").arg(w).arg(h) : QString("<undefined>"); }
        
    private:
        bool defined;
        int w, h;
        static const int UNDEFINED = 0; // -1
    };

BABLIB_NAMESPACE_END

#endif

