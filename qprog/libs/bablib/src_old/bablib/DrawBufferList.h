#ifndef __DRAW_BUFFER_LIST__
#define __DRAW_BUFFER_LIST__

#include <OpenGL.h>
#include <QVector>

BABLIB_NAMESPACE_BEGIN

// cette classe sert a representer l'ensemble des drawbuffers utilises par un framebuffer :
class DrawBufferList {
    public:
        DrawBufferList() : n(0) {}
        
        void add(GLenum buffer);
        void activate() const;
        void clear();
    
    private:
        static bool isColorBuffer(GLenum buffer);    
    
    private:
        int n;
        QVector<GLenum> buffers;    // contient la liste des buffers qui doivent être dessinés
    };


BABLIB_NAMESPACE_END

#endif

