#include "viewer_sv.h"


void Viewer::resetFrameBuffer( FrameBuffer& _fbo, ProgGLSL& _prog_reset )
{
     _fbo.activate();
        ViewportGL::push();
            _fbo.viewport().setGL();
            _prog_reset.activate();
                _fbo.viewport().drawScreenQuad();
            _prog_reset.deactivate();
        ViewportGL::pop();
    _fbo.deactivate();
}

void Viewer::resetFrameBuffers()
{
}
