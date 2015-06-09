#ifndef __UTIL_GL__
#define __UTIL_GL__

BABLIB_NAMESPACE_BEGIN

#include <Vec2.h>
#include <Vec3.h>

class UtilGL {
    public:
        static void unitQuadVertices();
        static void drawUnitQuad();

        static void drawAAQuad(Vec2 b0, Vec2 b1);
        static void drawAAQuadRaw(Vec2 b0, Vec2 b1);

        static void glvAAQuad(Vec2 b0, Vec2 b1);
        
        // to add:
        // - setup alpha blending
        // - glTriangle, etc.
        static void triangleFlat(Vec3 a, Vec3 b, Vec3 c);       // normal for flat shading
        static void triangleFlatRaw(Vec3 a, Vec3 b, Vec3 c);    // same but without glBegin/glEnd
        static void triangle(Vec3 a, Vec3 b, Vec3 c);           // no normal
    };

BABLIB_NAMESPACE_END

#endif


