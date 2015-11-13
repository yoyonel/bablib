#ifndef __PIXEL_2D_EVALUATOR__
#define __PIXEL_2D_EVALUATOR__

#include <OpenGL.h>
#include "Image2D.h"

BABLIB_NAMESPACE_BEGIN

class Pix2DEvaluator {
    public:
        Pix2DEvaluator(int n) : n(n) {}
        virtual ~Pix2DEvaluator() {}

        virtual int evaluate(int i, int j) const = 0;
        inline int valuesNumber() const { return n; }
        
    private:
        int n;
    };


class DepthPix2DEvaluator : public Pix2DEvaluator {
    public:
        DepthPix2DEvaluator(Image2DFloat &depthBuffer, GLfloat depthClearValue = 0) : Pix2DEvaluator(2), depthBuffer(depthBuffer), depthClearValue(depthClearValue) {}
        virtual inline int evaluate(int i, int j) const { return depthBuffer(i, j).r == depthClearValue ? 0 : 1; }
    private:
        const Image2DFloat& depthBuffer;
        const GLfloat depthClearValue;
    };

class Pix2DChannelEvaluator : public Pix2DEvaluator {
    public:
        Pix2DChannelEvaluator(Image2DUByte4 &image, int channel) : Pix2DEvaluator(256), image(image), channel(channel) {}
        virtual inline int evaluate(int i, int j) const { return static_cast<int>(image(i, j)[channel]); }
    private:
        const Image2DUByte4 &image;
        const int channel;
    };

BABLIB_NAMESPACE_END

#endif

