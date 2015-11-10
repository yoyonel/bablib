#ifndef __TRI__
#define __TRI__

#include <Vec3.h>

struct Tri {
    int index[3];
    Vec3 normal;
    Vec3 color;         // just for debug
    Vec3 debugColor;    // just for debug
    };

#endif

