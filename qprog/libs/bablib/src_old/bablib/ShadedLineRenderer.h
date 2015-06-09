#ifndef __SHADED_LINE_RENDERER__
#define __SHADED_LINE_RENDERER__

#include "Vec3d.h"
#include "Vec4d.h"
#include "Vec3.h"
#include "ProgGLSL.h"

BABLIB_NAMESPACE_BEGIN

class ShadedLineRenderer {
    public:
        ShadedLineRenderer();
        ~ShadedLineRenderer();
        
        void begin();
        void draw(const Vec3 &a, const Vec3 &b);
        void draw(const Vec3d &a, const Vec3d &b);
        void draw(const Vec4d &a, const Vec4d &b);
        void end();
        
    public:
        ProgGLSL prog;
    };

BABLIB_NAMESPACE_END

#endif

