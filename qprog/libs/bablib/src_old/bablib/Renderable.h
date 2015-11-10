#ifndef __RENDERABLE__
#define __RENDERABLE__

BABLIB_NAMESPACE_BEGIN

class Renderable {
    public:
        virtual ~Renderable() {}
        virtual void render() = 0;
    };

BABLIB_NAMESPACE_END

#endif

