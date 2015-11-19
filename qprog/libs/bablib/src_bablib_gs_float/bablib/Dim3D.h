#ifndef __DIM_3D__
#define __DIM_3D__

#include <QString>

BABLIB_NAMESPACE_BEGIN

class Dim3D {
    public:
        Dim3D() : defined(false), w(UNDEFINED), h(UNDEFINED), d(UNDEFINED) {}
        Dim3D(int w, int h, int d) : defined(w > UNDEFINED && h > UNDEFINED && d > UNDEFINED), w(defined ? w : UNDEFINED), h(defined ? h : UNDEFINED), d(defined ? d : UNDEFINED) {}

        int width() const { return w; }
        int height() const { return h; }
        int depth() const { return d; }
        bool isDefined() const { return defined; }
        bool isBetween(int min, int max) const { return w >= min && h >= min && w <= max && h <= max; }
        bool contains(int x, int y) const { return x >= 0 && x < w && y >= 0 && y < h; }
        bool contains(int x, int y, int z) const { return x >= 0 && x < w && y >= 0 && y < h && z >= 0 && z < d; }

        bool operator==(Dim3D s) const { return w == s.w && h == s.h && d == s.d; }
        bool operator!=(Dim3D s) const { return w != s.w || h != s.h || d != s.d; }

        QString toQString() const { return defined ? QString("(%1,%2,%3)").arg(w).arg(h).arg(d) : QString("<undefined>"); }

    private:
        bool defined;
        int w, h, d;
        static const int UNDEFINED = 0; // -1
    };

BABLIB_NAMESPACE_END

#endif

