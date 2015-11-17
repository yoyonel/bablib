#ifndef __AABOX__
#define __AABOX__

#include <float.h>
#undef min
#undef max

#include <Message.h>
class AABox {
    public:
        AABox() : min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {}
    
        void add(const Vec3 &p) {
            if (p.x < min.x) min.x = p.x;
            if (p.y < min.y) min.y = p.y;
            if (p.z < min.z) min.z = p.z;
            if (p.x > max.x) max.x = p.x;
            if (p.y > max.y) max.y = p.y;
            if (p.z > max.z) max.z = p.z;
            }
        
        Vec3  center() const { return (min + max) / 2; }
        float radius() const { return (max - min).norm() / 2; }
        
    public:
        Vec3 min, max;
    };

#endif

