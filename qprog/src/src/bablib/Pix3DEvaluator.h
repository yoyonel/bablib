#ifndef __PIXEL_3D_EVALUATOR__
#define __PIXEL_3D_EVALUATOR__

#include <OpenGL.h>
#include "Image3D.h"

BABLIB_NAMESPACE_BEGIN

class Pix3DEvaluator {
    public:
        Pix3DEvaluator(int n) : n(n) {}
        virtual ~Pix3DEvaluator() {}

        virtual int evaluate(int i, int j, int k) const = 0;
        inline int valuesNumber() const { return n; }
        
    private:
        int n;
    };


class Pix3DChannelEvaluator : public Pix3DEvaluator {
    public:
        Pix3DChannelEvaluator(Image3DUByte4 &image, int channel) : Pix3DEvaluator(256), image(image), channel(channel) {}
        virtual inline int evaluate(int i, int j, int k) const { return static_cast<int>(image(i, j, k)[channel]); }
    private:
        const Image3DUByte4 &image;
        const int channel;
    };

BABLIB_NAMESPACE_END

#endif

