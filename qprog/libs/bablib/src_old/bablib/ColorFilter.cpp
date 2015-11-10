#include <OpenGL.h>
#include "ColorFilter.h"
#include <math.h>

BABLIB_NAMESPACE_BEGIN

#ifndef fminf
#define fminf(_a, _b) (_a) < (_b) ? (_a) : (_b)
#define fmaxf(_a, _b) fminf(_b, _a)
#endif

#define CLAMP(x) x = fminf(fmaxf(x, c0), c1)
template<> void ColorClamp<Float4>::apply(Float4 &c) const { CLAMP(c.r); CLAMP(c.g); CLAMP(c.b); CLAMP(c.a); }
template<> void ColorClamp<Float3>::apply(Float3 &c) const { CLAMP(c.r); CLAMP(c.g); CLAMP(c.b); }
template<> void ColorClamp<Float2>::apply(Float2 &c) const { CLAMP(c.r); CLAMP(c.g); }
template<> void ColorClamp<Float1>::apply(Float1 &c) const { CLAMP(c.r); }
#undef CLAMP

BABLIB_NAMESPACE_END

