#ifndef __DISP_LIST_GL__
#define __DISP_LIST_GL__

#include <OpenGL.h>
#include "Renderable.h"

BABLIB_NAMESPACE_BEGIN

class DispListGL : public Renderable {
    public:
        DispListGL() { id = glGenLists(1); }
        ~DispListGL() { glDeleteLists(id, 1); }
        
        void begin(bool execute=false) const { glNewList(id, execute ? GL_COMPILE_AND_EXECUTE : GL_COMPILE); }
        void end()   const { glEndList(); }
        void exec()  const { glCallList(id); }

        // method of Renderable :
        virtual void render() { exec(); }
        
        // creates a display list ready to be rendered from a renderable object :
        static DispListGL* create(Renderable *obj) {
            DispListGL *res = new DispListGL();
            res->begin();
            obj->render();
            res->end();
            return res;
            }

    private:
        GLuint id;
    };

BABLIB_NAMESPACE_END

#endif

