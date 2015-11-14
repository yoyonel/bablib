#ifndef __PIXEL_1D_EVALUATOR__
#define __PIXEL_1D_EVALUATOR__

#include <OpenGL.h>
#include "Image1D.h"

BABLIB_NAMESPACE_BEGIN

class Pix1DEvaluator {
    public:
        Pix1DEvaluator(int n) : n(n) {}
        virtual ~Pix1DEvaluator() {}

        virtual int evaluate(int i) const = 0;
        inline int valuesNumber() const { return n; }
        
    private:
        int n;
    };


class Pix1DChannelEvaluator : public Pix1DEvaluator {
    public:
        Pix1DChannelEvaluator(Image1DUByte4 &image, int channel) : Pix1DEvaluator(256), image(image), channel(channel) {}
        virtual inline int evaluate(int i) const { return static_cast<int>(image(i)[channel]); }
    private:
        const Image1DUByte4 &image;
        const int channel;
    };

BABLIB_NAMESPACE_END

#endif
