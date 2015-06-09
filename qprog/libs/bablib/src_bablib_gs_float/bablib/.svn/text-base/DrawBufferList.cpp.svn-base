#include "DrawBufferList.h"
#include "Message.h"
//#include <GL/glprocs.h>
BABLIB_NAMESPACE_USE

namespace {
    int getGLConst(GLenum name) {
        int val;
        glGetIntegerv(name, &val);
        return val;
        }
    }

bool DrawBufferList::isColorBuffer(GLenum buffer) {
    int i = static_cast<int>(buffer) - GL_COLOR_ATTACHMENT0_EXT;
    return i >= 0 && i <= getGLConst(GL_MAX_COLOR_ATTACHMENTS_EXT);
    }

void DrawBufferList::add(GLenum buffer) {
    if (isColorBuffer(buffer)) {
        const int MAX_DRAW_BUFFERS = getGLConst(GL_MAX_DRAW_BUFFERS);
        if (n < MAX_DRAW_BUFFERS) {
            buffers.push_back(buffer);
            n++;
            }
        else
            Message::error(QString("nombre maximal de buffers autorise : %1").arg(MAX_DRAW_BUFFERS));
        }
    }

void DrawBufferList::activate() const {
    //glDrawBuffers(n, buffers);
    glDrawBuffersARB(n, buffers.constData());
    //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    }

void DrawBufferList::clear() {
    buffers.clear();
    n = 0;
    }


