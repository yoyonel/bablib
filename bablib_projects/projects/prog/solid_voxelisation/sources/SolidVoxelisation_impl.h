#ifndef SOLIDVOXELISATION_IMPL_H
#define SOLIDVOXELISATION_IMPL_H

template <typename F>
bool SolidVoxelisation::update(F _meth_draw) {
    // ----------
    const GLenum clearBufferBits = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
    const GLboolean GL_ALL_BITS = GLboolean(~0x0);
    const GLboolean colorMask[] = {GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS};

    FrameBuffer& _framebuffer = m_fb_sv;
    qglviewer::Camera& qgl_cam_light_mf = m_cam;

    glPushAttrib( GL_ALL_ATTRIB_BITS ); {
        _framebuffer.activate();
        glViewport(0, 0, _framebuffer.width(), _framebuffer.height());

        //setOpenGLStates();

        glColorMask( colorMask[0], colorMask[1], colorMask[2], colorMask[3] );
        glClearColor( 0x0, 0x0, 0x0, 0x0 );
        glClear( clearBufferBits );

        // desactivation du test de profondeur
        glDisable( GL_DEPTH_TEST );

        // desactivation du culling face
        glDisable(GL_CULL_FACE);

        // set blending mode
        glEnable( GL_BLEND );
        glLogicOp( GL_XOR );
        glEnable( GL_COLOR_LOGIC_OP );

        glDisable(GL_LIGHTING);

        // -------

        // Set light's camera (OpenGL 1.x style)
        // Sauvegarde des matrices de transformations
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        qgl_cam_light_mf.loadProjectionMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        //pt_world_to_light.glLoadModelView();
        qgl_cam_light_mf.loadModelViewMatrix();

        // Draw scene
        m_prog_sv.activate(); {
            _meth_draw(m_prog_sv);
        } m_prog_sv.deactivate();

        // Restore the OpenGL Matrix
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        //
        _framebuffer.deactivate();
    } glPopAttrib();

    MSG_CHECK_GL;

    return true;
}

#endif // SOLIDVOXELISATION_IMPL_H

