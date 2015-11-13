#ifndef __COLOR_ADDER__
#define __COLOR_ADDER__

#include "ColorTypes.h"

BABLIB_NAMESPACE_BEGIN

template<class Color>
class ColorAdder {
    public:
        ColorAdder() { reset(); }
        void  reset()      { n = 0; adder.reset(); }
        void  add(Color c, float k = 1) { adder += k * c; n += k; }
        Color getColor()   { return adder / n; }    //@@ changer : appeler ça getAverage()
        Color getSum()   { return adder; }          //@@ appeler getColor
        
        
    public:
        float n;        // nombre de pixels ajoutés depuis le dernier reset
        Color adder;    // couleur courante
    };

template<>
class ColorAdder<UByte4> {
    public:
        ColorAdder() { reset(); }
        void  reset()      { n = 0; adder.reset(); }
        void  add(UByte4 c, float k = 1) { adder += k * c.toUInt4(); n += k; }
        UByte4 getColor()  { return (adder / n).toUByte4(); }
        UByte4 getSum()    { return adder.toUByte4(); }
        
    public:
        float n;        // nombre de pixels ajoutés depuis le dernier reset
        UInt4 adder;    // couleur courante
    };

BABLIB_NAMESPACE_END

#endif
