#ifndef __COLOR_FILTER__
#define __COLOR_FILTER__

#include "ColorAdder.h"
#include "ColorTypes.h"

BABLIB_NAMESPACE_BEGIN

template<class Color>
class ColorFilter {
    public:
        virtual ~ColorFilter() {}   // pour ÃÂ©viter les warnings
        virtual void apply(Color &c) const = 0;
        //virtual Color filter(Color &c) const { Color res = c; apply(res); return res; }
    };

//@ macros creation rapide (avec seulement le contenu de 'apply')

template<class Color>
class ColorAlpha : public ColorFilter<Color> {
    public:
        ColorAlpha(float alpha) : alpha(alpha) {}
        virtual void apply(Color &c) const { c.setAlpha(alpha); }
    private:
        float alpha;
    };

template<class Color>
class ColorRed : public ColorFilter<Color> {
    public:
        ColorRed() {}
        virtual void apply(Color &c) const { c.r = 255; }
    };

template<class Color>
class ColorClamp : public ColorFilter<Color> {
    public:
        ColorClamp<Color>(float c0=0, float c1=1) : c0(c0), c1(c1) {}
        virtual void apply(Color &c) const;
    private:
        float c0, c1;
    };

// cette classe permet d'appliquer la mÃÂªme fonction <transfo> a chaque composante d'une couleur
template<class Color>
class ColorTransfoComp : public ColorFilter<Color> {
    public:
        ColorTransfoComp<Color>() {}
        virtual void apply(Color &c) const { for (int k=0; k<Color::SIZE; k++) c[k] = transfo(c[k]); }
        virtual float transfo(float x) const = 0;
    };

/*
#define FILTER_COLOR_FUNC(Color, color, function_of_x...) { \
    class : ColorTransfoComp<Color> { virtual float transfo(float x) const { return function_of_x; } } filter; \
    filter.apply(color);    \
    }
*/

BABLIB_NAMESPACE_END

#endif

